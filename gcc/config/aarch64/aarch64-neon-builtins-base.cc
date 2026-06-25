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
/* Convert a lane index to the correct index for the target endianness.
   For little-endian targets, this is a no-op.
   For big-endian targets, this is `len - index - 1`.  */
tree
convert_lane_index (poly_uint64 len, tree index)
{
  auto index_type = TREE_TYPE (index);
  return !BYTES_BIG_ENDIAN
	   ? index
	   : fold_build2 (MINUS_EXPR, index_type,
			  fold_build2 (MINUS_EXPR, index_type,
				       build_int_cst_type (index_type, len),
				       index),
			  build_one_cst (index_type));
}

/* Build a `VEC[INDEX]` expression.  */
tree
build_lane_get (tree vec, tree index)
{
  auto vec_type = TREE_TYPE (vec);
  auto vec_len = TYPE_VECTOR_SUBPARTS (vec_type);
  auto elem_type = TREE_TYPE (vec_type);
  auto elem_size = TYPE_SIZE (elem_type);
  auto offset = fold_build2 (MULT_EXPR, bitsizetype, elem_size,
			     convert_lane_index (vec_len, index));
  return fold_build3 (BIT_FIELD_REF, elem_type, vec, elem_size, offset);
}

/* Build a `VEC[INDEX] = ELEM;` statement.
   Returns an expression representing the updated VEC.  */
tree
build_lane_set (tree vec, tree index, tree elem)
{
  auto vec_type = TREE_TYPE (vec);
  auto vec_len = TYPE_VECTOR_SUBPARTS (vec_type);
  auto elem_type = TREE_TYPE (vec_type);
  auto elem_size = TYPE_SIZE (elem_type);
  auto offset = fold_build2 (MULT_EXPR, bitsizetype, elem_size,
			     convert_lane_index (vec_len, index));
  return fold_build3 (BIT_INSERT_EXPR, vec_type, vec, elem, offset);
}

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

struct gimple_get_lane : public gimple_function_base
{
  gimple *fold (gimple_folder &f) const override
  {
    auto src = gimple_call_arg (f.call, 0);
    auto lane = gimple_call_arg (f.call, 1);
    return gimple_build_assign (f.lhs, build_lane_get (src, lane));
  }
};

template <bool high_half_p>
struct gimple_get_half : public gimple_function_base
{
  gimple *fold (gimple_folder &f) const override
  {
    auto src = gimple_call_arg (f.call, 0);
    auto half
      = fold_build3 (BIT_FIELD_REF, TREE_TYPE (f.lhs), src, bitsize_int (64),
		     bitsize_int (high_half_p ^ BYTES_BIG_ENDIAN ? 64 : 0));
    return gimple_build_assign (f.lhs, half);
  }
};

struct gimple_set_lane : public gimple_function_base
{
  gimple *fold (gimple_folder &f) const override
  {
    auto elem = gimple_call_arg (f.call, 0);
    auto src = gimple_call_arg (f.call, 1);
    auto lane = gimple_call_arg (f.call, 2);
    return gimple_build_assign (f.lhs, build_lane_set (src, lane, elem));
  }
};

struct gimple_copy_lane : public gimple_function_base
{
  gimple *fold (gimple_folder &f) const override
  {
    auto vec1 = gimple_call_arg (f.call, 0);
    auto lane1 = gimple_call_arg (f.call, 1);
    auto vec2 = gimple_call_arg (f.call, 2);
    auto lane2 = gimple_call_arg (f.call, 3);

    // tmp1 = vec2[lane2]
    auto tmp1 = f.force_val (build_lane_get (vec2, lane2));

    // vec1[lane1] = tmp1
    // lhs = vec1
    return gimple_build_assign (f.lhs, build_lane_set (vec1, lane1, tmp1));
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

struct gimple_dup_lane : public gimple_function_base
{
  gimple *fold (gimple_folder &f) const override
  {
    auto vec = gimple_call_arg (f.call, 0);
    auto lane = gimple_call_arg (f.call, 1);
    auto type = TREE_TYPE (f.lhs);

    // tmp1 = vec[lane]
    auto tmp1 = f.force_val (build_lane_get (vec, lane));

    // lhs = VEC_DUP (vec[lane])
    return gimple_build_assign (f.lhs, build_vec_dup (type, tmp1));
  }
};

// Lane get/set
NEON_FUNCTION (vget_lane,    gimple_get_lane,)
NEON_FUNCTION (vgetq_lane,   gimple_get_lane,)
NEON_FUNCTION (vget_low,     gimple_get_half<false>,)
NEON_FUNCTION (vget_high,    gimple_get_half<true>,)
NEON_FUNCTION (vset_lane,    gimple_set_lane,)
NEON_FUNCTION (vsetq_lane,   gimple_set_lane,)
NEON_FUNCTION (vcopy_lane,   gimple_copy_lane,)
NEON_FUNCTION (vcopy_laneq,  gimple_copy_lane,)
NEON_FUNCTION (vcopyq_laneq, gimple_copy_lane,)
NEON_FUNCTION (vcopyq_lane,  gimple_copy_lane,)
NEON_FUNCTION (vdup_lane,    gimple_dup_lane,)
NEON_FUNCTION (vdupq_lane,   gimple_dup_lane,)
NEON_FUNCTION (vdup_laneq,   gimple_dup_lane,)
NEON_FUNCTION (vdupq_laneq,  gimple_dup_lane,)
NEON_FUNCTION (vdupb_lane,   gimple_get_lane,)
NEON_FUNCTION (vdupb_laneq,  gimple_get_lane,)
NEON_FUNCTION (vduph_lane,   gimple_get_lane,)
NEON_FUNCTION (vduph_laneq,  gimple_get_lane,)
NEON_FUNCTION (vdups_lane,   gimple_get_lane,)
NEON_FUNCTION (vdups_laneq,  gimple_get_lane,)
NEON_FUNCTION (vdupd_lane,   gimple_get_lane,)
NEON_FUNCTION (vdupd_laneq,  gimple_get_lane,)

// Lanewise arithmetic
NEON_FUNCTION (vaddd, gimple_expr, (PLUS_EXPR))
NEON_FUNCTION (vadd,  gimple_expr, (PLUS_EXPR, PLUS_EXPR, BIT_XOR_EXPR))
NEON_FUNCTION (vaddq, gimple_expr, (PLUS_EXPR, PLUS_EXPR, BIT_XOR_EXPR))
}
