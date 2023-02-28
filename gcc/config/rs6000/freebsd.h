/* Definitions for PowerPC running FreeBSD using the ELF format
   Copyright (C) 2001-2023 Free Software Foundation, Inc.
   Contributed by David E. O'Brien <obrien@FreeBSD.org> and BSDi.

   This file is part of GCC.

   GCC is free software; you can redistribute it and/or modify it
   under the terms of the GNU General Public License as published
   by the Free Software Foundation; either version 3, or (at your
   option) any later version.

   GCC is distributed in the hope that it will be useful, but WITHOUT
   ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
   or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public
   License for more details.

   You should have received a copy of the GNU General Public License
   along with GCC; see the file COPYING3.  If not see
   <http://www.gnu.org/licenses/>.  */

/* Undef gnu-user.h macros we don't want.  */
#undef CPLUSPLUS_CPP_SPEC
#undef LINK_GCC_C_SEQUENCE_SPEC

/* Override the defaults, which exist to force the proper definition.  */

#undef	CPP_OS_DEFAULT_SPEC
#define CPP_OS_DEFAULT_SPEC "%(cpp_os_freebsd)"

#undef	STARTFILE_DEFAULT_SPEC
#define STARTFILE_DEFAULT_SPEC "%(startfile_freebsd)"

#undef	ENDFILE_DEFAULT_SPEC
#define ENDFILE_DEFAULT_SPEC "%(endfile_freebsd)"

#undef	LIB_DEFAULT_SPEC
#define LIB_DEFAULT_SPEC "%(lib_freebsd)"

#undef	LINK_START_DEFAULT_SPEC
#define LINK_START_DEFAULT_SPEC "%(link_start_freebsd)"

#undef	LINK_OS_DEFAULT_SPEC
#define	LINK_OS_DEFAULT_SPEC "%(link_os_freebsd)"

/* XXX: This is wrong for many platforms in sysv4.h.
   We should work on getting that definition fixed.  */
#undef  LINK_SHLIB_SPEC
#define LINK_SHLIB_SPEC "%{shared:-shared} %{!shared: %{static:-static}}"


/************************[  Target stuff  ]***********************************/

/* Define the actual types of some ANSI-mandated types.  
   Needs to agree with <machine/ansi.h>.  GCC defaults come from c-decl.cc,
   c-common.cc, and config/<arch>/<arch>.h.  */

#undef  SIZE_TYPE
#define SIZE_TYPE "unsigned int"

/* rs6000.h gets this wrong for FreeBSD.  We use the GCC defaults instead.  */
#undef WCHAR_TYPE

#undef  WCHAR_TYPE_SIZE
#define WCHAR_TYPE_SIZE 32

/* We don't need to generate entries in .fixup, except when
   -mrelocatable or -mrelocatable-lib is given.  */
#undef RELOCATABLE_NEEDS_FIXUP
#define RELOCATABLE_NEEDS_FIXUP \
  (rs6000_isa_flags & rs6000_isa_flags_explicit & OPTION_MASK_RELOCATABLE)

/* Use standard DWARF numbering for DWARF debugging information.  */
#define RS6000_USE_DWARF_NUMBERING

#define POWERPC_FREEBSD
