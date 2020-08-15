/* Definitions for RTEMS based AARCH64 system.
   Copyright (C) 2016-2019 Free Software Foundation, Inc.

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

#define HAS_INIT_SECTION

#undef TARGET_OS_CPP_BUILTINS
#define TARGET_OS_CPP_BUILTINS()		\
    do {					\
	builtin_define ("__rtems__");		\
	builtin_define ("__USE_INIT_FINI__");	\
	builtin_assert ("system=rtems");	\
    } while (0)

#ifdef TARGET_RUST_OS_INFO
# error "TARGET_RUST_OS_INFO already defined in rtems.h (aarch64) - c++ undefines it and redefines it."
#endif
#define TARGET_RUST_OS_INFO()		\
  do {		\
    /*note: as far as I know, rustc has no supported for rtems, so this is just guessed*/ \
    /*everything is subject to change, especially target_env and target_family*/ \
    builtin_rust_info ("target_family", "unix");			\
    builtin_rust_info ("target_os", "rtems");			\
    builtin_rust_info ("target_vendor", "unknown");			\
    builtin_rust_info ("target_env", "");			\
  } while (0)