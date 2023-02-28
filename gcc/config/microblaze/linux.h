/* Definitions for MicroBlaze running Linux.
   Copyright (C) 2009-2023 Free Software Foundation, Inc.

   This file is part of GCC.

   Contributed by Michael Eager <eager@eagercon.com>.

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

#undef TARGET_SUPPORTS_PIC
#define TARGET_SUPPORTS_PIC 1

#undef CPP_SPEC
#define CPP_SPEC "%{posix:-D_POSIX_SOURCE} %{pthread:-D_REENTRANT}"

#undef TLS_NEEDS_GOT
#define TLS_NEEDS_GOT 1

#define GLIBC_DYNAMIC_LINKER "/lib/ld.so.1"
#define UCLIBC_DYNAMIC_LINKER "/lib/ld-uClibc.so.0"

#if TARGET_BIG_ENDIAN_DEFAULT == 0 /* LE */
#define MUSL_DYNAMIC_LINKER_E "%{mbig-endian:;:el}"
#else
#define MUSL_DYNAMIC_LINKER_E "%{mlittle-endian:el}"
#endif

#undef MUSL_DYNAMIC_LINKER
#define MUSL_DYNAMIC_LINKER "/lib/ld-musl-microblaze" MUSL_DYNAMIC_LINKER_E ".so.1"

#undef  SUBTARGET_EXTRA_SPECS
#define SUBTARGET_EXTRA_SPECS \
  { "dynamic_linker", GNU_USER_DYNAMIC_LINKER }

#undef LINK_SPEC
#define LINK_SPEC "%{shared:-shared} \
  %{!shared: \
    %{!static: \
      %{rdynamic:-export-dynamic} \
      -dynamic-linker %(dynamic_linker)} \
    %{static:-static}} \
  %{mbig-endian:-EB} \
  %{mlittle-endian:-EL}"

/* For the microblaze-*-linux* subtarget.  */
#undef TARGET_OS_CPP_BUILTINS
#define TARGET_OS_CPP_BUILTINS() GNU_USER_TARGET_OS_CPP_BUILTINS()

#define TARGET_ASM_FILE_END file_end_indicate_exec_stack
