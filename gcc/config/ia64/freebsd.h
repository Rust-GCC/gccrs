/* Definitions for Intel IA-64 running FreeBSD using the ELF format
   Copyright (C) 2001-2023 Free Software Foundation, Inc.
   Contributed by David E. O'Brien <obrien@FreeBSD.org> and BSDi.

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

#undef  SUBTARGET_EXTRA_SPECS
#define SUBTARGET_EXTRA_SPECS \
  { "fbsd_dynamic_linker", FBSD_DYNAMIC_LINKER }

#define LINK_SPEC "							\
  %{p:%nconsider using '-pg' instead of '-p' with gprof(1)}		\
  %{assert*} %{R*} %{rpath*} %{defsym*}					\
  %{shared:-Bshareable %{h*} %{soname*}}				\
  %{symbolic:-Bsymbolic}						\
  %{!shared:								\
    %{!static:								\
      %{rdynamic:-export-dynamic}					\
      -dynamic-linker %(fbsd_dynamic_linker) }	\
    %{static:-Bstatic}}"


/************************[  Target stuff  ]***********************************/

/* Define the actual types of some ANSI-mandated types.  
   Needs to agree with <machine/ansi.h>.  GCC defaults come from c-decl.cc,
   c-common.cc, and config/<arch>/<arch>.h.  */

/* Earlier headers may get this wrong for FreeBSD.
   We use the GCC defaults instead.  */
#undef WCHAR_TYPE

#undef  WCHAR_TYPE_SIZE
#define WCHAR_TYPE_SIZE 32

#define TARGET_ELF		1

#define JMP_BUF_SIZE  76
