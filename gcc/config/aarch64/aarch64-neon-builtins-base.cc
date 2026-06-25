/* ACLE support for AArch64 NEON (__ARM_FEATURE_SIMD intrinsics)
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

#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "tm.h"
#include "tree.h"
#include "rtl.h"
#include "tm_p.h"
#include "memmodel.h"
#include "insn-codes.h"
#include "optabs.h"
#include "recog.h"
#include "expr.h"
#include "basic-block.h"
#include "function.h"
#include "fold-const.h"
#include "gimple.h"
#include "gimple-iterator.h"
#include "gimplify.h"
#include "explow.h"
#include "tree-vector-builder.h"
#include "rtx-vector-builder.h"
#include "vec-perm-indices.h"
#include "aarch64-acle-builtins.h"
#include "aarch64-neon-builtins-base.h"
#include "aarch64-neon-builtins-functions.h"
#include "gimple-fold.h"

namespace aarch64_acle {

/* Build an expression for a vector with all lanes set to `ELEM`.  */
tree
build_vec_dup (tree type, tree elem)
{
  return known_eq (TYPE_VECTOR_SUBPARTS (type), 1U)
	   ? fold_build1 (VIEW_CONVERT_EXPR, type, elem)
	   : fold_build1 (VEC_DUPLICATE_EXPR, type, elem);
}

/* Base class for all function expanders.
   At least one of `expand` or `fold` must be overriden by derived classes.  */
class gimple_function_base : public function_base
{
  rtx expand (function_expander &) const override { gcc_unreachable (); }
  gimple *fold (gimple_folder &) const override { gcc_unreachable (); }
};

struct gimple_create : public gimple_function_base
{
  gimple *fold (gimple_folder &f) const override
  {
    auto arg = gimple_call_arg (f.call, 0);
    return gimple_build_assign (f.lhs, fold_build1 (VIEW_CONVERT_EXPR,
						    TREE_TYPE (f.lhs), arg));
  }
};

struct gimple_combine : public gimple_function_base
{
  gimple *fold (gimple_folder &f) const override
  {
    auto arg1 = gimple_call_arg (f.call, 0);
    auto arg2 = gimple_call_arg (f.call, 1);
    auto arg_type = TREE_TYPE (arg1);
    auto elem_type = TREE_TYPE (arg_type);
    auto ret_type = TREE_TYPE (f.lhs);

    if (known_eq (TYPE_VECTOR_SUBPARTS (arg_type), 1U))
      {
	arg1 = f.force_val (fold_build1 (VIEW_CONVERT_EXPR, elem_type, arg1));
	arg2 = f.force_val (fold_build1 (VIEW_CONVERT_EXPR, elem_type, arg2));
      }

    if (BYTES_BIG_ENDIAN)
      std::swap (arg1, arg2);

    return gimple_build_assign (f.lhs,
				build_constructor_va (ret_type, 2, NULL_TREE,
						      arg1, NULL_TREE, arg2));
  }
};

/* For intrinsics that map to a single GIMPLE expression with no argument
   preparation necessary.  */
class gimple_expr : public gimple_function_base
{
  tree_code m_int_code;
  tree_code m_float_code;
  tree_code m_poly_code;

public:
  constexpr gimple_expr (tree_code code)
    : m_int_code (code),
      m_float_code (code),
      m_poly_code (code)
    {}

  constexpr gimple_expr (tree_code int_code,
			 tree_code float_code,
			 tree_code poly_code)
    : m_int_code (int_code),
      m_float_code (float_code),
      m_poly_code (poly_code)
    {}

  gimple *fold (gimple_folder &f) const override
  {
    auto nargs = gimple_call_num_args (f.call);
    auto arg0 = nargs >= 1 ? gimple_call_arg (f.call, 0) : nullptr;
    auto arg1 = nargs >= 2 ? gimple_call_arg (f.call, 1) : nullptr;
    auto arg2 = nargs >= 3 ? gimple_call_arg (f.call, 2) : nullptr;

    auto type_class = f.type_suffix (0).tclass;
    switch (type_class)
      {
      case TYPE_signed:
      case TYPE_unsigned:
	return gimple_build_assign (f.lhs, m_int_code, arg0, arg1, arg2);
      case TYPE_float:
	return gimple_build_assign (f.lhs, m_float_code, arg0, arg1, arg2);
      case TYPE_poly:
	return gimple_build_assign (f.lhs, m_poly_code, arg0, arg1, arg2);
      default:
	gcc_unreachable ();
      }
  }
};

struct gimple_dup : public gimple_function_base
{
  gimple *fold (gimple_folder &f) const override
  {
    auto elem = gimple_call_arg (f.call, 0);
    auto ret_type = TREE_TYPE (f.lhs);
    return gimple_build_assign (f.lhs, build_vec_dup (ret_type, elem));
  }
};

// Vector creation
NEON_FUNCTION (vcreate,		gimple_create,)
NEON_FUNCTION (vcombine,	gimple_combine,)
NEON_FUNCTION (vdup_n,		gimple_dup,)
NEON_FUNCTION (vdupq_n,		gimple_dup,)
NEON_FUNCTION (vmov_n,		gimple_dup,)
NEON_FUNCTION (vmovq_n,		gimple_dup,)

// Lanewise arithmetic
NEON_FUNCTION (vaddd, gimple_expr, (PLUS_EXPR))
NEON_FUNCTION (vadd,  gimple_expr, (PLUS_EXPR, PLUS_EXPR, BIT_XOR_EXPR))
NEON_FUNCTION (vaddq, gimple_expr, (PLUS_EXPR, PLUS_EXPR, BIT_XOR_EXPR))
}
