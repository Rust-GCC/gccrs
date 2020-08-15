/* Definitions for rtems targeting the Moxie core.
   Copyright (C) 2010-2019 Free Software Foundation, Inc.
   Contributed by Anthony Green (green@moxielogic.com)

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

/* Target OS preprocessor built-ins.  */
#define TARGET_OS_CPP_BUILTINS()		\
  do						\
    {						\
      builtin_define_std ("moxie");		\
      builtin_define ("__rtems__");		\
      builtin_assert ("system=rtems");		\
    }						\
  while (0)

#define TARGET_RUST_OS_INFO()		\
  do {						\
    /*note: as far as I know, rustc has no supported for rtems, so this is just guessed*/ \
    /*everything is subject to change, especially target_env and target_family*/ \
    builtin_rust_info ("target_family", "unix");			\
    builtin_rust_info ("target_os", "rtems");			\
    builtin_rust_info ("target_vendor", "unknown");			\
    builtin_rust_info ("target_env", "");			\
  } while (0)

#undef LINK_SPEC
#undef SIZE_TYPE
#undef PTRDIFF_TYPE
#undef WCHAR_TYPE
#undef WCHAR_TYPE_SIZE
