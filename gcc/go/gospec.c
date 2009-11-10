/* gospec.c -- Specific flags and argument handling of the gcc Go front end.

   Copyright 2009 The Go Authors. All rights reserved.
   Use of this source code is governed by a BSD-style
   license that can be found in the LICENSE file.  */

#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "tm.h"
#include "gcc.h"

/* This bit is set if we saw a `-xfoo' language specification.  */
#define LANGSPEC	(1<<1)
/* This bit is set if they did `-lm' or `-lmath'.  */
#define MATHLIB		(1<<2)
/* This bit is set if they did `-lc'.  */
#define WITHLIBC	(1<<3)
/* Skip this option.  */
#define SKIPOPT		(1<<4)

#ifndef MATH_LIBRARY
#define MATH_LIBRARY "-lm"
#endif
#ifndef MATH_LIBRARY_PROFILE
#define MATH_LIBRARY_PROFILE MATH_LIBRARY
#endif

#ifndef LIBGO
#define LIBGO "-lgo"
#endif
#ifndef LIBGOBEGIN
#define LIBGOBEGIN "-lgobegin"
#endif
#ifndef LIBGO_PROFILE
#define LIBGO_PROFILE LIBGO
#endif

void
lang_specific_driver (int *in_argc, const char *const **in_argv,
		      int *in_added_libraries)
{
  int i, j;

  /* If nonzero, the user gave us the `-p' or `-pg' flag.  */
  int saw_profile_flag = 0;

  /* If nonzero, the user gave us the `-v' flag.  */
  int saw_verbose_flag = 0;

  /* This is a tristate:
     -1 means we should not link in libgo
     0  means we should link in libgo if it is needed
     1  means libgo is needed and should be linked in.
     2  means libgo is needed and should be linked statically.  */
  int library = 0;

  /* Used to track options that take arguments.  */
  const char *quote = NULL;

  /* The new argument list will be contained in this.  */
  const char **arglist;

  /* Nonzero if we saw a `-xfoo' language specification on the command
     line.  */
  int saw_speclang = 0;

  /* "-lm" or "-lmath" if it appears on the command line.  */
  const char *saw_math = 0;

  /* "-lc" if it appears on the command line.  */
  const char *saw_libc = 0;

  /* An array used to flag each argument that needs a bit set for
     LANGSPEC, MATHLIB, or WITHLIBC.  */
  int *args;

  /* By default, we throw on the math library if we have one.  */
  int need_math = (MATH_LIBRARY[0] != '\0');

  /* True if we saw -static.  */
  int static_link = 0;

  /* True if we should add -shared-libgcc to the command-line.  */
  int shared_libgcc = 1;

  /* The total number of arguments with the new stuff.  */
  int argc;

  /* The argument list.  */
  const char *const *argv;

  /* The number of libraries added in.  */
  int added_libraries;

  /* The total number of arguments with the new stuff.  */
  int num_args = 1;

  argc = *in_argc;
  argv = *in_argv;
  added_libraries = *in_added_libraries;

  args = XCNEWVEC (int, argc);

  for (i = 1; i < argc; i++)
    {
      /* If the previous option took an argument, we swallow it here.  */
      if (quote)
	{
	  quote = NULL;
	  continue;
	}

      /* We don't do this anymore, since we don't get them with minus
	 signs on them.  */
      if (argv[i][0] == '\0' || argv[i][1] == '\0')
	continue;

      if (argv[i][0] == '-')
	{
	  if (strcmp (argv[i], "-nostdlib") == 0
	      || strcmp (argv[i], "-nodefaultlibs") == 0)
	    {
	      library = -1;
	    }
	  else if (strcmp (argv[i], MATH_LIBRARY) == 0)
	    {
	      args[i] |= MATHLIB;
	      need_math = 0;
	    }
	  else if (strcmp (argv[i], "-lc") == 0)
	    args[i] |= WITHLIBC;
	  else if (strcmp (argv[i], "-pg") == 0 || strcmp (argv[i], "-p") == 0)
	    saw_profile_flag++;
	  else if (strcmp (argv[i], "-v") == 0)
	    saw_verbose_flag = 1;
	  else if (strncmp (argv[i], "-x", 2) == 0)
	    {
	      const char * arg;
	      if (argv[i][2] != '\0')
		arg = argv[i]+2;
	      else if ((argv[i+1]) != NULL)
		/* We need to swallow arg on next loop.  */
		quote = arg = argv[i+1];
  	      else  /* Error condition, message will be printed later.  */
		arg = "";
	      if (library == 0 && strcmp (arg, "go") == 0)
		library = 1;

	      saw_speclang = 1;
	    }
	  /* Arguments that go directly to the linker might be .o files,
	     or something, and so might cause libgo to be needed.  */
	  else if (strcmp (argv[i], "-Xlinker") == 0)
	    {
	      quote = argv[i];
	      if (library == 0)
		library = 1;
	    }
	  else if (strncmp (argv[i], "-Wl,", 4) == 0)
	    library = (library == 0) ? 1 : library;
	  /* Unrecognized libraries (e.g. -lfoo) may require libgo.  */
	  else if (strncmp (argv[i], "-l", 2) == 0)
	    library = (library == 0) ? 1 : library;
	  else if (((argv[i][2] == '\0'
		     && strchr ("bBVDUoeTuIYmLiA", argv[i][1]) != NULL)
		    || strcmp (argv[i], "-Tdata") == 0))
	    quote = argv[i];
	  else if ((argv[i][2] == '\0'
		    && strchr ("cSEM", argv[i][1]) != NULL)
		   || strcmp (argv[i], "-MM") == 0
		   || strcmp (argv[i], "-fsyntax-only") == 0)
	    {
	      /* Don't specify libraries if we won't link, since that would
		 cause a warning.  */
	      library = -1;
	    }
	  else if (strcmp (argv[i], "-static") == 0)
	    static_link = 1;
	  else if (strcmp (argv[i], "-static-libgcc") == 0)
	    shared_libgcc = 0;
	  else if (strcmp (argv[i], "-static-libgo") == 0)
	    {
	      library = library >= 0 ? 2 : library;
	      args[i] |= SKIPOPT;
	    }
	  else if (DEFAULT_WORD_SWITCH_TAKES_ARG (&argv[i][1]))
	    i++;
	  else
	    /* Pass other options through.  */
	    continue;
	}
      else
	{
	  if (saw_speclang)
	    {
	      saw_speclang = 0;
	      continue;
	    }

	  if (library == 0)
	    library = 1;
	}
    }

  if (quote)
    fatal ("argument to '%s' missing\n", quote);

  /* There's no point adding -shared-libgcc if we don't have a shared
     libgcc.  */
#ifndef ENABLE_SHARED_LIBGCC
  shared_libgcc = 0;
#endif

  /* Make sure to have room for the trailing NULL argument.  */
  num_args = argc + need_math + shared_libgcc + (library > 0) * 5 + 5;
  arglist = XNEWVEC (const char *, num_args);

  i = 0;
  j = 0;

  /* Copy the 0th argument, i.e., the name of the program itself.  */
  arglist[i++] = argv[j++];

#ifdef TARGET_SSE2
  /* This is a major hack.  On the x86, we want to use SSE floating
     point, because it gives more consistent results.  So we insert
     arguments to select it.  Doing this means that anybody who wants
     to run on a Pentium 3 or earlier will need to use explicit -march
     and -mfpmath options.  */
  arglist[j++] = "-mfpmath=sse";
  arglist[j++] = "-msse2";
#if !defined(TARGET_64BIT_DEFAULT) || !TARGET_64BIT_DEFAULT
  arglist[j++] = "-march=pentium4";
#endif
#endif /* defined(TARGET_SSE2) */

  /* We always combine all input files.  */
  arglist[j++] = "-combine";

  /* If we are going to link in libgo, force __go_register_types to be
     pulled in.  This will let the runtime support code find the type
     descriptors.  */
  if (library > 0)
    arglist[j++] = "-Wl,-u,__go_register_types";

  /* NOTE: We start at 1 now, not 0.  */
  while (i < argc)
    {
      arglist[j] = argv[i];

      /* Make sure -lgo is before the math library, since libgo itself
	 uses those math routines.  */
      if (!saw_math && (args[i] & MATHLIB) && library > 0)
	{
	  --j;
	  saw_math = argv[i];
	}

      if (!saw_libc && (args[i] & WITHLIBC) && library > 0)
	{
	  --j;
	  saw_libc = argv[i];
	}

      if ((args[i] & SKIPOPT) != 0)
	--j;

      i++;
      j++;
    }

  /* Add `-lgo' if we haven't already done so.  */
  if (library > 0)
    {
      arglist[j] = LIBGOBEGIN;
      if (arglist[j][0] != '-' || arglist[j][1] == 'l')
	added_libraries++;
      j++;
      if (library > 1 && !static_link)
	{
	  arglist[j] = "-Wl,-Bstatic";
	  j++;
	}
      arglist[j] = saw_profile_flag ? LIBGO_PROFILE : LIBGO;
      if (arglist[j][0] != '-' || arglist[j][1] == 'l')
	added_libraries++;
      j++;
      if (library > 1 && !static_link)
	{
	  arglist[j] = "-Wl,-Bdynamic";
	  j++;
	}
    }
  if (saw_math)
    arglist[j++] = saw_math;
  else if (library > 0 && need_math)
    {
      arglist[j] = saw_profile_flag ? MATH_LIBRARY_PROFILE : MATH_LIBRARY;
      if (arglist[j][0] != '-' || arglist[j][1] == 'l')
	added_libraries++;
      j++;
    }
  if (saw_libc)
    arglist[j++] = saw_libc;
  if (shared_libgcc && !static_link)
    arglist[j++] = "-shared-libgcc";

  arglist[j] = NULL;

  *in_argc = j;
  *in_argv = arglist;
  *in_added_libraries = added_libraries;
}

/* Called before linking.  Returns 0 on success and -1 on failure.  */
int lang_specific_pre_link (void)  /* Not used for Go.  */
{
  return 0;
}

/* Number of extra output files that lang_specific_pre_link may generate.  */
int lang_specific_extra_outfiles = 0;  /* Not used for Go.  */
