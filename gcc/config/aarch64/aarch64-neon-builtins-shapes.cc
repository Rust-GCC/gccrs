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

#define INCLUDE_ALGORITHM
#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "tm.h"
#include "basic-block.h"
#include "tree.h"
#include "function.h"
#include "gimple.h"
#include "rtl.h"
#include "tm_p.h"
#include "memmodel.h"
#include "insn-codes.h"
#include "optabs.h"
#include "aarch64-acle-builtins.h"
#include "aarch64-sve-builtins-shapes.h"
#include "aarch64-builtins.h"

using namespace aarch64_acle;

/* All NEON functions are non-overloaded, so we don't need bespoke
  function shapes.  Instead, we can just use a single shape for all NEON
  functions, parameterised by a signature.  */
struct neon_shape : public function_shape
{
  constexpr neon_shape (const char *signature)
    : m_signature (signature)
  {}

  const char *m_signature;

  void build (function_builder &b,
	      const function_group_info &group) const override
  {
    aarch64_acle::build_all (b, this->m_signature, group, MODE_none);
  }

  bool check (function_checker &) const override { return true; }

  bool explicit_type_suffix_p (unsigned int) const override { return true; }
  tree resolve (function_resolver &) const override { gcc_unreachable (); }
};

namespace aarch64_acle::shapes {
#define DEF_NEON_FUNCTION(NAME, TYPES, SHAPE_ARGS)			\
  static constexpr const neon_shape OBJ_NAME (NAME, TYPES) SHAPE_ARGS;	\
  const aarch64_acle::function_shape *SHAPE_NAME (NAME, TYPES)		\
    = &OBJ_NAME (NAME, TYPES);
#include "aarch64-neon-builtins.def"
}
