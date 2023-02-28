/* Definitions for PA_RISC with ELF format
   Copyright (C) 1999-2023 Free Software Foundation, Inc.

This file is part of GCC.

GCC is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 3, or (at your option)
any later version.

GCC is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with GCC; see the file COPYING3.  If not see
<http://www.gnu.org/licenses/>.  */


#undef TARGET_OS_CPP_BUILTINS
#define TARGET_OS_CPP_BUILTINS()		\
  do						\
    {						\
	NETBSD_OS_CPP_BUILTINS_ELF();	\
	builtin_assert ("machine=bigendian");	\
    }						\
  while (0)

#undef CPP_SPEC
#define CPP_SPEC NETBSD_CPP_SPEC

#undef ASM_SPEC
#define ASM_SPEC \
  "%{v:-V} %{n} %{T} %{Ym,*} %{Yd,*} %{Wa,*:%*}"

#undef EXTRA_SPECS
#define EXTRA_SPECS NETBSD_SUBTARGET_EXTRA_SPECS
#undef SUBTARGET_EXTRA_SPECS

#define NETBSD_ENTRY_POINT "__start"

#undef LINK_SPEC
#define LINK_SPEC NETBSD_LINK_SPEC_ELF

/* NetBSD profiling functions don't need gcc to allocate counters.  */
#define NO_DEFERRED_PROFILE_COUNTERS 1

/* Define the strings used for the special svr4 .type and .size directives.
   These strings generally do not vary from one system running svr4 to
   another, but if a given system (e.g. m88k running svr) needs to use
   different pseudo-op names for these, they may be overridden in the
   file which includes this one.  */

#undef STRING_ASM_OP
#define STRING_ASM_OP   "\t.stringz\t"

#define TEXT_SECTION_ASM_OP "\t.text"
#define DATA_SECTION_ASM_OP "\t.data"
#define BSS_SECTION_ASM_OP "\t.section\t.bss"

#define TARGET_ASM_FILE_START pa_linux_file_start

/* We want local labels to start with period if made with asm_fprintf.  */
#undef LOCAL_LABEL_PREFIX
#define LOCAL_LABEL_PREFIX "."

/* Define these to generate the Linux/ELF/SysV style of internal
   labels all the time - i.e. to be compatible with
   ASM_GENERATE_INTERNAL_LABEL in <elfos.h>.  Compare these with the
   ones in pa.h and note the lack of dollar signs in these.  FIXME:
   shouldn't we fix pa.h to use ASM_GENERATE_INTERNAL_LABEL instead? */

#undef ASM_OUTPUT_ADDR_VEC_ELT
#define ASM_OUTPUT_ADDR_VEC_ELT(FILE, VALUE) \
  fprintf (FILE, "\t.word .L%d\n", VALUE)

#undef ASM_OUTPUT_ADDR_DIFF_ELT
#define ASM_OUTPUT_ADDR_DIFF_ELT(FILE, BODY, VALUE, REL) \
  fprintf (FILE, "\t.word .L%d-.L%d\n", VALUE, REL)

/* Use the default.  */
#undef ASM_OUTPUT_LABEL

/* NOTE: (*targetm.asm_out.internal_label)() is defined for us by elfos.h, and
   does what we want (i.e. uses colons).  It must be compatible with
   ASM_GENERATE_INTERNAL_LABEL(), so do not define it here.  */

/* Use the default.  */
#undef ASM_OUTPUT_INTERNAL_LABEL

/* Use the default.  */
#undef TARGET_ASM_GLOBALIZE_LABEL
/* Globalizing directive for a label.  */
#define GLOBAL_ASM_OP ".globl "

/* FIXME: Hacked from the <elfos.h> one so that we avoid multiple
   labels in a function declaration (since pa.cc seems determined to do
   it differently)  */

#undef ASM_DECLARE_FUNCTION_NAME
#define ASM_DECLARE_FUNCTION_NAME(FILE, NAME, DECL)		\
  do								\
    {								\
      ASM_OUTPUT_TYPE_DIRECTIVE (FILE, NAME, "function");	\
      ASM_DECLARE_RESULT (FILE, DECL_RESULT (DECL));		\
    }								\
  while (0)

/* As well as globalizing the label, we need to encode the label
   to ensure a plabel is generated in an indirect call.  */

#undef ASM_OUTPUT_EXTERNAL_LIBCALL
#define ASM_OUTPUT_EXTERNAL_LIBCALL(FILE, FUN)  		\
  do								\
    {								\
      if (!FUNCTION_NAME_P (XSTR (FUN, 0)))			\
	pa_encode_label (FUN);					\
      (*targetm.asm_out.globalize_label) (FILE, XSTR (FUN, 0));	\
    }								\
  while (0)

/* NetBSD always uses gas.  */
#undef TARGET_GAS
#define TARGET_GAS 1

/* Use long int for these type to make hppa64 compatibility easier.  */
#undef SIZE_TYPE
#define SIZE_TYPE "long unsigned int"

#undef PTRDIFF_TYPE
#define PTRDIFF_TYPE "long int"

#if 0
#undef TARGET_SYNC_LIBCALL
#define TARGET_SYNC_LIBCALL 1
#endif
