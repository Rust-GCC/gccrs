/* Output VMS debug format symbol table information from GCC.
   Copyright (C) 1987-2023 Free Software Foundation, Inc.
   Contributed by Douglas B. Rupp (rupp@gnat.com).
   Updated by Bernard W. Giroud (bgiroud@users.sourceforge.net).

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

#ifdef VMS_DEBUGGING_INFO
#include "alias.h"
#include "tree.h"
#include "varasm.h"
#include "version.h"
#include "flags.h"
#include "rtl.h"
#include "output.h"
#include "vmsdbg.h"
#include "debug.h"
#include "langhooks.h"
#include "function.h"
#include "target.h"
#include "file-prefix-map.h" /* remap_debug_filename()  */

/* Difference in seconds between the VMS Epoch and the Unix Epoch */
static const long long vms_epoch_offset = 3506716800ll;

int vms_file_stats_name (const char *, long long *, long *, char *, int *);

/* NOTE: In the comments in this file, many references are made to "Debug
   Symbol Table".  This term is abbreviated as `DST' throughout the remainder
   of this file.  */

typedef struct dst_line_info_struct *dst_line_info_ref;

/* Each entry in the line_info_table maintains the file and
   line number associated with the label generated for that
   entry.  The label gives the PC value associated with
   the line number entry.  */
typedef struct dst_line_info_struct
{
  unsigned long dst_file_num;
  unsigned long dst_line_num;
}
dst_line_info_entry;

typedef struct dst_file_info_struct *dst_file_info_ref;

typedef struct dst_file_info_struct
{
  char *file_name;
  unsigned int max_line;
  unsigned int listing_line_start;
  long long cdt;
  long ebk;
  short ffb;
  char rfo;
}
dst_file_info_entry;

/* Maximum size (in bytes) of an artificially generated label.  */
#define MAX_ARTIFICIAL_LABEL_BYTES	30

/* Make sure we know the sizes of the various types debug can describe. These
   are only defaults.  If the sizes are different for your target, you should
   override these values by defining the appropriate symbols in your tm.h
   file.  */
#ifndef PTR_SIZE
#define PTR_SIZE 4 /* Must be 32 bits for VMS debug info */
#endif

/* Pointer to a structure of filenames referenced by this compilation unit.  */
static dst_file_info_ref file_info_table;

/* Total number of entries in the table (i.e. array) pointed to by
   `file_info_table'.  This is the *total* and includes both used and unused
   slots.  */
static unsigned int file_info_table_allocated;

/* Number of entries in the file_info_table which are actually in use.  */
static unsigned int file_info_table_in_use;

/* Size (in elements) of increments by which we may expand the filename
   table.  */
#define FILE_TABLE_INCREMENT 64

typedef char *char_p;

static vec<char_p> funcnam_table;
static vec<unsigned> funcnum_table;
#define FUNC_TABLE_INITIAL 256

/* Local pointer to the name of the main input file.  Initialized in
   vmsdbgout_init.  */
static const char *primary_filename;

static char *module_producer;
static unsigned int module_language;

/* A pointer to the base of a table that contains line information
   for each source code line in .text in the compilation unit.  */
static dst_line_info_ref line_info_table;

/* Number of elements currently allocated for line_info_table.  */
static unsigned int line_info_table_allocated;

/* Number of elements in line_info_table currently in use.  */
static unsigned int line_info_table_in_use;

/* Size (in elements) of increments by which we may expand line_info_table.  */
#define LINE_INFO_TABLE_INCREMENT 1024

/* Forward declarations for functions defined in this file.  */
static char *full_name (const char *);
static unsigned int lookup_filename (const char *);
static int write_debug_header (DST_HEADER *, const char *, int);
static int write_debug_addr (const char *, const char *, int);
static int write_debug_data1 (unsigned int, const char *, int);
static int write_debug_data2 (unsigned int, const char *, int);
static int write_debug_data4 (unsigned long, const char *, int);
static int write_debug_data8 (unsigned long long, const char *, int);
static int write_debug_delta4 (const char *, const char *, const char *, int);
static int write_debug_string (const char *, const char *, int);
static int write_modbeg (int);
static int write_modend (int);
static int write_rtnbeg (int, int);
static int write_rtnend (int, int);
static int write_pclines (int);
static int write_srccorr (int, dst_file_info_entry, int);
static int write_srccorrs (int);

static void vmsdbgout_init (const char *);
static void vmsdbgout_finish (const char *);
static void vmsdbgout_early_finish (const char *);
static void vmsdbgout_assembly_start (void);
static void vmsdbgout_define (unsigned int, const char *);
static void vmsdbgout_undef (unsigned int, const char *);
static void vmsdbgout_start_source_file (unsigned int, const char *);
static void vmsdbgout_end_source_file (unsigned int);
static void vmsdbgout_begin_block (unsigned int, unsigned int);
static void vmsdbgout_end_block (unsigned int, unsigned int);
static bool vmsdbgout_ignore_block (const_tree);
static void vmsdbgout_source_line (unsigned int, unsigned int, const char *,
				   int, bool);
static void vmsdbgout_write_source_line (unsigned, const char *, int , bool);
static void vmsdbgout_begin_prologue (unsigned int, unsigned int,
				      const char *);
static void vmsdbgout_end_prologue (unsigned int, const char *);
static void vmsdbgout_end_function (unsigned int);
static void vmsdbgout_begin_epilogue (unsigned int, const char *);
static void vmsdbgout_end_epilogue (unsigned int, const char *);
static void vmsdbgout_begin_function (tree);
static void vmsdbgout_function_decl (tree);
static void vmsdbgout_early_global_decl (tree);
static void vmsdbgout_late_global_decl (tree);
static void vmsdbgout_type_decl (tree, int);
static void vmsdbgout_abstract_function (tree);

/* The debug hooks structure.  */

const struct gcc_debug_hooks vmsdbg_debug_hooks
= {vmsdbgout_init,
   vmsdbgout_finish,
   vmsdbgout_early_finish,
   vmsdbgout_assembly_start,
   vmsdbgout_define,
   vmsdbgout_undef,
   vmsdbgout_start_source_file,
   vmsdbgout_end_source_file,
   vmsdbgout_begin_block,
   vmsdbgout_end_block,
   vmsdbgout_ignore_block,
   vmsdbgout_source_line,
   debug_nothing_int_int_charstar, /* set_ignored_loc */
   vmsdbgout_begin_prologue,
   vmsdbgout_end_prologue,
   vmsdbgout_begin_epilogue,
   vmsdbgout_end_epilogue,
   vmsdbgout_begin_function,
   vmsdbgout_end_function,
   debug_nothing_tree,		  /* register_main_translation_unit */
   vmsdbgout_function_decl,
   vmsdbgout_early_global_decl,
   vmsdbgout_late_global_decl,
   vmsdbgout_type_decl,		  /* type_decl */
   debug_nothing_tree_tree_tree_bool_bool, /* imported_module_or_decl */
   debug_false_tree_charstarstar_uhwistar, /* die_ref_for_decl */
   debug_nothing_tree_charstar_uhwi, /* register_external_die */
   debug_nothing_tree,		  /* deferred_inline_function */
   vmsdbgout_abstract_function,
   debug_nothing_rtx_code_label,  /* label */
   debug_nothing_int,		  /* handle_pch */
   debug_nothing_rtx_insn,	  /* var_location */
   debug_nothing_tree,	          /* inline_entry */
   debug_nothing_tree,		  /* size_function */
   debug_nothing_void,            /* switch_text_section */
   debug_nothing_tree_tree,	  /* set_name */
   0,                             /* start_end_main_source_file */
   TYPE_SYMTAB_IS_ADDRESS         /* tree_type_symtab_field */
};

/* Definitions of defaults for assembler-dependent names of various
   pseudo-ops and section names.  */
#define VMS_UNALIGNED_SHORT_ASM_OP	".word"
#define VMS_UNALIGNED_INT_ASM_OP	".long"
#define VMS_UNALIGNED_LONG_ASM_OP	".long"
#define VMS_UNALIGNED_DOUBLE_INT_ASM_OP	".quad"

#define VMS_ASM_BYTE_OP	".byte"

#define NUMBYTES(I) ((I) < 256 ? 1 : (I) < 65536 ? 2 : 4)

#define NUMBYTES0(I) ((I) < 128 ? 0 : (I) < 65536 ? 2 : 4)

#ifndef UNALIGNED_PTR_ASM_OP
#define UNALIGNED_PTR_ASM_OP \
  (PTR_SIZE == 8 ? VMS_UNALIGNED_DOUBLE_INT_ASM_OP : VMS_UNALIGNED_INT_ASM_OP)
#endif

#ifndef UNALIGNED_OFFSET_ASM_OP
#define UNALIGNED_OFFSET_ASM_OP(OFFSET) \
  (NUMBYTES (OFFSET) == 4 \
   ? VMS_UNALIGNED_LONG_ASM_OP \
   : (NUMBYTES (OFFSET) == 2 ? VMS_UNALIGNED_SHORT_ASM_OP : VMS_ASM_BYTE_OP))
#endif

/* Definitions of defaults for formats and names of various special
   (artificial) labels which may be generated within this file (when the -g
   options is used and VMS_DEBUGGING_INFO is in effect.  If necessary, these
   may be overridden from within the tm.h file, but typically, overriding these
   defaults is unnecessary.  */

static char text_end_label[MAX_ARTIFICIAL_LABEL_BYTES];

#ifndef TEXT_END_LABEL
#define TEXT_END_LABEL		"Lvetext"
#endif
#ifndef FUNC_BEGIN_LABEL
#define FUNC_BEGIN_LABEL	"LVFB"
#endif
#ifndef FUNC_PROLOG_LABEL
#define FUNC_PROLOG_LABEL	"LVFP"
#endif
#ifndef FUNC_EPILOG_LABEL
#define FUNC_EPILOG_LABEL	"LVEB"
#endif
#ifndef FUNC_END_LABEL
#define FUNC_END_LABEL		"LVFE"
#endif
#ifndef BLOCK_BEGIN_LABEL
#define BLOCK_BEGIN_LABEL	"LVBB"
#endif
#ifndef BLOCK_END_LABEL
#define BLOCK_END_LABEL		"LVBE"
#endif
#ifndef LINE_CODE_LABEL
#define LINE_CODE_LABEL		"LVM"
#endif

#ifndef ASM_OUTPUT_DEBUG_DELTA2
#define ASM_OUTPUT_DEBUG_DELTA2(FILE,LABEL1,LABEL2)			 \
  do									 \
    {									 \
      fprintf ((FILE), "\t%s\t", VMS_UNALIGNED_SHORT_ASM_OP);		 \
      assemble_name (FILE, LABEL1);					 \
      fprintf (FILE, "-");						 \
      assemble_name (FILE, LABEL2);					 \
    }									 \
  while (0)
#endif

#ifndef ASM_OUTPUT_DEBUG_DELTA4
#define ASM_OUTPUT_DEBUG_DELTA4(FILE,LABEL1,LABEL2)			 \
  do									 \
    {									 \
      fprintf ((FILE), "\t%s\t", VMS_UNALIGNED_INT_ASM_OP);		 \
      assemble_name (FILE, LABEL1);					 \
      fprintf (FILE, "-");						 \
      assemble_name (FILE, LABEL2);					 \
    }									 \
  while (0)
#endif

#ifndef ASM_OUTPUT_DEBUG_ADDR_DELTA
#define ASM_OUTPUT_DEBUG_ADDR_DELTA(FILE,LABEL1,LABEL2)			 \
  do									 \
    {									 \
      fprintf ((FILE), "\t%s\t", UNALIGNED_PTR_ASM_OP);			 \
      assemble_name (FILE, LABEL1);					 \
      fprintf (FILE, "-");						 \
      assemble_name (FILE, LABEL2);					 \
    }									 \
  while (0)
#endif

#ifndef ASM_OUTPUT_DEBUG_ADDR
#define ASM_OUTPUT_DEBUG_ADDR(FILE,LABEL)				 \
  do									 \
    {									 \
      fprintf ((FILE), "\t%s\t", UNALIGNED_PTR_ASM_OP);			 \
      assemble_name (FILE, LABEL);					 \
    }									 \
  while (0)
#endif

#ifndef ASM_OUTPUT_DEBUG_ADDR_CONST
#define ASM_OUTPUT_DEBUG_ADDR_CONST(FILE,ADDR)				\
  fprintf ((FILE), "\t%s\t%s", UNALIGNED_PTR_ASM_OP, (ADDR))
#endif

#ifndef ASM_OUTPUT_DEBUG_DATA1
#define ASM_OUTPUT_DEBUG_DATA1(FILE,VALUE) \
  fprintf ((FILE), "\t%s\t%#x", VMS_ASM_BYTE_OP, (unsigned char) VALUE)
#endif

#ifndef ASM_OUTPUT_DEBUG_DATA2
#define ASM_OUTPUT_DEBUG_DATA2(FILE,VALUE) \
  fprintf ((FILE), "\t%s\t%#x", VMS_UNALIGNED_SHORT_ASM_OP, \
	   (unsigned short) VALUE)
#endif

#ifndef ASM_OUTPUT_DEBUG_DATA4
#define ASM_OUTPUT_DEBUG_DATA4(FILE,VALUE) \
  fprintf ((FILE), "\t%s\t%#lx", VMS_UNALIGNED_INT_ASM_OP, \
	   (unsigned long) VALUE)
#endif

#ifndef ASM_OUTPUT_DEBUG_DATA
#define ASM_OUTPUT_DEBUG_DATA(FILE,VALUE) \
  fprintf ((FILE), "\t%s\t%#lx", UNALIGNED_OFFSET_ASM_OP (VALUE), VALUE)
#endif

#ifndef ASM_OUTPUT_DEBUG_ADDR_DATA
#define ASM_OUTPUT_DEBUG_ADDR_DATA(FILE,VALUE) \
  fprintf ((FILE), "\t%s\t%#lx", UNALIGNED_PTR_ASM_OP, \
	   (unsigned long) VALUE)
#endif

#ifndef ASM_OUTPUT_DEBUG_DATA8
#define ASM_OUTPUT_DEBUG_DATA8(FILE,VALUE) \
  fprintf ((FILE), "\t%s\t%#llx", VMS_UNALIGNED_DOUBLE_INT_ASM_OP, \
                                 (unsigned long long) VALUE)
#endif

/* This is similar to the default ASM_OUTPUT_ASCII, except that no trailing
   newline is produced.  When flag_verbose_asm is asserted, we add commentary
   at the end of the line, so we must avoid output of a newline here.  */
#ifndef ASM_OUTPUT_DEBUG_STRING
#define ASM_OUTPUT_DEBUG_STRING(FILE,P)		\
  do						\
    {						\
      int slen = strlen (P);			\
      const char *p = (P);			\
      int i;					\
      fprintf (FILE, "\t.ascii \"");		\
      for (i = 0; i < slen; i++)		\
	{					\
	  int c = p[i];				\
	  if (c == '\"' || c == '\\')		\
	    putc ('\\', FILE);			\
	  if (c >= ' ' && c < 0177)		\
	    putc (c, FILE);			\
	  else					\
	    fprintf (FILE, "\\%o", c);		\
	}					\
      fprintf (FILE, "\"");			\
    }						\
  while (0)
#endif

/* Convert a reference to the assembler name of a C-level name.  This
   macro has the same effect as ASM_OUTPUT_LABELREF, but copies to
   a string rather than writing to a file.  */
#ifndef ASM_NAME_TO_STRING
#define ASM_NAME_TO_STRING(STR, NAME)		\
  do						\
    {						\
      if ((NAME)[0] == '*')			\
	strcpy (STR, NAME+1);			\
      else					\
	strcpy (STR, NAME);			\
    }						\
  while (0)
#endif


/* Output the debug header HEADER.  Also output COMMENT if flag_verbose_asm is
   set.  Return the header size.  Just return the size if DOSIZEONLY is
   nonzero.  */

static int
write_debug_header (DST_HEADER *header, const char *comment, int dosizeonly)
{
  if (!dosizeonly)
    {
      ASM_OUTPUT_DEBUG_DATA2 (asm_out_file,
			      header->dst__header_length.dst_w_length);

      if (flag_verbose_asm)
	fprintf (asm_out_file, "\t%s record length", ASM_COMMENT_START);
      fputc ('\n', asm_out_file);

      ASM_OUTPUT_DEBUG_DATA2 (asm_out_file,
			      header->dst__header_type.dst_w_type);

      if (flag_verbose_asm)
	fprintf (asm_out_file, "\t%s record type (%s)", ASM_COMMENT_START,
		 comment);

      fputc ('\n', asm_out_file);
    }

  return 4;
}

/* Output the address of SYMBOL.  Also output COMMENT if flag_verbose_asm is
   set.  Return the address size.  Just return the size if DOSIZEONLY is
   nonzero.  */

static int
write_debug_addr (const char *symbol, const char *comment, int dosizeonly)
{
  if (!dosizeonly)
    {
      ASM_OUTPUT_DEBUG_ADDR (asm_out_file, symbol);
      if (flag_verbose_asm)
	fprintf (asm_out_file, "\t%s %s", ASM_COMMENT_START, comment);
      fputc ('\n', asm_out_file);
    }

  return PTR_SIZE;
}

/* Output the single byte DATA1.  Also output COMMENT if flag_verbose_asm is
   set.  Return the data size.  Just return the size if DOSIZEONLY is
   nonzero.  */

static int
write_debug_data1 (unsigned int data1, const char *comment, int dosizeonly)
{
  if (!dosizeonly)
    {
      ASM_OUTPUT_DEBUG_DATA1 (asm_out_file, data1);
      if (flag_verbose_asm)
	fprintf (asm_out_file, "\t%s %s", ASM_COMMENT_START, comment);
      fputc ('\n', asm_out_file);
    }

  return 1;
}

/* Output the single word DATA2.  Also output COMMENT if flag_verbose_asm is
   set.  Return the data size.  Just return the size if DOSIZEONLY is
   nonzero.  */

static int
write_debug_data2 (unsigned int data2, const char *comment, int dosizeonly)
{
  if (!dosizeonly)
    {
      ASM_OUTPUT_DEBUG_DATA2 (asm_out_file, data2);
      if (flag_verbose_asm)
	fprintf (asm_out_file, "\t%s %s", ASM_COMMENT_START, comment);
      fputc ('\n', asm_out_file);
    }

  return 2;
}

/* Output double word DATA4.  Also output COMMENT if flag_verbose_asm is set.
   Return the data size.  Just return the size if DOSIZEONLY is nonzero.  */

static int
write_debug_data4 (unsigned long data4, const char *comment, int dosizeonly)
{
  if (!dosizeonly)
    {
      ASM_OUTPUT_DEBUG_DATA4 (asm_out_file, data4);
      if (flag_verbose_asm)
	fprintf (asm_out_file, "\t%s %s", ASM_COMMENT_START, comment);
      fputc ('\n', asm_out_file);
    }

  return 4;
}

/* Output quad word DATA8.  Also output COMMENT if flag_verbose_asm is set.
   Return the data size.  Just return the size if DOSIZEONLY is nonzero.  */

static int
write_debug_data8 (unsigned long long data8, const char *comment,
		   int dosizeonly)
{
  if (!dosizeonly)
    {
      ASM_OUTPUT_DEBUG_DATA8 (asm_out_file, data8);
      if (flag_verbose_asm)
	fprintf (asm_out_file, "\t%s %s", ASM_COMMENT_START, comment);
      fputc ('\n', asm_out_file);
    }

  return 8;
}

/* Output the difference between LABEL1 and LABEL2.  Also output COMMENT if
   flag_verbose_asm is set.  Return the data size.  Just return the size if
   DOSIZEONLY is nonzero.  */

static int
write_debug_delta4 (const char *label1, const char *label2,
		    const char *comment, int dosizeonly)
{
  if (!dosizeonly)
    {
      ASM_OUTPUT_DEBUG_DELTA4 (asm_out_file, label1, label2);
      if (flag_verbose_asm)
	fprintf (asm_out_file, "\t%s %s", ASM_COMMENT_START, comment);
      fputc ('\n', asm_out_file);
    }

  return 4;
}

/* Output a character string STRING.  Also write COMMENT if flag_verbose_asm is
   set.  Return the string length.  Just return the length if DOSIZEONLY is
   nonzero.  */

static int
write_debug_string (const char *string, const char *comment, int dosizeonly)
{
  if (!dosizeonly)
    {
      ASM_OUTPUT_DEBUG_STRING (asm_out_file, string);
      if (flag_verbose_asm)
	fprintf (asm_out_file, "\t%s %s", ASM_COMMENT_START, comment);
      fputc ('\n', asm_out_file);
    }

  return strlen (string);
}

/* Output a module begin header and return the header size.  Just return the
   size if DOSIZEONLY is nonzero.  */

static int
write_modbeg (int dosizeonly)
{
  DST_MODULE_BEGIN modbeg;
  DST_MB_TRLR mb_trlr;
  int i;
  char *module_name, *m;
  int modnamelen;
  int prodnamelen;
  int totsize = 0;

  /* Assumes primary filename has Unix syntax file spec.  */
  module_name = xstrdup (lbasename (primary_filename));

  m = strrchr (module_name, '.');
  if (m)
    *m = 0;

  modnamelen = strlen (module_name);
  for (i = 0; i < modnamelen; i++)
    module_name[i] = TOUPPER (module_name[i]);

  prodnamelen = strlen (module_producer);

  modbeg.dst_a_modbeg_header.dst__header_length.dst_w_length
    = DST_K_MODBEG_SIZE + modnamelen + DST_K_MB_TRLR_SIZE + prodnamelen - 1;
  modbeg.dst_a_modbeg_header.dst__header_type.dst_w_type = DST_K_MODBEG;
  modbeg.dst_b_modbeg_flags.dst_v_modbeg_hide = 0;
  modbeg.dst_b_modbeg_flags.dst_v_modbeg_version = 1;
  modbeg.dst_b_modbeg_flags.dst_v_modbeg_unused = 0;
  modbeg.dst_b_modbeg_unused = 0;
  modbeg.dst_l_modbeg_language = (DST_LANGUAGE) module_language;
  modbeg.dst_w_version_major = DST_K_VERSION_MAJOR;
  modbeg.dst_w_version_minor = DST_K_VERSION_MINOR;
  modbeg.dst_b_modbeg_name = strlen (module_name);

  mb_trlr.dst_b_compiler = strlen (module_producer);

  totsize += write_debug_header (&modbeg.dst_a_modbeg_header,
				 "modbeg", dosizeonly);
  totsize += write_debug_data1 (*((char *) &modbeg.dst_b_modbeg_flags),
				"flags", dosizeonly);
  totsize += write_debug_data1 (modbeg.dst_b_modbeg_unused,
				"unused", dosizeonly);
  totsize += write_debug_data4 (modbeg.dst_l_modbeg_language,
				"language", dosizeonly);
  totsize += write_debug_data2 (modbeg.dst_w_version_major,
				"DST major version", dosizeonly);
  totsize += write_debug_data2 (modbeg.dst_w_version_minor,
				"DST minor version", dosizeonly);
  totsize += write_debug_data1 (modbeg.dst_b_modbeg_name,
				"length of module name", dosizeonly);
  totsize += write_debug_string (module_name, "module name", dosizeonly);
  totsize += write_debug_data1 (mb_trlr.dst_b_compiler,
				"length of compiler name", dosizeonly);
  totsize += write_debug_string (module_producer, "compiler name", dosizeonly);

  return totsize;
}

/* Output a module end trailer and return the trailer size.   Just return
   the size if DOSIZEONLY is nonzero.  */

static int
write_modend (int dosizeonly)
{
  DST_MODULE_END modend;
  int totsize = 0;

  modend.dst_a_modend_header.dst__header_length.dst_w_length
   = DST_K_MODEND_SIZE - 1;
  modend.dst_a_modend_header.dst__header_type.dst_w_type = DST_K_MODEND;

  totsize += write_debug_header (&modend.dst_a_modend_header, "modend",
				 dosizeonly);

  return totsize;
}

/* Output a routine begin header routine RTNNUM and return the header size.
   Just return the size if DOSIZEONLY is nonzero.  */

static int
write_rtnbeg (int rtnnum, int dosizeonly)
{
  const char *rtnname;
  int rtnnamelen;
  char *rtnentryname;
  int totsize = 0;
  char label[MAX_ARTIFICIAL_LABEL_BYTES];
  DST_ROUTINE_BEGIN rtnbeg;
  DST_PROLOG prolog;

  rtnname = funcnam_table[rtnnum];
  rtnnamelen = strlen (rtnname);
  rtnentryname = concat (rtnname, "..en", NULL);

  if (!strcmp (rtnname, "main"))
    {
      DST_HEADER header;
      const char *go = "TRANSFER$BREAK$GO";

      /* This command isn't documented in DSTRECORDS, so it's made to
	 look like what DEC C does */

      /* header size - 1st byte + flag byte + STO_LW size
	 + string count byte + string length */
      header.dst__header_length.dst_w_length
	= DST_K_DST_HEADER_SIZE - 1 + 1 + 4 + 1 + strlen (go);
      header.dst__header_type.dst_w_type = DST_K_TBG;

      totsize += write_debug_header (&header, "transfer", dosizeonly);

      /* I think this is a flag byte, but I don't know what this flag means */
      totsize += write_debug_data1 (0x1, "flags ???", dosizeonly);

      /* Routine Begin PD Address */
      totsize += write_debug_addr (rtnname, "main procedure descriptor",
				   dosizeonly);
      totsize += write_debug_data1 (strlen (go), "length of main_name",
				    dosizeonly);
      totsize += write_debug_string (go, "main name", dosizeonly);
    }

  /* The header length never includes the length byte.  */
  rtnbeg.dst_a_rtnbeg_header.dst__header_length.dst_w_length
   = DST_K_RTNBEG_SIZE + rtnnamelen - 1;
  rtnbeg.dst_a_rtnbeg_header.dst__header_type.dst_w_type = DST_K_RTNBEG;
  rtnbeg.dst_b_rtnbeg_flags.dst_v_rtnbeg_unused = 0;
  rtnbeg.dst_b_rtnbeg_flags.dst_v_rtnbeg_unalloc = 0;
  rtnbeg.dst_b_rtnbeg_flags.dst_v_rtnbeg_prototype = 0;
  rtnbeg.dst_b_rtnbeg_flags.dst_v_rtnbeg_inlined = 0;
  rtnbeg.dst_b_rtnbeg_flags.dst_v_rtnbeg_no_call = 1;
  rtnbeg.dst_b_rtnbeg_name = rtnnamelen;

  totsize += write_debug_header (&rtnbeg.dst_a_rtnbeg_header, "rtnbeg",
				 dosizeonly);
  totsize += write_debug_data1 (*((char *) &rtnbeg.dst_b_rtnbeg_flags),
				"flags", dosizeonly);

  /* Routine Begin Address */
  totsize += write_debug_addr (rtnentryname, "routine entry name", dosizeonly);

  /* Routine Begin PD Address */
  totsize += write_debug_addr (rtnname, "routine procedure descriptor",
			       dosizeonly);

  /* Routine Begin Name */
  totsize += write_debug_data1 (rtnbeg.dst_b_rtnbeg_name,
				"length of routine name", dosizeonly);

  totsize += write_debug_string (rtnname, "routine name", dosizeonly);

  free (rtnentryname);

  if (debug_info_level > DINFO_LEVEL_TERSE)
    {
      prolog.dst_a_prolog_header.dst__header_length.dst_w_length
	= DST_K_PROLOG_SIZE - 1;
      prolog.dst_a_prolog_header.dst__header_type.dst_w_type = DST_K_PROLOG;

      totsize += write_debug_header (&prolog.dst_a_prolog_header, "prolog",
				     dosizeonly);

      ASM_GENERATE_INTERNAL_LABEL
        (label, FUNC_PROLOG_LABEL,
	 funcnum_table[rtnnum]);
      totsize += write_debug_addr (label, "prolog breakpoint addr",
				   dosizeonly);
    }

  return totsize;
}

/* Output a routine end trailer for routine RTNNUM and return the header size.
   Just return the size if DOSIZEONLY is nonzero.  */

static int
write_rtnend (int rtnnum, int dosizeonly)
{
  DST_ROUTINE_END rtnend;
  char label1[MAX_ARTIFICIAL_LABEL_BYTES];
  char label2[MAX_ARTIFICIAL_LABEL_BYTES];
  int totsize;

  totsize = 0;

  rtnend.dst_a_rtnend_header.dst__header_length.dst_w_length
   = DST_K_RTNEND_SIZE - 1;
  rtnend.dst_a_rtnend_header.dst__header_type.dst_w_type = DST_K_RTNEND;
  rtnend.dst_b_rtnend_unused = 0;
  rtnend.dst_l_rtnend_size = 0; /* Calculated below.  */

  totsize += write_debug_header (&rtnend.dst_a_rtnend_header, "rtnend",
				 dosizeonly);
  totsize += write_debug_data1 (rtnend.dst_b_rtnend_unused, "unused",
				dosizeonly);

  ASM_GENERATE_INTERNAL_LABEL
   (label1, FUNC_BEGIN_LABEL,
    funcnum_table[rtnnum]);
  ASM_GENERATE_INTERNAL_LABEL
   (label2, FUNC_END_LABEL,
    funcnum_table[rtnnum]);
  totsize += write_debug_delta4 (label2, label1, "routine size", dosizeonly);

  return totsize;
}

#define K_DELTA_PC(I) \
 ((I) < 128 ? -(I) : (I) < 65536 ? DST_K_DELTA_PC_W : DST_K_DELTA_PC_L)

#define K_SET_LINUM(I) \
 ((I) < 256 ? DST_K_SET_LINUM_B \
  : (I) < 65536 ? DST_K_SET_LINUM : DST_K_SET_LINUM_L)

#define K_INCR_LINUM(I) \
 ((I) < 256 ? DST_K_INCR_LINUM \
  : (I) < 65536 ? DST_K_INCR_LINUM_W : DST_K_INCR_LINUM_L)

/* Output the PC to line number correlations and return the size.  Just return
   the size if DOSIZEONLY is nonzero */

static int
write_pclines (int dosizeonly)
{
  unsigned i;
  int fn;
  int ln, lastln;
  int linestart = 0;
  int max_line;
  DST_LINE_NUM_HEADER line_num;
  DST_PCLINE_COMMANDS pcline;
  char label[MAX_ARTIFICIAL_LABEL_BYTES];
  char lastlabel[MAX_ARTIFICIAL_LABEL_BYTES];
  int totsize = 0;
  char buff[256];

  max_line = file_info_table[1].max_line;
  file_info_table[1].listing_line_start = linestart;
  linestart = linestart + ((max_line / 100000) + 1) * 100000;

  for (i = 2; i < file_info_table_in_use; i++)
    {
      max_line = file_info_table[i].max_line;
      file_info_table[i].listing_line_start = linestart;
      linestart = linestart + ((max_line / 10000) + 1) * 10000;
    }

  /* Set starting address to beginning of text section.  */
  line_num.dst_a_line_num_header.dst__header_length.dst_w_length = 8;
  line_num.dst_a_line_num_header.dst__header_type.dst_w_type = DST_K_LINE_NUM;
  pcline.dst_b_pcline_command = DST_K_SET_ABS_PC;

  totsize += write_debug_header (&line_num.dst_a_line_num_header,
				 "line_num", dosizeonly);
  totsize += write_debug_data1 (pcline.dst_b_pcline_command,
				"line_num (SET ABS PC)", dosizeonly);

  if (dosizeonly)
    totsize += 4;
  else
    {
      ASM_OUTPUT_DEBUG_ADDR (asm_out_file, TEXT_SECTION_ASM_OP);
      if (flag_verbose_asm)
	fprintf (asm_out_file, "\t%s line_num", ASM_COMMENT_START);
      fputc ('\n', asm_out_file);
    }

  fn = line_info_table[1].dst_file_num;
  ln = (file_info_table[fn].listing_line_start
	+ line_info_table[1].dst_line_num);
  line_num.dst_a_line_num_header.dst__header_length.dst_w_length = 4 + 4;
  pcline.dst_b_pcline_command = DST_K_SET_LINUM_L;

  totsize += write_debug_header (&line_num.dst_a_line_num_header,
				 "line_num", dosizeonly);
  totsize += write_debug_data1 (pcline.dst_b_pcline_command,
				"line_num (SET LINUM LONG)", dosizeonly);

  sprintf (buff, "line_num (%d)", ln ? ln - 1 : 0);
  totsize += write_debug_data4 (ln ? ln - 1 : 0, buff, dosizeonly);

  lastln = ln;
  strcpy (lastlabel, TEXT_SECTION_ASM_OP);
  for (i = 1; i < line_info_table_in_use; i++)
    {
      int extrabytes;

      fn = line_info_table[i].dst_file_num;
      ln = (file_info_table[fn].listing_line_start
	    + line_info_table[i].dst_line_num);

      if (ln - lastln > 1)
	extrabytes = 5; /* NUMBYTES (ln - lastln - 1) + 1; */
      else if (ln <= lastln)
	extrabytes = 5; /* NUMBYTES (ln - 1) + 1; */
      else
	extrabytes = 0;

      line_num.dst_a_line_num_header.dst__header_length.dst_w_length
	= 8 + extrabytes;

      totsize += write_debug_header
	(&line_num.dst_a_line_num_header, "line_num", dosizeonly);

      if (ln - lastln > 1)
	{
	  int lndif = ln - lastln - 1;

	  /* K_INCR_LINUM (lndif); */
	  pcline.dst_b_pcline_command = DST_K_INCR_LINUM_L;

	  totsize += write_debug_data1 (pcline.dst_b_pcline_command,
					"line_num (INCR LINUM LONG)",
					dosizeonly);

	  sprintf (buff, "line_num (%d)", lndif);
	  totsize += write_debug_data4 (lndif, buff, dosizeonly);
	}
      else if (ln <= lastln)
	{
	  /* K_SET_LINUM (ln-1); */
	  pcline.dst_b_pcline_command = DST_K_SET_LINUM_L;

	  totsize += write_debug_data1 (pcline.dst_b_pcline_command,
					"line_num (SET LINUM LONG)",
					dosizeonly);

	  sprintf (buff, "line_num (%d)", ln - 1);
	  totsize += write_debug_data4 (ln - 1, buff, dosizeonly);
	}

      pcline.dst_b_pcline_command = DST_K_DELTA_PC_L;

      totsize += write_debug_data1 (pcline.dst_b_pcline_command,
				    "line_num (DELTA PC LONG)", dosizeonly);

      ASM_GENERATE_INTERNAL_LABEL (label, LINE_CODE_LABEL, i);
      totsize += write_debug_delta4 (label, lastlabel, "increment line_num",
				     dosizeonly);

      lastln = ln;
      strcpy (lastlabel, label);
    }

  return totsize;
}

/* Output a source correlation for file FILEID using information saved in
   FILE_INFO_ENTRY and return the size.  Just return the size if DOSIZEONLY is
   nonzero.  */

static int
write_srccorr (int fileid, dst_file_info_entry file_info_entry,
	       int dosizeonly)
{
  int src_command_size;
  int linesleft = file_info_entry.max_line;
  int linestart = file_info_entry.listing_line_start;
  int flen = strlen (file_info_entry.file_name);
  int linestodo = 0;
  DST_SOURCE_CORR src_header;
  DST_SRC_COMMAND src_command;
  DST_SRC_COMMAND src_command_sf;
  DST_SRC_COMMAND src_command_sl;
  DST_SRC_COMMAND src_command_sr;
  DST_SRC_COMMAND src_command_dl;
  DST_SRC_CMDTRLR src_cmdtrlr;
  char buff[256];
  int totsize = 0;

  if (fileid == 1)
    {
      src_header.dst_a_source_corr_header.dst__header_length.dst_w_length
	= DST_K_SOURCE_CORR_HEADER_SIZE + 1 - 1;
      src_header.dst_a_source_corr_header.dst__header_type.dst_w_type
	= DST_K_SOURCE;
      src_command.dst_b_src_command = DST_K_SRC_FORMFEED;

      totsize += write_debug_header (&src_header.dst_a_source_corr_header,
				     "source corr", dosizeonly);

      totsize += write_debug_data1 (src_command.dst_b_src_command,
				    "source_corr (SRC FORMFEED)",
				    dosizeonly);
    }

  src_command_size
    = DST_K_SRC_COMMAND_SIZE + flen + DST_K_SRC_CMDTRLR_SIZE;
  src_command.dst_b_src_command = DST_K_SRC_DECLFILE;
  src_command.dst_a_src_cmd_fields.dst_a_src_decl_src.dst_b_src_df_length
    = src_command_size - 2;
  src_command.dst_a_src_cmd_fields.dst_a_src_decl_src.dst_b_src_df_flags = 0;
  src_command.dst_a_src_cmd_fields.dst_a_src_decl_src.dst_w_src_df_fileid
    = fileid;
  src_command.dst_a_src_cmd_fields.dst_a_src_decl_src.dst_q_src_df_rms_cdt
    = file_info_entry.cdt;
  src_command.dst_a_src_cmd_fields.dst_a_src_decl_src.dst_l_src_df_rms_ebk
    = file_info_entry.ebk;
  src_command.dst_a_src_cmd_fields.dst_a_src_decl_src.dst_w_src_df_rms_ffb
    = file_info_entry.ffb;
  src_command.dst_a_src_cmd_fields.dst_a_src_decl_src.dst_b_src_df_rms_rfo
    = file_info_entry.rfo;
  src_command.dst_a_src_cmd_fields.dst_a_src_decl_src.dst_b_src_df_filename
    = flen;

  src_header.dst_a_source_corr_header.dst__header_length.dst_w_length
    = DST_K_SOURCE_CORR_HEADER_SIZE + src_command_size - 1;
  src_header.dst_a_source_corr_header.dst__header_type.dst_w_type
    = DST_K_SOURCE;

  src_cmdtrlr.dst_b_src_df_libmodname = 0;

  totsize += write_debug_header (&src_header.dst_a_source_corr_header,
				 "source corr", dosizeonly);
  totsize += write_debug_data1 (src_command.dst_b_src_command,
				"source_corr (DECL SRC FILE)", dosizeonly);
  totsize += write_debug_data1
    (src_command.dst_a_src_cmd_fields.dst_a_src_decl_src.dst_b_src_df_length,
     "source_corr (length)", dosizeonly);

  totsize += write_debug_data1
    (src_command.dst_a_src_cmd_fields.dst_a_src_decl_src.dst_b_src_df_flags,
     "source_corr (flags)", dosizeonly);

  totsize += write_debug_data2
    (src_command.dst_a_src_cmd_fields.dst_a_src_decl_src.dst_w_src_df_fileid,
     "source_corr (fileid)", dosizeonly);

  totsize += write_debug_data8
    (src_command.dst_a_src_cmd_fields.dst_a_src_decl_src.dst_q_src_df_rms_cdt,
     "source_corr (creation date)", dosizeonly);

  totsize += write_debug_data4
    (src_command.dst_a_src_cmd_fields.dst_a_src_decl_src.dst_l_src_df_rms_ebk,
     "source_corr (EOF block number)", dosizeonly);

  totsize += write_debug_data2
    (src_command.dst_a_src_cmd_fields.dst_a_src_decl_src.dst_w_src_df_rms_ffb,
     "source_corr (first free byte)", dosizeonly);

  totsize += write_debug_data1
    (src_command.dst_a_src_cmd_fields.dst_a_src_decl_src.dst_b_src_df_rms_rfo,
     "source_corr (record and file organization)", dosizeonly);

  totsize += write_debug_data1
    (src_command.dst_a_src_cmd_fields.dst_a_src_decl_src.dst_b_src_df_filename,
     "source_corr (filename length)", dosizeonly);

  totsize += write_debug_string (remap_debug_filename (
				    file_info_entry.file_name),
				 "source file name", dosizeonly);
  totsize += write_debug_data1 (src_cmdtrlr.dst_b_src_df_libmodname,
				"source_corr (libmodname)", dosizeonly);

  src_command_sf.dst_b_src_command = DST_K_SRC_SETFILE;
  src_command_sf.dst_a_src_cmd_fields.dst_w_src_unsword = fileid;

  src_command_sr.dst_b_src_command = DST_K_SRC_SETREC_W;
  src_command_sr.dst_a_src_cmd_fields.dst_w_src_unsword = 1;

  src_command_sl.dst_b_src_command = DST_K_SRC_SETLNUM_L;
  src_command_sl.dst_a_src_cmd_fields.dst_l_src_unslong = linestart + 1;

  src_command_dl.dst_b_src_command = DST_K_SRC_DEFLINES_W;

  if (linesleft > 65534)
    linesleft = linesleft - 65534, linestodo = 65534;
  else
    linestodo = linesleft, linesleft = 0;

  src_command_dl.dst_a_src_cmd_fields.dst_w_src_unsword = linestodo;

  src_header.dst_a_source_corr_header.dst__header_length.dst_w_length
    = DST_K_SOURCE_CORR_HEADER_SIZE + 3 + 3 + 5 + 3 - 1;
  src_header.dst_a_source_corr_header.dst__header_type.dst_w_type
    = DST_K_SOURCE;

  if (src_command_dl.dst_a_src_cmd_fields.dst_w_src_unsword)
    {
      totsize += write_debug_header (&src_header.dst_a_source_corr_header,
				     "source corr", dosizeonly);

      totsize += write_debug_data1 (src_command_sf.dst_b_src_command,
				    "source_corr (src setfile)", dosizeonly);

      totsize += write_debug_data2
	(src_command_sf.dst_a_src_cmd_fields.dst_w_src_unsword,
	 "source_corr (fileid)", dosizeonly);

      totsize += write_debug_data1 (src_command_sr.dst_b_src_command,
				    "source_corr (setrec)", dosizeonly);

      totsize += write_debug_data2
	(src_command_sr.dst_a_src_cmd_fields.dst_w_src_unsword,
	 "source_corr (recnum)", dosizeonly);

      totsize += write_debug_data1 (src_command_sl.dst_b_src_command,
				    "source_corr (setlnum)", dosizeonly);

      totsize += write_debug_data4
	(src_command_sl.dst_a_src_cmd_fields.dst_l_src_unslong,
	 "source_corr (linenum)", dosizeonly);

      totsize += write_debug_data1 (src_command_dl.dst_b_src_command,
				    "source_corr (deflines)", dosizeonly);

      sprintf (buff, "source_corr (%d)",
	       src_command_dl.dst_a_src_cmd_fields.dst_w_src_unsword);
      totsize += write_debug_data2
	(src_command_dl.dst_a_src_cmd_fields.dst_w_src_unsword,
	 buff, dosizeonly);

      while (linesleft > 0)
	{
	  src_header.dst_a_source_corr_header.dst__header_length.dst_w_length
	    = DST_K_SOURCE_CORR_HEADER_SIZE + 3 - 1;
	  src_header.dst_a_source_corr_header.dst__header_type.dst_w_type
	    = DST_K_SOURCE;
	  src_command_dl.dst_b_src_command = DST_K_SRC_DEFLINES_W;

	  if (linesleft > 65534)
	    linesleft = linesleft - 65534, linestodo = 65534;
	  else
	    linestodo = linesleft, linesleft = 0;

	  src_command_dl.dst_a_src_cmd_fields.dst_w_src_unsword = linestodo;

	  totsize += write_debug_header (&src_header.dst_a_source_corr_header,
					 "source corr", dosizeonly);
	  totsize += write_debug_data1 (src_command_dl.dst_b_src_command,
					"source_corr (deflines)", dosizeonly);
	  sprintf (buff, "source_corr (%d)",
		   src_command_dl.dst_a_src_cmd_fields.dst_w_src_unsword);
	  totsize += write_debug_data2
	    (src_command_dl.dst_a_src_cmd_fields.dst_w_src_unsword,
	     buff, dosizeonly);
	}
    }

  return totsize;
}

/* Output all the source correlation entries and return the size.  Just return
   the size if DOSIZEONLY is nonzero.  */

static int
write_srccorrs (int dosizeonly)
{
  unsigned int i;
  int totsize = 0;

  for (i = 1; i < file_info_table_in_use; i++)
    totsize += write_srccorr (i, file_info_table[i], dosizeonly);

  return totsize;
}

/* Output a marker (i.e. a label) for the beginning of a function, before
   the prologue.  */

static void
vmsdbgout_begin_prologue (unsigned int line, unsigned int column,
			  const char *file)
{
  char label[MAX_ARTIFICIAL_LABEL_BYTES];

  if (write_symbols == VMS_AND_DWARF2_DEBUG)
    (*dwarf2_debug_hooks.begin_prologue) (line, column, file);

  if (debug_info_level > DINFO_LEVEL_NONE)
    {
      ASM_GENERATE_INTERNAL_LABEL (label, FUNC_BEGIN_LABEL,
				   current_function_funcdef_no);
      ASM_OUTPUT_LABEL (asm_out_file, label);
    }
}

/* Output a marker (i.e. a label) for the beginning of a function, after
   the prologue.  */

static void
vmsdbgout_end_prologue (unsigned int line, const char *file)
{
  char label[MAX_ARTIFICIAL_LABEL_BYTES];

  if (write_symbols == VMS_AND_DWARF2_DEBUG)
    (*dwarf2_debug_hooks.end_prologue) (line, file);

  if (debug_info_level > DINFO_LEVEL_TERSE)
    {
      ASM_GENERATE_INTERNAL_LABEL (label, FUNC_PROLOG_LABEL,
				   current_function_funcdef_no);
      ASM_OUTPUT_LABEL (asm_out_file, label);

      /* VMS PCA expects every PC range to correlate to some line and file.  */
      vmsdbgout_write_source_line (line, file, 0, true);
    }
}

/* No output for VMS debug, but make obligatory call to Dwarf2 debug */

static void
vmsdbgout_end_function (unsigned int line)
{
  if (write_symbols == VMS_AND_DWARF2_DEBUG)
    (*dwarf2_debug_hooks.end_function) (line);
}

/* Output a marker (i.e. a label) for the beginning of the epilogue.
   This gets called *before* the epilogue code has been generated.  */

static void
vmsdbgout_begin_epilogue (unsigned int line, const char *file)
{
  char label[MAX_ARTIFICIAL_LABEL_BYTES];
  static int save_current_function_funcdef_no = -1;

  if (write_symbols == VMS_AND_DWARF2_DEBUG)
    (*dwarf2_debug_hooks.begin_epilogue) (line, file);

  if (debug_info_level > DINFO_LEVEL_NONE)
    {
      if (save_current_function_funcdef_no != current_function_funcdef_no)
	{
	  /* Output a label to mark the endpoint of the code generated for this
	     function.  */
	  ASM_GENERATE_INTERNAL_LABEL (label, FUNC_EPILOG_LABEL,
				       current_function_funcdef_no);

	  ASM_OUTPUT_LABEL (asm_out_file, label);

	  save_current_function_funcdef_no = current_function_funcdef_no;

	  /* VMS PCA expects every PC range to correlate to some line and
	     file.  */
	  vmsdbgout_write_source_line (line, file, 0, true);
	}
    }
}

/* Output a marker (i.e. a label) for the absolute end of the generated code
   for a function definition.  This gets called *after* the epilogue code has
   been generated.  */

static void
vmsdbgout_end_epilogue (unsigned int line, const char *file)
{
  char label[MAX_ARTIFICIAL_LABEL_BYTES];

  if (write_symbols == VMS_AND_DWARF2_DEBUG)
    (*dwarf2_debug_hooks.end_epilogue) (line, file);

  if (debug_info_level > DINFO_LEVEL_NONE)
    {
      /* Output a label to mark the endpoint of the code generated for this
         function.  */
      ASM_GENERATE_INTERNAL_LABEL (label, FUNC_END_LABEL,
				   current_function_funcdef_no);
      ASM_OUTPUT_LABEL (asm_out_file, label);

      /* VMS PCA expects every PC range to correlate to some line and file.  */
      vmsdbgout_write_source_line (line, file, 0, true);
    }
}

/* Output a marker (i.e. a label) for the beginning of the generated code for
   a lexical block.  */

static void
vmsdbgout_begin_block (unsigned line, unsigned blocknum)
{
  if (write_symbols == VMS_AND_DWARF2_DEBUG)
    (*dwarf2_debug_hooks.begin_block) (line, blocknum);

  if (debug_info_level > DINFO_LEVEL_TERSE)
    targetm.asm_out.internal_label (asm_out_file, BLOCK_BEGIN_LABEL, blocknum);
}

/* Output a marker (i.e. a label) for the end of the generated code for a
   lexical block.  */

static void
vmsdbgout_end_block (unsigned line, unsigned blocknum)
{
  if (write_symbols == VMS_AND_DWARF2_DEBUG)
    (*dwarf2_debug_hooks.end_block) (line, blocknum);

  if (debug_info_level > DINFO_LEVEL_TERSE)
    targetm.asm_out.internal_label (asm_out_file, BLOCK_END_LABEL, blocknum);
}

/* Not implemented in VMS Debug.  */

static bool
vmsdbgout_ignore_block (const_tree block)
{
  bool retval = 0;

  if (write_symbols == VMS_AND_DWARF2_DEBUG)
    retval = (*dwarf2_debug_hooks.ignore_block) (block);

  return retval;
}

/* Add an entry for function DECL into the funcnam_table.  */

static void
vmsdbgout_begin_function (tree decl)
{
  const char *name = XSTR (XEXP (DECL_RTL (decl), 0), 0);

  if (write_symbols == VMS_AND_DWARF2_DEBUG)
    (*dwarf2_debug_hooks.begin_function) (decl);

  /* Add the new entry to the end of the function name table.  */
  funcnam_table.safe_push (xstrdup (name));
  funcnum_table.safe_push (current_function_funcdef_no);
}

static char fullname_buff [4096];

/* Return the full file specification for FILENAME.  The specification must be
   in VMS syntax in order to be processed by VMS Debug.  */

static char *
full_name (const char *filename)
{
#ifdef VMS
  FILE *fp = fopen (filename, "r");

  fgetname (fp, fullname_buff, 1);
  fclose (fp);
#else
  /* Unix paths really mess up VMS debug. Better to just output the
     base filename.  */
  strcpy (fullname_buff, filename);
#endif

  return fullname_buff;
}

/* Lookup a filename (in the list of filenames that we know about here in
   vmsdbgout.cc) and return its "index".  The index of each (known) filename is
   just a unique number which is associated with only that one filename.  We
   need such numbers for the sake of generating labels  and references
   to those files numbers.  If the filename given as an argument is not
   found in our current list, add it to the list and assign it the next
   available unique index number.  In order to speed up searches, we remember
   the index of the filename was looked up last.  This handles the majority of
   all searches.  */

static unsigned int
lookup_filename (const char *file_name)
{
  static unsigned int last_file_lookup_index = 0;
  char *fn;
  unsigned i;
  const char *fnam;
  long long cdt = 0;
  long ebk = 0;
  short ffb = 0;
  char rfo = 0;
  long siz = 0;
  int ver = 0;

  fnam = full_name (file_name);

  /* Check to see if the file name that was searched on the previous call
     matches this file name. If so, return the index.  */
  if (last_file_lookup_index != 0)
    {
      fn = file_info_table[last_file_lookup_index].file_name;
      if (strcmp (fnam, fn) == 0)
	return last_file_lookup_index;
    }

  /* Didn't match the previous lookup, search the table */
  for (i = 1; i < file_info_table_in_use; ++i)
    {
      fn = file_info_table[i].file_name;
      if (strcmp (fnam, fn) == 0)
	{
	  last_file_lookup_index = i;
	  return i;
	}
    }

  /* Prepare to add a new table entry by making sure there is enough space in
     the table to do so.  If not, expand the current table.  */
  if (file_info_table_in_use == file_info_table_allocated)
    {

      file_info_table_allocated += FILE_TABLE_INCREMENT;
      file_info_table = XRESIZEVEC (dst_file_info_entry, file_info_table,
				    file_info_table_allocated);
    }

  if (vms_file_stats_name (file_name, &cdt, &siz, &rfo, &ver) == 0)
    {
      ebk = siz / 512 + 1;
      ffb = siz - ((siz / 512) * 512);
    }

  /* Add the new entry to the end of the filename table.  */
  file_info_table[file_info_table_in_use].file_name = xstrdup (fnam);
  file_info_table[file_info_table_in_use].max_line = 0;
  file_info_table[file_info_table_in_use].cdt = cdt;
  file_info_table[file_info_table_in_use].ebk = ebk;
  file_info_table[file_info_table_in_use].ffb = ffb;
  file_info_table[file_info_table_in_use].rfo = rfo;

  last_file_lookup_index = file_info_table_in_use++;
  return last_file_lookup_index;
}

/* Output a label to mark the beginning of a source code line entry
   and record information relating to this source line, in
   'line_info_table' for later output of the .debug_line section.  */

static void
vmsdbgout_write_source_line (unsigned line, const char *filename,
                             int /* discriminator */, bool /* is_stmt */)
{
  dst_line_info_ref line_info;

  targetm.asm_out.internal_label (asm_out_file, LINE_CODE_LABEL,
                                  line_info_table_in_use);

  /* Expand the line info table if necessary.  */
  if (line_info_table_in_use == line_info_table_allocated)
    {
      line_info_table_allocated += LINE_INFO_TABLE_INCREMENT;
      line_info_table = XRESIZEVEC (dst_line_info_entry, line_info_table,
                                    line_info_table_allocated);
    }

  /* Add the new entry at the end of the line_info_table.  */
  line_info = &line_info_table[line_info_table_in_use++];
  line_info->dst_file_num = lookup_filename (filename);
  line_info->dst_line_num = line;
  if (line > file_info_table[line_info->dst_file_num].max_line)
    file_info_table[line_info->dst_file_num].max_line = line;
}

static void
vmsdbgout_source_line (unsigned line, unsigned int column,
		       const char *filename,
                       int discriminator, bool is_stmt)
{
  if (write_symbols == VMS_AND_DWARF2_DEBUG)
    (*dwarf2_debug_hooks.source_line) (line, column, filename, discriminator,
				       is_stmt);

  if (debug_info_level >= DINFO_LEVEL_TERSE)
    vmsdbgout_write_source_line (line, filename, discriminator, is_stmt);
}

/* Record the beginning of a new source file, for later output.
   At present, unimplemented.  */

static void
vmsdbgout_start_source_file (unsigned int lineno, const char *filename)
{
  if (write_symbols == VMS_AND_DWARF2_DEBUG)
    (*dwarf2_debug_hooks.start_source_file) (lineno, filename);
}

/* Record the end of a source file, for later output.
   At present, unimplemented.  */

static void
vmsdbgout_end_source_file (unsigned int lineno ATTRIBUTE_UNUSED)
{
  if (write_symbols == VMS_AND_DWARF2_DEBUG)
    (*dwarf2_debug_hooks.end_source_file) (lineno);
}

/* Set up for Debug output at the start of compilation.  */

static void
vmsdbgout_init (const char *filename)
{
  const char *language_string = lang_hooks.name;

  if (write_symbols == VMS_AND_DWARF2_DEBUG)
    (*dwarf2_debug_hooks.init) (filename);

  if (debug_info_level == DINFO_LEVEL_NONE)
    return;

  /* Remember the name of the primary input file.  */
  primary_filename = filename;

  /* Allocate the initial hunk of the file_info_table.  */
  file_info_table = XCNEWVEC (dst_file_info_entry, FILE_TABLE_INCREMENT);
  file_info_table_allocated = FILE_TABLE_INCREMENT;
  /* Skip the first entry - file numbers begin at 1.  */
  file_info_table_in_use = 1;

  funcnam_table.create (FUNC_TABLE_INITIAL);
  funcnum_table.create (FUNC_TABLE_INITIAL);

  /* Allocate the initial hunk of the line_info_table.  */
  line_info_table = XCNEWVEC (dst_line_info_entry, LINE_INFO_TABLE_INCREMENT);
  line_info_table_allocated = LINE_INFO_TABLE_INCREMENT;
  /* zero-th entry is allocated, but unused */
  line_info_table_in_use = 1;

  lookup_filename (primary_filename);

  if (lang_GNU_C ())
    module_language = DST_K_C;
  else if (lang_GNU_CXX ())
    module_language = DST_K_CXX;
  else if (!strcmp (language_string, "GNU Ada"))
    module_language = DST_K_ADA;
  else if (!strcmp (language_string, "GNU F77"))
    module_language = DST_K_FORTRAN;
  else
    module_language = DST_K_UNKNOWN;

  module_producer = concat (language_string, " ", version_string, NULL);

  ASM_GENERATE_INTERNAL_LABEL (text_end_label, TEXT_END_LABEL, 0);

}

/* Not implemented in VMS Debug.  */

static void
vmsdbgout_assembly_start (void)
{
  if (write_symbols == VMS_AND_DWARF2_DEBUG)
    (*dwarf2_debug_hooks.assembly_start) ();
}

/* Not implemented in VMS Debug.  */

static void
vmsdbgout_define (unsigned int lineno, const char *buffer)
{
  if (write_symbols == VMS_AND_DWARF2_DEBUG)
    (*dwarf2_debug_hooks.define) (lineno, buffer);
}

/* Not implemented in VMS Debug.  */

static void
vmsdbgout_undef (unsigned int lineno, const char *buffer)
{
  if (write_symbols == VMS_AND_DWARF2_DEBUG)
    (*dwarf2_debug_hooks.undef) (lineno, buffer);
}

/* Not implemented in VMS Debug.  */

static void
vmsdbgout_function_decl (tree decl)
{
  if (write_symbols == VMS_AND_DWARF2_DEBUG)
    (*dwarf2_debug_hooks.function_decl) (decl);
}

/* Not implemented in VMS Debug.  */

static void
vmsdbgout_early_global_decl (tree decl)
{
  if (write_symbols == VMS_AND_DWARF2_DEBUG)
    (*dwarf2_debug_hooks.early_global_decl) (decl);
}

/* Not implemented in VMS Debug.  */

static void
vmsdbgout_late_global_decl (tree decl)
{
  if (write_symbols == VMS_AND_DWARF2_DEBUG)
    (*dwarf2_debug_hooks.late_global_decl) (decl);
}

/* Not implemented in VMS Debug.  */

static void
vmsdbgout_type_decl (tree decl, int local)
{
  if (write_symbols == VMS_AND_DWARF2_DEBUG)
    (*dwarf2_debug_hooks.type_decl) (decl, local);
}

/* Not implemented in VMS Debug.  */

static void
vmsdbgout_abstract_function (tree decl)
{
  if (write_symbols == VMS_AND_DWARF2_DEBUG)
    (*dwarf2_debug_hooks.outlining_inline_function) (decl);
}

static void
vmsdbgout_early_finish (const char *filename)
{
  if (write_symbols == VMS_AND_DWARF2_DEBUG)
    (*dwarf2_debug_hooks.early_finish) (filename);
}

/* Output stuff that Debug requires at the end of every file and generate the
   VMS Debug debugging info.  */

static void
vmsdbgout_finish (const char *filename ATTRIBUTE_UNUSED)
{
  unsigned int i, ifunc;
  int totsize;

  if (write_symbols == VMS_AND_DWARF2_DEBUG)
    (*dwarf2_debug_hooks.finish) (filename);

  if (debug_info_level == DINFO_LEVEL_NONE)
    return;

  /* Output a terminator label for the .text section.  */
  switch_to_section (text_section);
  targetm.asm_out.internal_label (asm_out_file, TEXT_END_LABEL, 0);

  /* Output debugging information.
     Warning! Do not change the name of the .vmsdebug section without
     changing it in the assembler also.  */
  switch_to_section (get_named_section (NULL, ".vmsdebug", 0));
  ASM_OUTPUT_ALIGN (asm_out_file, 0);

  totsize = write_modbeg (1);
  FOR_EACH_VEC_ELT (funcnum_table, i, ifunc)
    {
      totsize += write_rtnbeg (i, 1);
      totsize += write_rtnend (i, 1);
    }
  totsize += write_pclines (1);

  write_modbeg (0);
  FOR_EACH_VEC_ELT (funcnum_table, i, ifunc)
    {
      write_rtnbeg (i, 0);
      write_rtnend (i, 0);
    }
  write_pclines (0);

  if (debug_info_level > DINFO_LEVEL_TERSE)
    {
      totsize = write_srccorrs (1);
      write_srccorrs (0);
    }

  totsize = write_modend (1);
  write_modend (0);
}

/* Need for both Dwarf2 on IVMS and VMS Debug on AVMS */

#ifdef VMS
#define __NEW_STARLET 1
#include <vms/rms.h>
#include <vms/atrdef.h>
#include <vms/fibdef.h>
#include <vms/stsdef.h>
#include <vms/iodef.h>
#include <vms/fatdef.h>
#include <vms/descrip.h>
#include <unixlib.h>

#define MAXPATH 256

/* descrip.h doesn't have everything ...  */
typedef struct fibdef* __fibdef_ptr32 __attribute__ (( mode (SI) ));
struct dsc$descriptor_fib
{
  unsigned int fib$l_len;
  __fibdef_ptr32 fib$l_addr;
};

/* I/O Status Block.  */
struct IOSB
{
  unsigned short status, count;
  unsigned int devdep;
};

static char *tryfile;

/* Variable length string.  */
struct vstring
{
  short length;
  char string[NAM$C_MAXRSS+1];
};

static char filename_buff [MAXPATH];
static char vms_filespec [MAXPATH];

/* Callback function for filespec style conversion.  */

static int
translate_unix (char *name, int type ATTRIBUTE_UNUSED)
{
  strncpy (filename_buff, name, MAXPATH);
  filename_buff [MAXPATH - 1] = (char) 0;
  return 0;
}

/* Wrapper for DECC function that converts a Unix filespec
   to VMS style filespec.  */

static char *
to_vms_file_spec (char *filespec)
{
  strncpy (vms_filespec, "", MAXPATH);
  decc$to_vms (filespec, translate_unix, 1, 1);
  strncpy (vms_filespec, filename_buff, MAXPATH);

  vms_filespec [MAXPATH - 1] = (char) 0;

  return vms_filespec;
}

#else
#define VMS_EPOCH_OFFSET 35067168000000000LL
#define VMS_GRANULARITY_FACTOR 10000000
#endif

/* Return VMS file date, size, format, version given a name.  */

int
vms_file_stats_name (const char *filename, long long *cdt, long *siz, char *rfo,
		     int *ver)
{
#ifdef VMS
  struct FAB fab;
  struct NAM nam;

  unsigned long long create;
  FAT recattr;
  char ascnamebuff [256];

  ATRDEF atrlst[]
    = {
      { ATR$S_CREDATE,  ATR$C_CREDATE,  &create },
      { ATR$S_RECATTR,  ATR$C_RECATTR,  &recattr },
      { ATR$S_ASCNAME,  ATR$C_ASCNAME,  &ascnamebuff },
      { 0, 0, 0}
    };

  FIBDEF fib;
  struct dsc$descriptor_fib fibdsc = {sizeof (fib), (void *) &fib};

  struct IOSB iosb;

  long status;
  unsigned short chan;

  struct vstring file;
  struct dsc$descriptor_s filedsc
    = {NAM$C_MAXRSS, DSC$K_DTYPE_T, DSC$K_CLASS_S, (void *) file.string};
  struct vstring device;
  struct dsc$descriptor_s devicedsc
    = {NAM$C_MAXRSS, DSC$K_DTYPE_T, DSC$K_CLASS_S, (void *) device.string};
  struct vstring result;
  struct dsc$descriptor_s resultdsc
    = {NAM$C_MAXRSS, DSC$K_DTYPE_VT, DSC$K_CLASS_VS, (void *) result.string};

  if (strcmp (filename, "<internal>") == 0
      || strcmp (filename, "<built-in>") == 0)
    {
      if (cdt)
	*cdt = 0;

      if (siz)
	*siz = 0;

      if (rfo)
	*rfo = 0;

      if (ver)
        *ver = 0;

      return 0;
    }

  tryfile = to_vms_file_spec (filename);

  /* Allocate and initialize a FAB and NAM structures.  */
  fab = cc$rms_fab;
  nam = cc$rms_nam;

  nam.nam$l_esa = file.string;
  nam.nam$b_ess = NAM$C_MAXRSS;
  nam.nam$l_rsa = result.string;
  nam.nam$b_rss = NAM$C_MAXRSS;
  fab.fab$l_fna = tryfile;
  fab.fab$b_fns = strlen (tryfile);
  fab.fab$l_nam = &nam;

  /* Validate filespec syntax and device existence.  */
  status = SYS$PARSE (&fab, 0, 0);
  if ((status & 1) != 1)
    return 1;

  file.string[nam.nam$b_esl] = 0;

  /* Find matching filespec.  */
  status = SYS$SEARCH (&fab, 0, 0);
  if ((status & 1) != 1)
    return 1;

  file.string[nam.nam$b_esl] = 0;
  result.string[result.length=nam.nam$b_rsl] = 0;

  /* Get the device name and assign an IO channel.  */
  strncpy (device.string, nam.nam$l_dev, nam.nam$b_dev);
  devicedsc.dsc$w_length  = nam.nam$b_dev;
  chan = 0;
  status = SYS$ASSIGN (&devicedsc, &chan, 0, 0, 0);
  if ((status & 1) != 1)
    return 1;

  /* Initialize the FIB and fill in the directory id field.  */
  memset (&fib, 0, sizeof (fib));
  fib.fib$w_did[0]  = nam.nam$w_did[0];
  fib.fib$w_did[1]  = nam.nam$w_did[1];
  fib.fib$w_did[2]  = nam.nam$w_did[2];
  fib.fib$l_acctl = 0;
  fib.fib$l_wcc = 0;
  strcpy (file.string, (strrchr (result.string, ']') + 1));
  filedsc.dsc$w_length = strlen (file.string);
  result.string[result.length = 0] = 0;

  /* Open and close the file to fill in the attributes.  */
  status
    = SYS$QIOW (0, chan, IO$_ACCESS|IO$M_ACCESS, &iosb, 0, 0,
		&fibdsc, &filedsc, &result.length, &resultdsc, &atrlst, 0);
  if ((status & 1) != 1)
    return 1;
  if ((iosb.status & 1) != 1)
    return 1;

  result.string[result.length] = 0;
  status = SYS$QIOW (0, chan, IO$_DEACCESS, &iosb, 0, 0, &fibdsc, 0, 0, 0,
		     &atrlst, 0);
  if ((status & 1) != 1)
    return 1;
  if ((iosb.status & 1) != 1)
    return 1;

  /* Deassign the channel and exit.  */
  status = SYS$DASSGN (chan);
  if ((status & 1) != 1)
    return 1;

  if (cdt) *cdt = create;
  if (siz) *siz = (512 * 65536 * recattr.fat$w_efblkh) +
                  (512 * (recattr.fat$w_efblkl - 1)) +
                  recattr.fat$w_ffbyte;
  if (rfo) *rfo = recattr.fat$v_rtype;
  if (ver) *ver = strtol (strrchr (ascnamebuff, ';')+1, 0, 10);

  return 0;
#else
  struct stat buff;

  if ((stat (filename, &buff)) != 0)
     return 1;

  if (cdt)
    *cdt = (long long) (buff.st_mtime * VMS_GRANULARITY_FACTOR)
                        + VMS_EPOCH_OFFSET;

  if (siz)
    *siz = buff.st_size;

  if (rfo)
    *rfo = 2; /* Stream LF format */

  if (ver)
    *ver = 1;

  return 0;
#endif
}
#endif
