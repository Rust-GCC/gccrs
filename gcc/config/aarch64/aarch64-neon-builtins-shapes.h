/* ACLE support for AArch64 NEON (function shapes)
   Copyright (C) 2026-2026 Free Software Foundation, Inc.

   This file is part of GCC.

   GCC is free software; you can redistribute it and/or modify it
   under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 3, or (at your option)
   any later version.

   GCC is distributed in the hope that it will be useful, but
   WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with GCC; see the file COPYING3.  If not see
   <http://www.gnu.org/licenses/>.  */

#ifndef GCC_AARCH64_NEON_BUILTINS_SHAPES_H
#define GCC_AARCH64_NEON_BUILTINS_SHAPES_H

namespace aarch64_acle::shapes {
#define DEF_NEON_FUNCTION(NAME, TYPES, SHAPE_ARGS) \
  extern const aarch64_acle::function_shape *const SHAPE_NAME (NAME, TYPES);
#include "aarch64-neon-builtins.def"
}

#endif
