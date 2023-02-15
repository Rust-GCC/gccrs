/* Definitions of target machine for GNU compiler,
   for 64 bit PowerPC linux.
   Copyright (C) 2000-2023 Free Software Foundation, Inc.

   This file is part of GCC.

   GCC is free software; you can redistribute it and/or modify it
   under the terms of the GNU General Public License as published
   by the Free Software Foundation; either version 3, or (at your
   option) any later version.

   GCC is distributed in the hope that it will be useful, but WITHOUT
   ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
   or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public
   License for more details.

   Under Section 7 of GPL version 3, you are granted additional
   permissions described in the GCC Runtime Library Exception, version
   3.1, as published by the Free Software Foundation.

   You should have received a copy of the GNU General Public License and
   a copy of the GCC Runtime Library Exception along with this program;
   see the files COPYING3 and COPYING.RUNTIME respectively.  If not, see
   <http://www.gnu.org/licenses/>.  */

#ifndef RS6000_BI_ARCH

#undef	TARGET_64BIT
#define	TARGET_64BIT 1

#define	DEFAULT_ARCH64_P 1
#define	RS6000_BI_ARCH_P 0

#else

#define	DEFAULT_ARCH64_P (TARGET_DEFAULT & MASK_64BIT)
#define	RS6000_BI_ARCH_P 1

#endif

#ifdef IN_LIBGCC2
#undef TARGET_64BIT
#ifdef __powerpc64__
#define TARGET_64BIT 1
#else
#define TARGET_64BIT 0
#endif
#endif

#undef	TARGET_AIX
#define	TARGET_AIX TARGET_64BIT

#ifdef HAVE_LD_NO_DOT_SYMS
/* New ABI uses a local sym for the function entry point.  */
extern int dot_symbols;
#undef DOT_SYMBOLS
#define DOT_SYMBOLS dot_symbols
#endif

#define TARGET_PROFILE_KERNEL profile_kernel

#undef TARGET_KEEP_LEAF_WHEN_PROFILED
#define TARGET_KEEP_LEAF_WHEN_PROFILED rs6000_keep_leaf_when_profiled

#define TARGET_USES_LINUX64_OPT 1
#ifdef HAVE_LD_LARGE_TOC
#undef TARGET_CMODEL
#define TARGET_CMODEL rs6000_current_cmodel
#define SET_CMODEL(opt) rs6000_current_cmodel = opt
#else
#define SET_CMODEL(opt) do {} while (0)
#endif

#undef  PROCESSOR_DEFAULT
#define PROCESSOR_DEFAULT PROCESSOR_POWER7
#undef  PROCESSOR_DEFAULT64
#define PROCESSOR_DEFAULT64 PROCESSOR_POWER8

/* We don't need to generate entries in .fixup, except when
   -mrelocatable or -mrelocatable-lib is given.  */
#undef RELOCATABLE_NEEDS_FIXUP
#define RELOCATABLE_NEEDS_FIXUP \
  (rs6000_isa_flags & rs6000_isa_flags_explicit & OPTION_MASK_RELOCATABLE)

#undef	RS6000_ABI_NAME
#define	RS6000_ABI_NAME "linux"

#define INVALID_64BIT "%<-m%s%> not supported in this configuration"
#define INVALID_32BIT INVALID_64BIT

#ifdef LINUX64_DEFAULT_ABI_ELFv2
#define ELFv2_ABI_CHECK (rs6000_elf_abi != 1)
#else
#define ELFv2_ABI_CHECK (rs6000_elf_abi == 2)
#endif

#undef	SUBSUBTARGET_OVERRIDE_OPTIONS
#define	SUBSUBTARGET_OVERRIDE_OPTIONS				\
  do rs6000_linux64_override_options (); while (0)

#undef	ASM_SPEC
#undef	LINK_OS_LINUX_SPEC
#undef	LINK_SECURE_PLT_SPEC

#ifndef	RS6000_BI_ARCH
#define	ASM_SPEC	 "%(asm_spec64) %(asm_spec_common)"
#define	LINK_OS_LINUX_SPEC "%(link_os_linux_spec64)"
#define	LINK_SECURE_PLT_SPEC ""
#else
#if DEFAULT_ARCH64_P
#define	ASM_SPEC	 "%{m32:%(asm_spec32)}%{!m32:%(asm_spec64)} %(asm_spec_common)"
#define	LINK_OS_LINUX_SPEC "%{m32:%(link_os_linux_spec32)}%{!m32:%(link_os_linux_spec64)}"
#define	LINK_SECURE_PLT_SPEC "%{m32: " LINK_SECURE_PLT_DEFAULT_SPEC "}"
#else
#define	ASM_SPEC	 "%{!m64:%(asm_spec32)}%{m64:%(asm_spec64)} %(asm_spec_common)"
#define	LINK_OS_LINUX_SPEC "%{!m64:%(link_os_linux_spec32)}%{m64:%(link_os_linux_spec64)}"
#define	LINK_SECURE_PLT_SPEC "%{!m64: " LINK_SECURE_PLT_DEFAULT_SPEC "}"
#endif
#endif

#define ASM_SPEC32 "-a32 \
%{mrelocatable} %{mrelocatable-lib} %{" FPIE_OR_FPIC_SPEC ":-K PIC} \
%{memb|msdata=eabi: -memb}"

#define ASM_SPEC64 "-a64"

#define ASM_SPEC_COMMON "%(asm_cpu) \
%{,assembler|,assembler-with-cpp: %{mregnames} %{mno-regnames}}" \
  ENDIAN_SELECT(" -mbig", " -mlittle", DEFAULT_ASM_ENDIAN)

#undef	SUBSUBTARGET_EXTRA_SPECS
#define SUBSUBTARGET_EXTRA_SPECS \
  { "asm_spec_common",		ASM_SPEC_COMMON },			\
  { "asm_spec32",		ASM_SPEC32 },				\
  { "asm_spec64",		ASM_SPEC64 },				\
  { "link_os_linux_spec32",	LINK_OS_LINUX_SPEC32 },			\
  { "link_os_linux_spec64",	LINK_OS_LINUX_SPEC64 },			\
  { "link_os_extra_spec32",	LINK_OS_EXTRA_SPEC32 },			\
  { "link_os_extra_spec64",	LINK_OS_EXTRA_SPEC64 },			\
  { "link_os_new_dtags",	LINK_OS_NEW_DTAGS_SPEC },		\
  { "include_extra",		INCLUDE_EXTRA_SPEC },			\
  { "dynamic_linker_prefix",	DYNAMIC_LINKER_PREFIX },

/* Optional specs used for overriding the system include directory, default
   -rpath links, and prefix for the dynamic linker.  Normally, there are not
   defined, but if the user configure with the --with-advance-toolchain=<xxx>
   option, the advance-toolchain.h file will override these.  */
#ifndef INCLUDE_EXTRA_SPEC
#define INCLUDE_EXTRA_SPEC	""
#endif

#ifndef LINK_OS_EXTRA_SPEC32
#define LINK_OS_EXTRA_SPEC32	""
#endif

#ifndef LINK_OS_EXTRA_SPEC64
#define LINK_OS_EXTRA_SPEC64	""
#endif

#ifndef LINK_OS_NEW_DTAGS_SPEC
#define LINK_OS_NEW_DTAGS_SPEC	""
#endif

#ifndef DYNAMIC_LINKER_PREFIX
#define DYNAMIC_LINKER_PREFIX	""
#endif

#undef	MULTILIB_DEFAULTS
#if DEFAULT_ARCH64_P
#define MULTILIB_DEFAULTS { "m64" }
#else
#define MULTILIB_DEFAULTS { "m32" }
#endif

/* Split stack is only supported for 64 bit, and requires glibc >= 2.18.  */
#if TARGET_GLIBC_MAJOR * 1000 + TARGET_GLIBC_MINOR >= 2018
# ifndef RS6000_BI_ARCH
#  define TARGET_CAN_SPLIT_STACK
# else
#  if DEFAULT_ARCH64_P
/* Supported, and the default is -m64  */
#   define TARGET_CAN_SPLIT_STACK_64BIT 1
#  else
/* Supported, and the default is -m32  */
#   define TARGET_CAN_SPLIT_STACK_64BIT 0
#  endif
# endif
#endif

#ifndef RS6000_BI_ARCH

/* 64-bit PowerPC Linux always has a TOC.  */
#undef  TARGET_HAS_TOC
#define TARGET_HAS_TOC		1

/* Some things from sysv4.h we don't do when 64 bit.  */
#undef	OPTION_RELOCATABLE
#define	OPTION_RELOCATABLE	0
#undef	OPTION_EABI
#define	OPTION_EABI		0
#undef	OPTION_PROTOTYPE
#define	OPTION_PROTOTYPE	0
#undef RELOCATABLE_NEEDS_FIXUP
#define RELOCATABLE_NEEDS_FIXUP 0

#endif

/* We use glibc _mcount for profiling.  */
#define NO_PROFILE_COUNTERS 1
#define PROFILE_HOOK(LABEL) \
  do { if (TARGET_64BIT) output_profile_hook (LABEL); } while (0)

/* PowerPC64 Linux word-aligns FP doubles when -malign-power is given.  */
#undef  ADJUST_FIELD_ALIGN
#define ADJUST_FIELD_ALIGN(FIELD, TYPE, COMPUTED) \
  ((TARGET_64BIT							\
    && TARGET_ALIGN_NATURAL == 0					\
    && TYPE_MODE (strip_array_types (TYPE)) == DFmode)			\
   ? MIN ((COMPUTED), 32)						\
   : (COMPUTED))

/* PowerPC64 Linux increases natural record alignment to doubleword if
   the first field is an FP double, only if in power alignment mode.  */
#undef  ROUND_TYPE_ALIGN
#define ROUND_TYPE_ALIGN(STRUCT, COMPUTED, SPECIFIED)			\
  ((TARGET_64BIT							\
    && (TREE_CODE (STRUCT) == RECORD_TYPE				\
	|| TREE_CODE (STRUCT) == UNION_TYPE				\
	|| TREE_CODE (STRUCT) == QUAL_UNION_TYPE)			\
    && TARGET_ALIGN_NATURAL == 0)					\
   ? rs6000_special_round_type_align (STRUCT, COMPUTED, SPECIFIED)	\
   : MAX ((COMPUTED), (SPECIFIED)))

/* Use the default for compiling target libs.  */
#ifdef IN_TARGET_LIBS
#undef TARGET_ALIGN_NATURAL
#define TARGET_ALIGN_NATURAL 1
#endif

/* Indicate that jump tables go in the text section.  */
#undef  JUMP_TABLES_IN_TEXT_SECTION
#define JUMP_TABLES_IN_TEXT_SECTION TARGET_64BIT

/* The linux ppc64 ABI isn't explicit on whether aggregates smaller
   than a doubleword should be padded upward or downward.  You could
   reasonably assume that they follow the normal rules for structure
   layout treating the parameter area as any other block of memory,
   then map the reg param area to registers.  i.e. pad upward.
   Setting both of the following defines results in this behavior.
   Setting just the first one will result in aggregates that fit in a
   doubleword being padded downward, and others being padded upward.
   Not a bad idea as this results in struct { int x; } being passed
   the same way as an int.  */
#define AGGREGATE_PADDING_FIXED TARGET_64BIT
#define AGGREGATES_PAD_UPWARD_ALWAYS 0

/* Specify padding for the last element of a block move between
   registers and memory.  FIRST is nonzero if this is the only
   element.  */
#define BLOCK_REG_PADDING(MODE, TYPE, FIRST) \
  (!(FIRST) ? PAD_UPWARD : targetm.calls.function_arg_padding (MODE, TYPE))

/* Linux doesn't support saving and restoring 64-bit regs in a 32-bit
   process.  */
#define OS_MISSING_POWERPC64 !TARGET_64BIT

#ifdef SINGLE_LIBC
#define OPTION_GLIBC_P(opts)	(DEFAULT_LIBC == LIBC_GLIBC)
#define OPTION_UCLIBC_P(opts)	(DEFAULT_LIBC == LIBC_UCLIBC)
#define OPTION_BIONIC_P(opts)	(DEFAULT_LIBC == LIBC_BIONIC)
#undef OPTION_MUSL_P
#define OPTION_MUSL_P(opts)	(DEFAULT_LIBC == LIBC_MUSL)
#else
#define OPTION_GLIBC_P(opts)	((opts)->x_linux_libc == LIBC_GLIBC)
#define OPTION_UCLIBC_P(opts)	((opts)->x_linux_libc == LIBC_UCLIBC)
#define OPTION_BIONIC_P(opts)	((opts)->x_linux_libc == LIBC_BIONIC)
#undef OPTION_MUSL_P
#define OPTION_MUSL_P(opts)	((opts)->x_linux_libc == LIBC_MUSL)
#endif
#define OPTION_GLIBC		OPTION_GLIBC_P (&global_options)
#define OPTION_UCLIBC		OPTION_UCLIBC_P (&global_options)
#define OPTION_BIONIC		OPTION_BIONIC_P (&global_options)
#undef OPTION_MUSL
#define OPTION_MUSL		OPTION_MUSL_P (&global_options)

/* Determine what functions are present at the runtime;
   this includes full c99 runtime and sincos.  */
#undef TARGET_LIBC_HAS_FUNCTION
#define TARGET_LIBC_HAS_FUNCTION linux_libc_has_function

#undef  TARGET_OS_CPP_BUILTINS
#define TARGET_OS_CPP_BUILTINS()			\
  do							\
    {							\
      if (strcmp (rs6000_abi_name, "linux") == 0	\
	  || strcmp (rs6000_abi_name, "aixdesc") == 0)	\
	GNU_USER_TARGET_OS_CPP_BUILTINS();		\
      if (TARGET_64BIT)					\
	{						\
	  builtin_define ("__PPC__");			\
	  builtin_define ("__PPC64__");			\
	  builtin_define ("__powerpc__");		\
	  builtin_define ("__powerpc64__");		\
	  if (!DOT_SYMBOLS)				\
	    builtin_define ("_CALL_LINUX");		\
	  builtin_assert ("cpu=powerpc64");		\
	  builtin_assert ("machine=powerpc64");		\
	}						\
      else						\
	{						\
	  builtin_define_std ("PPC");			\
	  builtin_define_std ("powerpc");		\
	  builtin_assert ("cpu=powerpc");		\
	  builtin_assert ("machine=powerpc");		\
	  TARGET_OS_SYSV_CPP_BUILTINS ();		\
	}						\
    }							\
  while (0)

#undef  CPP_OS_DEFAULT_SPEC
#define CPP_OS_DEFAULT_SPEC "%(cpp_os_linux) %(include_extra)"

#undef  LINK_SHLIB_SPEC
#define LINK_SHLIB_SPEC "%{shared:-shared} %{!shared: %{static:-static}} \
  %{static-pie:-static -pie --no-dynamic-linker -z text}"

#undef  LIB_DEFAULT_SPEC
#define LIB_DEFAULT_SPEC "%(lib_linux)"

#undef  STARTFILE_DEFAULT_SPEC
#define STARTFILE_DEFAULT_SPEC "%(startfile_linux)"

#undef	ENDFILE_DEFAULT_SPEC
#define ENDFILE_DEFAULT_SPEC "%(endfile_linux)"

#undef	LINK_START_DEFAULT_SPEC
#define LINK_START_DEFAULT_SPEC "%(link_start_linux)"

#undef	LINK_OS_DEFAULT_SPEC
#define LINK_OS_DEFAULT_SPEC "%(link_os_linux)"

#define GLIBC_DYNAMIC_LINKER32 "%(dynamic_linker_prefix)/lib/ld.so.1"

#ifdef LINUX64_DEFAULT_ABI_ELFv2
#define GLIBC_DYNAMIC_LINKER64 \
"%{mabi=elfv1:%(dynamic_linker_prefix)/lib64/ld64.so.1;" \
":%(dynamic_linker_prefix)/lib64/ld64.so.2}"
#else
#define GLIBC_DYNAMIC_LINKER64 \
"%{mabi=elfv2:%(dynamic_linker_prefix)/lib64/ld64.so.2;" \
":%(dynamic_linker_prefix)/lib64/ld64.so.1}"
#endif

#undef MUSL_DYNAMIC_LINKER32
#define MUSL_DYNAMIC_LINKER32 \
  "/lib/ld-musl-powerpc" MUSL_DYNAMIC_LINKER_E "%{msoft-float:-sf}.so.1"
#undef MUSL_DYNAMIC_LINKER64
#define MUSL_DYNAMIC_LINKER64 \
  "/lib/ld-musl-powerpc64" MUSL_DYNAMIC_LINKER_E "%{msoft-float:-sf}.so.1"

#undef  DEFAULT_ASM_ENDIAN
#if (TARGET_DEFAULT & MASK_LITTLE_ENDIAN)
#define DEFAULT_ASM_ENDIAN " -mlittle"
#define LINK_OS_LINUX_EMUL32 ENDIAN_SELECT(" -m elf32ppclinux",		\
					   " -m elf32lppclinux",	\
					   " -m elf32lppclinux")
#define LINK_OS_LINUX_EMUL64 ENDIAN_SELECT(" -m elf64ppc",		\
					   " -m elf64lppc",		\
					   " -m elf64lppc")
#else
#define DEFAULT_ASM_ENDIAN " -mbig"
#define LINK_OS_LINUX_EMUL32 ENDIAN_SELECT(" -m elf32ppclinux",		\
					   " -m elf32lppclinux",	\
					   " -m elf32ppclinux")
#define LINK_OS_LINUX_EMUL64 ENDIAN_SELECT(" -m elf64ppc",		\
					   " -m elf64lppc",		\
					   " -m elf64ppc")
#endif

#define LINK_OS_LINUX_SPEC32 LINK_OS_LINUX_EMUL32 " %{!shared: %{!static: \
  %{!static-pie: \
    %{rdynamic:-export-dynamic} \
    -dynamic-linker " GNU_USER_DYNAMIC_LINKER32 "}}} \
  %(link_os_extra_spec32)"

#define LINK_OS_LINUX_SPEC64 LINK_OS_LINUX_EMUL64 " %{!shared: %{!static: \
  %{!static-pie: \
    %{rdynamic:-export-dynamic} \
    -dynamic-linker " GNU_USER_DYNAMIC_LINKER64 "}}} \
  %(link_os_extra_spec64)"

/* Use gnu-user.h LINK_GCC_SEQUENCE_SPEC for linux.  */
#undef LINK_GCC_C_SEQUENCE_SPEC
#define	LINK_GCC_C_SEQUENCE_SPEC \
  "%{mads|myellowknife|mmvme|msim:%G %L %G;" \
  "!mcall-*|mcall-linux:" GNU_USER_TARGET_LINK_GCC_C_SEQUENCE_SPEC ";" \
  ":%G %L %G}"

#undef  TOC_SECTION_ASM_OP
#define TOC_SECTION_ASM_OP \
  (TARGET_64BIT						\
   ? "\t.section\t\".toc\",\"aw\""			\
   : "\t.section\t\".got\",\"aw\"")

#undef  MINIMAL_TOC_SECTION_ASM_OP
#define MINIMAL_TOC_SECTION_ASM_OP \
  (TARGET_64BIT						\
   ? "\t.section\t\".toc1\",\"aw\""			\
   : (flag_pic						\
      ? "\t.section\t\".got2\",\"aw\""			\
      : "\t.section\t\".got1\",\"aw\""))

/* Must be at least as big as our pointer type.  */
#undef	SIZE_TYPE
#define	SIZE_TYPE (TARGET_64BIT ? "long unsigned int" : "unsigned int")

#undef	PTRDIFF_TYPE
#define	PTRDIFF_TYPE (TARGET_64BIT ? "long int" : "int")

#undef	WCHAR_TYPE
#define	WCHAR_TYPE (TARGET_64BIT ? "int" : "long int")
#undef  WCHAR_TYPE_SIZE
#define WCHAR_TYPE_SIZE 32

#undef  RS6000_MCOUNT
#define RS6000_MCOUNT "_mcount"

#ifdef __powerpc64__
/* _init and _fini functions are built from bits spread across many
   object files, each potentially with a different TOC pointer.  For
   that reason, place a nop after the call so that the linker can
   restore the TOC pointer if a TOC adjusting call stub is needed.  */
#if DOT_SYMBOLS
#define CRT_CALL_STATIC_FUNCTION(SECTION_OP, FUNC)	\
  asm (SECTION_OP "\n"					\
"	bl ." #FUNC "\n"				\
"	nop\n"						\
"	.previous");
#else
#define CRT_CALL_STATIC_FUNCTION(SECTION_OP, FUNC)	\
  asm (SECTION_OP "\n"					\
"	bl " #FUNC "\n"					\
"	nop\n"						\
"	.previous");
#endif
#endif

/* FP save and restore routines.  */
#undef  SAVE_FP_PREFIX
#define SAVE_FP_PREFIX (TARGET_64BIT ? "._savef" : "_savefpr_")
#undef  SAVE_FP_SUFFIX
#define SAVE_FP_SUFFIX ""
#undef  RESTORE_FP_PREFIX
#define RESTORE_FP_PREFIX (TARGET_64BIT ? "._restf" : "_restfpr_")
#undef  RESTORE_FP_SUFFIX
#define RESTORE_FP_SUFFIX ""

/* Dwarf2 debugging.  */
#undef  PREFERRED_DEBUGGING_TYPE
#define PREFERRED_DEBUGGING_TYPE DWARF2_DEBUG

/* This is how to declare the size of a function.  */
#undef	ASM_DECLARE_FUNCTION_SIZE
#define	ASM_DECLARE_FUNCTION_SIZE(FILE, FNAME, DECL)			\
  do									\
    {									\
      if (!flag_inhibit_size_directive)					\
	{								\
	  fputs ("\t.size\t", (FILE));					\
	  if (TARGET_64BIT && DOT_SYMBOLS)				\
	    putc ('.', (FILE));						\
	  assemble_name ((FILE), (FNAME));				\
	  fputs (",.-", (FILE));					\
	  rs6000_output_function_entry (FILE, FNAME);			\
	  putc ('\n', (FILE));						\
	}								\
    }									\
  while (0)

/* Return nonzero if this entry is to be written into the constant
   pool in a special way.  We do so if this is a SYMBOL_REF, LABEL_REF
   or a CONST containing one of them.  If -mfp-in-toc (the default),
   we also do this for floating-point constants.  We actually can only
   do this if the FP formats of the target and host machines are the
   same, but we can't check that since not every file that uses
   the macros includes real.h.  We also do this when we can write an
   integer into the TOC and the entry is not larger than a TOC entry,
   but not for -mcmodel=medium where we'll use a toc-relative load for
   constants outside the TOC.  */

#undef  ASM_OUTPUT_SPECIAL_POOL_ENTRY_P
#define ASM_OUTPUT_SPECIAL_POOL_ENTRY_P(X, MODE)			\
  (TARGET_TOC								\
   && (SYMBOL_REF_P (X)							\
       || (GET_CODE (X) == CONST && GET_CODE (XEXP (X, 0)) == PLUS	\
	   && SYMBOL_REF_P (XEXP (XEXP (X, 0), 0)))			\
       || GET_CODE (X) == LABEL_REF					\
       || (CONST_INT_P (X)						\
	   && TARGET_CMODEL != CMODEL_MEDIUM				\
	   && GET_MODE_BITSIZE (MODE) <= GET_MODE_BITSIZE (Pmode))	\
       || (CONST_DOUBLE_P (X)						\
	   && ((TARGET_64BIT						\
		&& (TARGET_MINIMAL_TOC					\
		    || (SCALAR_FLOAT_MODE_P (GET_MODE (X))		\
			&& ! TARGET_NO_FP_IN_TOC)))			\
	       || (!TARGET_64BIT					\
		   && !TARGET_NO_FP_IN_TOC				\
		   && SCALAR_FLOAT_MODE_P (GET_MODE (X))		\
		   && BITS_PER_WORD == HOST_BITS_PER_INT)))))

/* Select a format to encode pointers in exception handling data.  CODE
   is 0 for data, 1 for code labels, 2 for function pointers.  GLOBAL is
   true if the symbol may be affected by dynamic relocations.  */
#undef	ASM_PREFERRED_EH_DATA_FORMAT
#define	ASM_PREFERRED_EH_DATA_FORMAT(CODE, GLOBAL) \
  (TARGET_64BIT || flag_pic						\
   ? (((GLOBAL) ? DW_EH_PE_indirect : 0) | DW_EH_PE_pcrel		\
      | (TARGET_64BIT ? DW_EH_PE_udata8 : DW_EH_PE_sdata4))		\
   : DW_EH_PE_absptr)

/* For backward compatibility, we must continue to use the AIX
   structure return convention.  */
#undef DRAFT_V4_STRUCT_RET
#define DRAFT_V4_STRUCT_RET (!TARGET_64BIT)

#ifdef TARGET_LIBC_PROVIDES_SSP
/* ppc32 glibc provides __stack_chk_guard in -0x7008(2),
   ppc64 glibc provides it at -0x7010(13).  */
#define TARGET_THREAD_SSP_OFFSET	(TARGET_64BIT ? -0x7010 : -0x7008)
#endif

#define POWERPC_LINUX

/* ppc{32,64} linux has 128-bit long double support in glibc 2.4 and later.  */
#ifdef TARGET_DEFAULT_LONG_DOUBLE_128
#define RS6000_DEFAULT_LONG_DOUBLE_SIZE 128
#endif

/* Static stack checking is supported by means of probes.  */
#define STACK_CHECK_STATIC_BUILTIN 1

/* The default value isn't sufficient in 64-bit mode.  */
#define STACK_CHECK_PROTECT (TARGET_64BIT ? 16 * 1024 : 12 * 1024)

/* Software floating point support for exceptions and rounding modes
   depends on the C library in use.  */
#undef TARGET_FLOAT_EXCEPTIONS_ROUNDING_SUPPORTED_P
#define TARGET_FLOAT_EXCEPTIONS_ROUNDING_SUPPORTED_P \
  rs6000_linux_float_exceptions_rounding_supported_p

/* Support for TARGET_ATOMIC_ASSIGN_EXPAND_FENV without FPRs depends
   on glibc 2.19 or greater.  */
#if TARGET_GLIBC_MAJOR > 2 \
  || (TARGET_GLIBC_MAJOR == 2 && TARGET_GLIBC_MINOR >= 19)
#define RS6000_GLIBC_ATOMIC_FENV 1
#endif

/* The IEEE 128-bit emulator is only built on Linux systems.  Flag that we
   should enable the type handling for KFmode on VSX systems even if we are not
   enabling the __float128 keyword.  */
#undef	TARGET_FLOAT128_ENABLE_TYPE
#define TARGET_FLOAT128_ENABLE_TYPE 1

/* Enable using prefixed PC-relative addressing on POWER10 if the ABI
   supports it.  The ELF v2 ABI only supports PC-relative relocations for
   the medium code model.  */
#define PCREL_SUPPORTED_BY_OS	(TARGET_POWER10 && TARGET_PREFIXED	\
				 && ELFv2_ABI_CHECK			\
				 && TARGET_CMODEL == CMODEL_MEDIUM)
