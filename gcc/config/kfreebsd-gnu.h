/* Definitions for kFreeBSD-based GNU systems with ELF format
   Copyright (C) 2004-2019 Free Software Foundation, Inc.
   Contributed by Robert Millan.

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

#define GNU_USER_TARGET_OS_CPP_BUILTINS()		\
  do						\
    {						\
	builtin_define ("__FreeBSD_kernel__");	\
	builtin_define ("__GLIBC__");		\
	builtin_define_std ("unix");		\
	builtin_assert ("system=unix");		\
	builtin_assert ("system=posix");	\
    }						\
  while (0)

#define GNU_USER_TARGET_D_OS_VERSIONS()		\
    do {					\
	builtin_version ("FreeBSD");		\
	builtin_version ("CRuntime_Glibc");	\
    } while (0)

#define GNU_USER_TARGET_RUST_OS_INFO()		\
    do {					\
  builtin_rust_info ("target_family", "unix");			\
  builtin_rust_info ("target_os", "kfreebsd");			\
  builtin_rust_info ("target_vendor", "unknown");			\
  builtin_rust_info ("target_env", "gnu");			\
    } while (0)

#define GNU_USER_DYNAMIC_LINKER        GLIBC_DYNAMIC_LINKER
#define GNU_USER_DYNAMIC_LINKER32      GLIBC_DYNAMIC_LINKER32
#define GNU_USER_DYNAMIC_LINKER64      GLIBC_DYNAMIC_LINKER64
#define GNU_USER_DYNAMIC_LINKERX32     GLIBC_DYNAMIC_LINKERX32
