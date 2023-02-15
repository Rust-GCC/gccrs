/* Definitions of MIPS sub target machine for GNU compiler.
   Toshiba r3900.  You should include mips.h after this.

   Copyright (C) 1989-2023 Free Software Foundation, Inc.
   Contributed by Gavin Koch (gavin@cygnus.com).

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

#undef MIPS_CPU_STRING_DEFAULT
#define MIPS_CPU_STRING_DEFAULT "r3900"
#define MIPS_ISA_DEFAULT 1

#undef MULTILIB_DEFAULTS
#define MULTILIB_DEFAULTS { MULTILIB_ENDIAN_DEFAULT, "msoft-float" }

/* We use the MIPS EABI by default.  */
#undef MIPS_ABI_DEFAULT
#define MIPS_ABI_DEFAULT ABI_EABI

/* By default (if not mips-something-else) produce code for the r3900 */
#undef SUBTARGET_CC1_SPEC
#define SUBTARGET_CC1_SPEC "\
%{mhard-float:%e-mhard-float not supported} \
%{msingle-float:%{msoft-float: \
  %e-msingle-float and -msoft-float cannot both be specified}}"
