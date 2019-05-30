/* Scan linker error messages for missing template instantiations and provide
   them.

   Copyright (C) 1995-2019 Free Software Foundation, Inc.
   Contributed by Jason Merrill (jason@cygnus.com).

This file is part of GCC.

GCC is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free
Software Foundation; either version 3, or (at your option) any later
version.

GCC is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or
FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
for more details.

You should have received a copy of the GNU General Public License
along with GCC; see the file COPYING3.  If not see
<http://www.gnu.org/licenses/>.  */

#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "tm.h"
#include "intl.h"
#include "obstack.h"
#include "demangle.h"
#include "collect2.h"
#include "collect-utils.h"
#include "filenames.h"
#include "diagnostic-core.h"

/* TARGET_64BIT may be defined to use driver specific functionality. */
#undef TARGET_64BIT
#define TARGET_64BIT TARGET_64BIT_DEFAULT

#define MAX_ITERATIONS 17

/* Defined in the automatically-generated underscore.c.  */
extern int prepends_underscore;

static int tlink_verbose;

static char *initial_cwd;

/* Hash table boilerplate for working with htab_t.  We have hash tables
   for symbol names, file names, and demangled symbols.  */

typedef struct symbol_hash_entry
{
  const char *key;
  struct file_hash_entry *file;
  int chosen;
  int tweaking;
  int tweaked;
} symbol;

typedef struct file_hash_entry
{
  const char *key;
  const char *args;
  const char *dir;
  const char *main;
  int tweaking;
} file;

typedef const char *str;

typedef struct demangled_hash_entry
{
  const char *key;
  vec<str> mangled;
} demangled;

/* Hash and comparison functions for these hash tables.  */

static int hash_string_eq (const void *, const void *);
static hashval_t hash_string_hash (const void *);

static int
hash_string_eq (const void *s1_p, const void *s2_p)
{
  const char *const *s1 = (const char *const *) s1_p;
  const char *s2 = (const char *) s2_p;
  return strcmp (*s1, s2) == 0;
}

static hashval_t
hash_string_hash (const void *s_p)
{
  const char *const *s = (const char *const *) s_p;
  return (*htab_hash_string) (*s);
}

static htab_t symbol_table;

static struct symbol_hash_entry * symbol_hash_lookup (const char *, int);
static struct file_hash_entry * file_hash_lookup (const char *);
static struct demangled_hash_entry *demangled_hash_lookup (const char *, int);
static void symbol_push (symbol *);
static symbol * symbol_pop (void);
static void file_push (file *);
static file * file_pop (void);
static char * frob_extension (const char *, const char *);
static char * obstack_fgets (FILE *, struct obstack *);
static char * tfgets (FILE *);
static char * pfgets (FILE *);
static void freadsym (FILE *, file *, int);
static void read_repo_file (file *);
static void maybe_tweak (char *, file *);
static int recompile_files (void);
static int read_repo_files (char **);
static void demangle_new_symbols (void);
static int scan_linker_output (const char *);

/* Look up an entry in the symbol hash table.  */

static struct symbol_hash_entry *
symbol_hash_lookup (const char *string, int create)
{
  void **e;
  e = htab_find_slot_with_hash (symbol_table, string,
				(*htab_hash_string) (string),
				create ? INSERT : NO_INSERT);
  if (e == NULL)
    return NULL;
  if (*e == NULL)
    {
      struct symbol_hash_entry *v;
      *e = v = XCNEW (struct symbol_hash_entry);
      v->key = xstrdup (string);
    }
  return (struct symbol_hash_entry *) *e;
}

static htab_t file_table;

/* Look up an entry in the file hash table.  */

static struct file_hash_entry *
file_hash_lookup (const char *string)
{
  void **e;
  e = htab_find_slot_with_hash (file_table, string,
				(*htab_hash_string) (string),
				INSERT);
  if (*e == NULL)
    {
      struct file_hash_entry *v;
      *e = v = XCNEW (struct file_hash_entry);
      v->key = xstrdup (string);
    }
  return (struct file_hash_entry *) *e;
}

static htab_t demangled_table;

/* Look up an entry in the demangled name hash table.  */

static struct demangled_hash_entry *
demangled_hash_lookup (const char *string, int create)
{
  void **e;
  e = htab_find_slot_with_hash (demangled_table, string,
				(*htab_hash_string) (string),
				create ? INSERT : NO_INSERT);
  if (e == NULL)
    return NULL;
  if (*e == NULL)
    {
      struct demangled_hash_entry *v;
      *e = v = XCNEW (struct demangled_hash_entry);
      v->key = xstrdup (string);
    }
  return (struct demangled_hash_entry *) *e;
}

/* Stack code.  */

struct symbol_stack_entry
{
  symbol *value;
  struct symbol_stack_entry *next;
};
struct obstack symbol_stack_obstack;
struct symbol_stack_entry *symbol_stack;

struct file_stack_entry
{
  file *value;
  struct file_stack_entry *next;
};
struct obstack file_stack_obstack;
struct file_stack_entry *file_stack;

static void
symbol_push (symbol *p)
{
  struct symbol_stack_entry *ep
    = XOBNEW (&symbol_stack_obstack, struct symbol_stack_entry);
  ep->value = p;
  ep->next = symbol_stack;
  symbol_stack = ep;
}

static symbol *
symbol_pop (void)
{
  struct symbol_stack_entry *ep = symbol_stack;
  symbol *p;
  if (ep == NULL)
    return NULL;
  p = ep->value;
  symbol_stack = ep->next;
  obstack_free (&symbol_stack_obstack, ep);
  return p;
}

static void
file_push (file *p)
{
  struct file_stack_entry *ep;

  if (p->tweaking)
    return;

  ep = XOBNEW (&file_stack_obstack, struct file_stack_entry);
  ep->value = p;
  ep->next = file_stack;
  file_stack = ep;
  p->tweaking = 1;
}

static file *
file_pop (void)
{
  struct file_stack_entry *ep = file_stack;
  file *p;
  if (ep == NULL)
    return NULL;
  p = ep->value;
  file_stack = ep->next;
  obstack_free (&file_stack_obstack, ep);
  p->tweaking = 0;
  return p;
}

/* Other machinery.  */

/* Initialize the tlink machinery.  Called from do_tlink.  */

static void
tlink_init (void)
{
  const char *p;

  symbol_table = htab_create (500, hash_string_hash, hash_string_eq,
			      NULL);
  file_table = htab_create (500, hash_string_hash, hash_string_eq,
			    NULL);
  demangled_table = htab_create (500, hash_string_hash, hash_string_eq,
				 NULL);

  obstack_begin (&symbol_stack_obstack, 0);
  obstack_begin (&file_stack_obstack, 0);

  p = getenv ("TLINK_VERBOSE");
  if (p)
    tlink_verbose = atoi (p);
  else
    {
      tlink_verbose = 1;
      if (verbose)
	tlink_verbose = 2;
      if (debug)
	tlink_verbose = 3;
    }

  initial_cwd = getpwd ();
}

static int
tlink_execute (const char *prog, char **argv, const char *outname,
	       const char *errname, bool use_atfile)
{
  struct pex_obj *pex;

  pex = collect_execute (prog, argv, outname, errname,
			 PEX_LAST | PEX_SEARCH, use_atfile);
  return collect_wait (prog, pex);
}

static char *
frob_extension (const char *s, const char *ext)
{
  const char *p;

  p = strrchr (lbasename (s), '.');
  if (! p)
    p = s + strlen (s);

  obstack_grow (&temporary_obstack, s, p - s);
  return (char *) obstack_copy0 (&temporary_obstack, ext, strlen (ext));
}

static char *
obstack_fgets (FILE *stream, struct obstack *ob)
{
  int c;
  while ((c = getc (stream)) != EOF && c != '\n')
    obstack_1grow (ob, c);
  if (obstack_object_size (ob) == 0)
    return NULL;
  obstack_1grow (ob, '\0');
  return XOBFINISH (ob, char *);
}

static char *
tfgets (FILE *stream)
{
  return obstack_fgets (stream, &temporary_obstack);
}

static char *
pfgets (FILE *stream)
{
  return xstrdup (tfgets (stream));
}

/* Real tlink code.  */

/* Subroutine of read_repo_file.  We are reading the repo file for file F,
   which is coming in on STREAM, and the symbol that comes next in STREAM
   is offered, chosen or provided if CHOSEN is 0, 1 or 2, respectively.

   XXX "provided" is unimplemented, both here and in the compiler.  */

static void
freadsym (FILE *stream, file *f, int chosen)
{
  symbol *sym;

  {
    const char *name = tfgets (stream);
    sym = symbol_hash_lookup (name, true);
  }

  if (sym->file == NULL)
    {
      /* We didn't have this symbol already, so we choose this file.  */

      symbol_push (sym);
      sym->file = f;
      sym->chosen = chosen;
    }
  else if (chosen)
    {
      /* We want this file; cast aside any pretender.  */

      if (sym->chosen && sym->file != f)
	{
	  if (sym->chosen == 1)
	    file_push (sym->file);
	  else
	    {
	      file_push (f);
	      f = sym->file;
	      chosen = sym->chosen;
	    }
	}
      sym->file = f;
      sym->chosen = chosen;
    }
}

/* Read in the repo file denoted by F, and record all its information.  */

static void
read_repo_file (file *f)
{
  char c;
  FILE *stream = fopen (f->key, "r");

  if (tlink_verbose >= 2)
    fprintf (stderr, _("collect: reading %s\n"), f->key);

  while (fscanf (stream, "%c ", &c) == 1)
    {
      switch (c)
	{
	case 'A':
	  f->args = pfgets (stream);
	  break;
	case 'D':
	  f->dir = pfgets (stream);
	  break;
	case 'M':
	  f->main = pfgets (stream);
	  break;
	case 'P':
	  freadsym (stream, f, 2);
	  break;
	case 'C':
	  freadsym (stream, f, 1);
	  break;
	case 'O':
	  freadsym (stream, f, 0);
	  break;
	}
      obstack_free (&temporary_obstack, temporary_firstobj);
    }
  fclose (stream);
  if (f->args == NULL)
    f->args = getenv ("COLLECT_GCC_OPTIONS");
  if (f->dir == NULL)
    f->dir = ".";
}

/* We might want to modify LINE, which is a symbol line from file F.  We do
   this if either we saw an error message referring to the symbol in
   question, or we have already allocated the symbol to another file and
   this one wants to emit it as well.  */

static void
maybe_tweak (char *line, file *f)
{
  symbol *sym = symbol_hash_lookup (line + 2, false);

  if ((sym->file == f && sym->tweaking)
      || (sym->file != f && line[0] == 'C'))
    {
      sym->tweaking = 0;
      sym->tweaked = 1;

      if (line[0] == 'O')
	{
	  line[0] = 'C';
	  sym->chosen = 1;
	}
      else
	{
	  line[0] = 'O';
	  sym->chosen = 0;
	}
    }
}

/* Update the repo files for each of the object files we have adjusted and
   recompile.  */

static int
recompile_files (void)
{
  file *f;

  putenv (xstrdup ("COMPILER_PATH="));
  putenv (xstrdup ("LIBRARY_PATH="));

  while ((f = file_pop ()) != NULL)
    {
      char *line;
      const char *p, *q;
      char **argv;
      struct obstack arg_stack;
      FILE *stream = fopen (f->key, "r");
      const char *const outname = frob_extension (f->key, ".rnw");
      FILE *output = fopen (outname, "w");

      while ((line = tfgets (stream)) != NULL)
	{
	  switch (line[0])
	    {
	    case 'C':
	    case 'O':
	      maybe_tweak (line, f);
	    }
	  fprintf (output, "%s\n", line);
	}
      fclose (stream);
      fclose (output);
      /* On Windows "rename" returns -1 and sets ERRNO to EACCESS if
	 the new file name already exists.  Therefore, we explicitly
	 remove the old file first.  */
      if (remove (f->key) == -1)
	fatal_error (input_location,
		     "removing repository file %qs: %m", f->key);
      if (rename (outname, f->key) == -1)
	fatal_error (input_location, "renaming repository file from "
		     "%qs to %qs: %m", outname, f->key);

      if (!f->args)
	{
	  error ("repository file %qs does not contain command-line "
		 "arguments", f->key);
	  return 0;
	}

      /* Build a null-terminated argv array suitable for
	 tlink_execute().  Manipulate arguments on the arg_stack while
	 building argv on the temporary_obstack.  */

      obstack_init (&arg_stack);
      obstack_ptr_grow (&temporary_obstack, c_file_name);

      for (p = f->args; *p != '\0'; p = q + 1)
	{
	  /* Arguments are delimited by single-quotes.  Find the
	     opening quote.  */
	  p = strchr (p, '\'');
	  if (!p)
	    goto done;

	  /* Find the closing quote.  */
	  q = strchr (p + 1, '\'');
	  if (!q)
	    goto done;

	  obstack_grow (&arg_stack, p + 1, q - (p + 1));

	  /* Replace '\'' with '.  This is how set_collect_gcc_options
	     encodes a single-quote.  */
	  while (q[1] == '\\' && q[2] == '\'' && q[3] == '\'')
	    {
	      const char *r;

	      r = strchr (q + 4, '\'');
	      if (!r)
		goto done;

	      obstack_grow (&arg_stack, q + 3, r - (q + 3));
	      q = r;
	    }

	  obstack_1grow (&arg_stack, '\0');
	  obstack_ptr_grow (&temporary_obstack, obstack_finish (&arg_stack));
	}
    done:
      obstack_ptr_grow (&temporary_obstack, f->main);
      obstack_ptr_grow (&temporary_obstack, NULL);
      argv = XOBFINISH (&temporary_obstack, char **);

      if (tlink_verbose)
	fprintf (stderr, _("collect: recompiling %s\n"), f->main);

      if (chdir (f->dir) != 0
	  || tlink_execute (c_file_name, argv, NULL, NULL, false) != 0
	  || chdir (initial_cwd) != 0)
	return 0;

      read_repo_file (f);

      obstack_free (&arg_stack, NULL);
      obstack_free (&temporary_obstack, temporary_firstobj);
    }
  return 1;
}

/* The first phase of processing: determine which object files have
   .rpo files associated with them, and read in the information.  */

static int
read_repo_files (char **object_lst)
{
  char **object = object_lst;

  for (; *object; object++)
    {
      const char *p;
      file *f;

      /* Don't bother trying for ld flags.  */
      if (*object[0] == '-')
	continue;

      p = frob_extension (*object, ".rpo");

      if (! file_exists (p))
	continue;

      f = file_hash_lookup (p);

      read_repo_file (f);
    }

  if (file_stack != NULL && ! recompile_files ())
    return 0;

  return (symbol_stack != NULL);
}

/* Add the demangled forms of any new symbols to the hash table.  */

static void
demangle_new_symbols (void)
{
  symbol *sym;

  while ((sym = symbol_pop ()) != NULL)
    {
      demangled *dem;
      const char *p = cplus_demangle (sym->key, DMGL_PARAMS | DMGL_ANSI);

      if (! p)
	continue;

      dem = demangled_hash_lookup (p, true);
      dem->mangled.safe_push (sym->key);
    }
}

/* We want to tweak symbol SYM.  Return true if all is well, false on
   error.  */

static bool
start_tweaking (symbol *sym)
{
  if (sym && sym->tweaked)
    {
      error ("%qs was assigned to %qs, but was not defined "
	     "during recompilation, or vice versa",
	     sym->key, sym->file->key);
      return 0;
    }
  if (sym && !sym->tweaking)
    {
      if (tlink_verbose >= 2)
	fprintf (stderr, _("collect: tweaking %s in %s\n"),
		 sym->key, sym->file->key);
      sym->tweaking = 1;
      file_push (sym->file);
    }
  return true;
}

/* Step through the output of the linker, in the file named FNAME, and
   adjust the settings for each symbol encountered.  */

static int
scan_linker_output (const char *fname)
{
  FILE *stream = fopen (fname, "r");
  char *line;
  int skip_next_in_line = 0;

  while ((line = tfgets (stream)) != NULL)
    {
      char *p = line, *q;
      symbol *sym;
      demangled *dem = 0;
      int end;
      int ok = 0;
      unsigned ix;
      str s;

      /* On darwin9, we might have to skip " in " lines as well.  */
      if (skip_next_in_line
	  && strstr (p, " in "))
	  continue;
      skip_next_in_line = 0;

      while (*p && ISSPACE ((unsigned char) *p))
	++p;

      if (! *p)
	continue;

      for (q = p; *q && ! ISSPACE ((unsigned char) *q); ++q)
	;

      /* Try the first word on the line.  */
      if (*p == '.')
	++p;
      if (!strncmp (p, USER_LABEL_PREFIX, strlen (USER_LABEL_PREFIX)))
	p += strlen (USER_LABEL_PREFIX);

      end = ! *q;
      *q = 0;
      sym = symbol_hash_lookup (p, false);

      /* Some SVR4 linkers produce messages like
	 ld: 0711-317 ERROR: Undefined symbol: .g__t3foo1Zi
	 */
      if (! sym && ! end && strstr (q + 1, "Undefined symbol: "))
	{
	  char *p = strrchr (q + 1, ' ');
	  p++;
	  if (*p == '.')
	    p++;
	  if (!strncmp (p, USER_LABEL_PREFIX, strlen (USER_LABEL_PREFIX)))
	    p += strlen (USER_LABEL_PREFIX);
	  sym = symbol_hash_lookup (p, false);
	}

      if (! sym && ! end)
	/* Try a mangled name in quotes.  */
	{
	  char *oldq = q + 1;
	  q = 0;

	  /* On darwin9, we look for "foo" referenced from:\n\(.* in .*\n\)*  */
	  if (strcmp (oldq, "referenced from:") == 0)
	    {
	      /* We have to remember that we found a symbol to tweak.  */
	      ok = 1;

	      /* We actually want to start from the first word on the
		 line.  */
	      oldq = p;

	      /* Since the format is multiline, we have to skip
		 following lines with " in ".  */
	      skip_next_in_line = 1;
	    }

	  /* First try `GNU style'.  */
	  p = strchr (oldq, '`');
	  if (p)
	    p++, q = strchr (p, '\'');
	  /* Then try "double quotes".  */
	  else if (p = strchr (oldq, '"'), p)
	    p++, q = strchr (p, '"');
	  /* Then try 'single quotes'.  */
	  else if (p = strchr (oldq, '\''), p)
	    p++, q = strchr (p, '\'');
	  else {
	    /* Then try entire line.  */
	    q = strchr (oldq, 0);
	    if (q != oldq)
	      p = (char *)oldq;
	  }

	  if (p)
	    {
	      /* Don't let the strstr's below see the demangled name; we
		 might get spurious matches.  */
	      p[-1] = '\0';

	      /* powerpc64-linux references .foo when calling function foo.  */
	      if (*p == '.')
		p++;
	    }

	  /* We need to check for certain error keywords here, or we would
	     mistakenly use GNU ld's "In function `foo':" message.  */
	  if (q && (ok
		    || strstr (oldq, "ndefined")
		    || strstr (oldq, "nresolved")
		    || strstr (oldq, "nsatisfied")
		    || strstr (oldq, "ultiple")))
	    {
	      *q = 0;
	      dem = demangled_hash_lookup (p, false);
	      if (!dem)
		{
		  if (!strncmp (p, USER_LABEL_PREFIX,
				strlen (USER_LABEL_PREFIX)))
		    p += strlen (USER_LABEL_PREFIX);
		  sym = symbol_hash_lookup (p, false);
		}
	    }
	}

      if (dem)
	{
	  /* We found a demangled name.  If this is the name of a
	     constructor or destructor, there can be several mangled names
	     that match it, so choose or unchoose all of them.  If some are
	     chosen and some not, leave the later ones that don't match
	     alone for now; either this will cause the link to succeed, or
	     on the next attempt we will switch all of them the other way
	     and that will cause it to succeed.  */
	  int chosen = 0;
	  int len = dem->mangled.length ();
	  ok = true;
	  FOR_EACH_VEC_ELT (dem->mangled, ix, s)
	    {
	      sym = symbol_hash_lookup (s, false);
	      if (ix == 0)
		chosen = sym->chosen;
	      else if (sym->chosen != chosen)
		/* Mismatch.  */
		continue;
	      /* Avoid an error about re-tweaking when we guess wrong in
		 the case of mismatch.  */
	      if (len > 1)
		sym->tweaked = false;
	      ok = start_tweaking (sym);
	    }
	}
      else
	ok = start_tweaking (sym);

      obstack_free (&temporary_obstack, temporary_firstobj);

      if (!ok)
	{
	  fclose (stream);
	  return 0;
	}
    }

  fclose (stream);
  return (file_stack != NULL);
}

/* Entry point for tlink.  Called from main in collect2.c.

   Iteratively try to provide definitions for all the unresolved symbols
   mentioned in the linker error messages.

   LD_ARGV is an array of arguments for the linker.
   OBJECT_LST is an array of object files that we may be able to recompile
     to provide missing definitions.  Currently ignored.  */

void
do_tlink (char **ld_argv, char **object_lst ATTRIBUTE_UNUSED)
{
  int ret = tlink_execute ("ld", ld_argv, ldout, lderrout,
			   HAVE_GNU_LD && at_file_supplied);

  tlink_init ();

  if (ret)
    {
      int i = 0;

      /* Until collect does a better job of figuring out which are object
	 files, assume that everything on the command line could be.  */
      if (read_repo_files (ld_argv))
	while (ret && i++ < MAX_ITERATIONS)
	  {
	    if (tlink_verbose >= 3)
	      {
		dump_ld_file (ldout, stdout);
		dump_ld_file (lderrout, stderr);
	      }
	    demangle_new_symbols ();
	    if (! scan_linker_output (ldout)
		&& ! scan_linker_output (lderrout))
	      break;
	    if (! recompile_files ())
	      break;
	    if (tlink_verbose)
	      fprintf (stderr, _("collect: relinking\n"));
	    ret = tlink_execute ("ld", ld_argv, ldout, lderrout,
				 HAVE_GNU_LD && at_file_supplied);
	  }
    }

  dump_ld_file (ldout, stdout);
  unlink (ldout);
  dump_ld_file (lderrout, stderr);
  unlink (lderrout);
  if (ret)
    {
      error ("ld returned %d exit status", ret);
      exit (ret);
    }
  else
    {
      /* We have just successfully produced an output file, so assume that we
	 may unlink it if need be for now on.  */ 
      may_unlink_output_file = true;
    }
}
