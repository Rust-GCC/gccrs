/* function_base implementation for RISC-V 'V' Extension for GNU compiler.
   Copyright (C) 2022-2023 Free Software Foundation, Inc.
   Contributed by Ju-Zhe Zhong (juzhe.zhong@rivai.ai), RiVAI Technologies Ltd.

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
#include "emit-rtl.h"
#include "tree-vector-builder.h"
#include "rtx-vector-builder.h"
#include "riscv-vector-builtins.h"
#include "riscv-vector-builtins-shapes.h"
#include "riscv-vector-builtins-bases.h"

using namespace riscv_vector;

namespace riscv_vector {

/* Enumerates types of loads/stores operations.
   It's only used in here so we don't define it
   in riscv-vector-builtins-bases.h.  */
enum lst_type
{
  LST_UNIT_STRIDE,
  LST_STRIDED,
  LST_INDEXED,
};

/* Implements vsetvl<mode> && vsetvlmax<mode>.  */
template<bool VLMAX_P>
class vsetvl : public function_base
{
public:
  bool apply_vl_p () const override
  {
    return false;
  }

  rtx expand (function_expander &e) const override
  {
    if (VLMAX_P)
      e.add_input_operand (Pmode, gen_rtx_REG (Pmode, 0));
    else
      e.add_input_operand (0);

    tree type = builtin_types[e.type.index].vector;
    machine_mode mode = TYPE_MODE (type);
    machine_mode inner_mode = GET_MODE_INNER (mode);
    /* SEW.  */
    e.add_input_operand (Pmode,
			 gen_int_mode (GET_MODE_BITSIZE (inner_mode), Pmode));

    /* LMUL.  */
    e.add_input_operand (Pmode, gen_int_mode (get_vlmul (mode), Pmode));

    /* TA.  */
    e.add_input_operand (Pmode, gen_int_mode (1, Pmode));

    /* MU.  */
    e.add_input_operand (Pmode, gen_int_mode (0, Pmode));
    return e.generate_insn (code_for_vsetvl_no_side_effects (Pmode));
  }
};

/* Implements
 * vle.v/vse.v/vlm.v/vsm.v/vlse.v/vsse.v/vluxei.v/vloxei.v/vsuxei.v/vsoxei.v
 * codegen.  */
template<bool STORE_P, lst_type LST_TYPE, bool ORDERED_P>
class loadstore : public function_base
{
public:
  bool apply_tail_policy_p () const override { return !STORE_P; }
  bool apply_mask_policy_p () const override { return !STORE_P; }

  unsigned int call_properties (const function_instance &) const override
  {
    if (STORE_P)
      return CP_WRITE_MEMORY;
    else
      return CP_READ_MEMORY;
  }

  bool can_be_overloaded_p (enum predication_type_index pred) const override
  {
    if (STORE_P || LST_TYPE == LST_INDEXED)
      return true;
    return pred != PRED_TYPE_none && pred != PRED_TYPE_mu;
  }

  rtx expand (function_expander &e) const override
  {
    if (LST_TYPE == LST_INDEXED)
      {
	int unspec = ORDERED_P ? UNSPEC_ORDERED : UNSPEC_UNORDERED;
	if (STORE_P)
	  return e.use_exact_insn (
	    code_for_pred_indexed_store (unspec, e.vector_mode (),
					 e.index_mode ()));
	else
	  return e.use_exact_insn (
	    code_for_pred_indexed_load (unspec, e.vector_mode (),
					e.index_mode ()));
      }
    else if (LST_TYPE == LST_STRIDED)
      {
	if (STORE_P)
	  return e.use_contiguous_store_insn (
	    code_for_pred_strided_store (e.vector_mode ()));
	else
	  return e.use_contiguous_load_insn (
	    code_for_pred_strided_load (e.vector_mode ()));
      }
    else
      {
	if (STORE_P)
	  return e.use_contiguous_store_insn (
	    code_for_pred_store (e.vector_mode ()));
	else
	  return e.use_contiguous_load_insn (
	    code_for_pred_mov (e.vector_mode ()));
      }
  }
};

/* Implements
 * vadd/vsub/vand/vor/vxor/vsll/vsra/vsrl/vmin/vmax/vminu/vmaxu/vdiv/vrem/vdivu/vremu/vsadd/vsaddu/vssub/vssubu.
 */
template<rtx_code CODE>
class binop : public function_base
{
public:
  rtx expand (function_expander &e) const override
  {
    switch (e.op_info->op)
      {
      case OP_TYPE_vx:
	return e.use_exact_insn (code_for_pred_scalar (CODE, e.vector_mode ()));
      case OP_TYPE_vv:
	return e.use_exact_insn (code_for_pred (CODE, e.vector_mode ()));
      default:
	gcc_unreachable ();
      }
  }
};

/* Implements vrsub.  */
class vrsub : public function_base
{
public:
  rtx expand (function_expander &e) const override
  {
    return e.use_exact_insn (
      code_for_pred_sub_reverse_scalar (e.vector_mode ()));
  }
};

/* Implements vneg/vnot.  */
template<rtx_code CODE>
class unop : public function_base
{
public:
  rtx expand (function_expander &e) const override
  {
    return e.use_exact_insn (code_for_pred (CODE, e.vector_mode ()));
  }
};

/* Implements vsext.vf2/vsext.vf4/vsext.vf8/vzext.vf2/vzext.vf4/vzext.vf8.  */
template<rtx_code CODE>
class ext : public function_base
{
public:
  rtx expand (function_expander &e) const override
  {
    switch (e.op_info->op)
      {
      case OP_TYPE_vf2:
	return e.use_exact_insn (code_for_pred_vf2 (CODE, e.vector_mode ()));
      case OP_TYPE_vf4:
	return e.use_exact_insn (code_for_pred_vf4 (CODE, e.vector_mode ()));
      case OP_TYPE_vf8:
	return e.use_exact_insn (code_for_pred_vf8 (CODE, e.vector_mode ()));
      default:
	gcc_unreachable ();
      }
  }
};

/* Implements vmulh/vmulhu/vmulhsu.  */
template<int UNSPEC>
class vmulh : public function_base
{
public:
  rtx expand (function_expander &e) const override
  {
    switch (e.op_info->op)
      {
      case OP_TYPE_vx:
	return e.use_exact_insn (
	  code_for_pred_mulh_scalar (UNSPEC, e.vector_mode ()));
      case OP_TYPE_vv:
	return e.use_exact_insn (
	  code_for_pred_mulh (UNSPEC, e.vector_mode ()));
      default:
	gcc_unreachable ();
      }
  }
};

/* Implements vwadd/vwsub/vwmul.  */
template<rtx_code CODE1, rtx_code CODE2>
class widen_binop : public function_base
{
public:
  rtx expand (function_expander &e) const override
  {
    switch (e.op_info->op)
      {
      case OP_TYPE_vv:
	return e.use_exact_insn (
	  code_for_pred_dual_widen (CODE1, CODE2, e.vector_mode ()));
      case OP_TYPE_vx:
	return e.use_exact_insn (
	  code_for_pred_dual_widen_scalar (CODE1, CODE2, e.vector_mode ()));
      case OP_TYPE_wv:
	return e.use_exact_insn (
	  code_for_pred_single_widen (CODE1, CODE2, e.vector_mode ()));
      case OP_TYPE_wx:
	return e.use_exact_insn (
	  code_for_pred_single_widen_scalar (CODE1, CODE2, e.vector_mode ()));
      default:
	gcc_unreachable ();
      }
  }
};

/* Implements vwmulsu.  */
class vwmulsu : public function_base
{
public:
  rtx expand (function_expander &e) const override
  {
    switch (e.op_info->op)
      {
      case OP_TYPE_vv:
	return e.use_exact_insn (code_for_pred_widen_mulsu (e.vector_mode ()));
      case OP_TYPE_vx:
	return e.use_exact_insn (
	  code_for_pred_widen_mulsu_scalar (e.vector_mode ()));
      default:
	gcc_unreachable ();
      }
  }
};

/* Implements vwcvt.  */
template<rtx_code CODE>
class vwcvt : public function_base
{
public:
  rtx expand (function_expander &e) const override
  {
    return e.use_exact_insn (code_for_pred (CODE, e.vector_mode ()));
  }
};

/* Implements vadc.  */
class vadc : public function_base
{
public:
  bool apply_mask_policy_p () const override { return false; }
  bool use_mask_predication_p () const override { return false; }

  rtx expand (function_expander &e) const override
  {
    switch (e.op_info->op)
      {
      case OP_TYPE_vvm:
	return e.use_exact_insn (code_for_pred_adc (e.vector_mode ()));
      case OP_TYPE_vxm:
	return e.use_exact_insn (code_for_pred_adc_scalar (e.vector_mode ()));
      default:
	gcc_unreachable ();
      }
  }
};

/* Implements vsbc.  */
class vsbc : public function_base
{
public:
  bool apply_mask_policy_p () const override { return false; }
  bool use_mask_predication_p () const override { return false; }

  rtx expand (function_expander &e) const override
  {
    switch (e.op_info->op)
      {
      case OP_TYPE_vvm:
	return e.use_exact_insn (code_for_pred_sbc (e.vector_mode ()));
      case OP_TYPE_vxm:
	return e.use_exact_insn (code_for_pred_sbc_scalar (e.vector_mode ()));
      default:
	gcc_unreachable ();
      }
  }
};

/* Implements vmadc.  */
class vmadc : public function_base
{
public:
  bool apply_tail_policy_p () const override { return false; }
  bool apply_mask_policy_p () const override { return false; }
  bool use_mask_predication_p () const override { return false; }
  bool has_merge_operand_p () const override { return false; }

  rtx expand (function_expander &e) const override
  {
    switch (e.op_info->op)
      {
      case OP_TYPE_vvm:
	return e.use_exact_insn (code_for_pred_madc (e.vector_mode ()));
      case OP_TYPE_vxm:
	return e.use_exact_insn (code_for_pred_madc_scalar (e.vector_mode ()));
      case OP_TYPE_vv:
	return e.use_exact_insn (
	  code_for_pred_madc_overflow (e.vector_mode ()));
      case OP_TYPE_vx:
	return e.use_exact_insn (
	  code_for_pred_madc_overflow_scalar (e.vector_mode ()));
      default:
	gcc_unreachable ();
      }
  }
};

/* Implements vmsbc.  */
class vmsbc : public function_base
{
public:
  bool apply_tail_policy_p () const override { return false; }
  bool apply_mask_policy_p () const override { return false; }
  bool use_mask_predication_p () const override { return false; }
  bool has_merge_operand_p () const override { return false; }

  rtx expand (function_expander &e) const override
  {
    switch (e.op_info->op)
      {
      case OP_TYPE_vvm:
	return e.use_exact_insn (code_for_pred_msbc (e.vector_mode ()));
      case OP_TYPE_vxm:
	return e.use_exact_insn (code_for_pred_msbc_scalar (e.vector_mode ()));
      case OP_TYPE_vv:
	return e.use_exact_insn (
	  code_for_pred_msbc_overflow (e.vector_mode ()));
      case OP_TYPE_vx:
	return e.use_exact_insn (
	  code_for_pred_msbc_overflow_scalar (e.vector_mode ()));
      default:
	gcc_unreachable ();
      }
  }
};

/* Implements vnsrl/vnsra.  */
template<rtx_code CODE>
class vnshift : public function_base
{
public:
  rtx expand (function_expander &e) const override
  {
    switch (e.op_info->op)
      {
      case OP_TYPE_wx:
	return e.use_exact_insn (
	  code_for_pred_narrow_scalar (CODE, e.vector_mode ()));
      case OP_TYPE_wv:
	return e.use_exact_insn (code_for_pred_narrow (CODE, e.vector_mode ()));
      default:
	gcc_unreachable ();
      }
  }
};

/* Implements vncvt.  */
class vncvt_x : public function_base
{
public:
  rtx expand (function_expander &e) const override
  {
    return e.use_exact_insn (code_for_pred_trunc (e.vector_mode ()));
  }
};

/* Implements vmerge.  */
class vmerge : public function_base
{
public:
  bool apply_mask_policy_p () const override { return false; }
  bool use_mask_predication_p () const override { return false; }
  rtx expand (function_expander &e) const override
  {
    switch (e.op_info->op)
      {
      case OP_TYPE_vvm:
	return e.use_exact_insn (code_for_pred_merge (e.vector_mode ()));
      case OP_TYPE_vxm:
	return e.use_exact_insn (code_for_pred_merge_scalar (e.vector_mode ()));
      default:
	gcc_unreachable ();
      }
  }
};

/* Implements vmv.v.x/vmv.v.v.  */
class vmv_v : public function_base
{
public:
  rtx expand (function_expander &e) const override
  {
    switch (e.op_info->op)
      {
      case OP_TYPE_v:
	return e.use_exact_insn (code_for_pred_mov (e.vector_mode ()));
      case OP_TYPE_x:
	return e.use_exact_insn (code_for_pred_broadcast (e.vector_mode ()));
      default:
	gcc_unreachable ();
      }
  }
};

/* Implements vaadd/vasub/vsmul/vssra/vssrl.  */
template<int UNSPEC>
class sat_op : public function_base
{
public:
  rtx expand (function_expander &e) const override
  {
    switch (e.op_info->op)
      {
      case OP_TYPE_vx:
	return e.use_exact_insn (
	  code_for_pred_scalar (UNSPEC, e.vector_mode ()));
      case OP_TYPE_vv:
	return e.use_exact_insn (code_for_pred (UNSPEC, e.vector_mode ()));
      default:
	gcc_unreachable ();
      }
  }
};

/* Implements vnclip/vnclipu.  */
template<int UNSPEC>
class vnclip : public function_base
{
public:
  rtx expand (function_expander &e) const override
  {
    switch (e.op_info->op)
      {
      case OP_TYPE_wx:
	return e.use_exact_insn (
	  code_for_pred_narrow_clip_scalar (UNSPEC, e.vector_mode ()));
      case OP_TYPE_wv:
	return e.use_exact_insn (
	  code_for_pred_narrow_clip (UNSPEC, e.vector_mode ()));
      default:
	gcc_unreachable ();
      }
  }
};

/* Implements vmseq/vmsne/vmslt/vmsgt/vmsle/vmsge.  */
template<rtx_code CODE>
class icmp : public function_base
{
public:
  rtx expand (function_expander &e) const override
  {
    switch (e.op_info->op)
      {
	case OP_TYPE_vx: {
	  if (CODE == GE || CODE == GEU)
	    return e.use_compare_insn (CODE, code_for_pred_ge_scalar (
					       e.vector_mode ()));
	  else if (CODE == EQ || CODE == NE)
	    return e.use_compare_insn (CODE, code_for_pred_eqne_scalar (
					       e.vector_mode ()));
	  else
	    return e.use_compare_insn (CODE, code_for_pred_cmp_scalar (
					       e.vector_mode ()));
	}
	case OP_TYPE_vv: {
	  if (CODE == LT || CODE == LTU || CODE == GE || CODE == GEU)
	    return e.use_compare_insn (CODE,
				       code_for_pred_ltge (e.vector_mode ()));
	  else
	    return e.use_compare_insn (CODE,
				       code_for_pred_cmp (e.vector_mode ()));
	}
      default:
	gcc_unreachable ();
      }
  }
};

/* Enumerates types of ternary operations.
   We have 2 types ternop:
     - 1. accumulator is vd:
        vmacc.vv vd,vs1,vs2 # vd = vs1 * vs2 + vd.
     - 2. accumulator is vs2:
        vmadd.vv vd,vs1,vs2 # vd = vs1 * vd + vs2.  */
enum ternop_type
{
  TERNOP_VMACC,
  TERNOP_VNMSAC,
  TERNOP_VMADD,
  TERNOP_VNMSUB,
};

/* Implements vmacc/vnmsac/vmadd/vnmsub.  */
template<ternop_type TERNOP_TYPE>
class imac : public function_base
{
public:
  bool has_merge_operand_p () const override { return false; }

  rtx expand (function_expander &e) const override
  {
    switch (TERNOP_TYPE)
      {
      case TERNOP_VMACC:
	if (e.op_info->op == OP_TYPE_vx)
	  return e.use_ternop_insn (
	    true, code_for_pred_mul_scalar (PLUS, e.vector_mode ()));
	if (e.op_info->op == OP_TYPE_vv)
	  return e.use_ternop_insn (true,
				    code_for_pred_mul (PLUS, e.vector_mode ()));
	break;
      case TERNOP_VNMSAC:
	if (e.op_info->op == OP_TYPE_vx)
	  return e.use_ternop_insn (
	    true, code_for_pred_mul_scalar (MINUS, e.vector_mode ()));
	if (e.op_info->op == OP_TYPE_vv)
	  return e.use_ternop_insn (true, code_for_pred_mul (MINUS,
							     e.vector_mode ()));
	break;
      case TERNOP_VMADD:
	if (e.op_info->op == OP_TYPE_vx)
	  return e.use_ternop_insn (
	    false, code_for_pred_mul_scalar (PLUS, e.vector_mode ()));
	if (e.op_info->op == OP_TYPE_vv)
	  return e.use_ternop_insn (false,
				    code_for_pred_mul (PLUS, e.vector_mode ()));
	break;
      case TERNOP_VNMSUB:
	if (e.op_info->op == OP_TYPE_vx)
	  return e.use_ternop_insn (
	    false, code_for_pred_mul_scalar (MINUS, e.vector_mode ()));
	if (e.op_info->op == OP_TYPE_vv)
	  return e.use_ternop_insn (false,
				    code_for_pred_mul (MINUS,
						       e.vector_mode ()));
	break;
      default:
	break;
      }
    gcc_unreachable ();
  }
};

/* Enumerates types of widen ternary operations.
   We have 4 types ternop:
     - 1. vwmacc.
     - 2. vwmaccu.
     - 3. vwmaccsu.
     - 4. vwmaccus.  */
enum widen_ternop_type
{
  WIDEN_TERNOP_VWMACC,
  WIDEN_TERNOP_VWMACCU,
  WIDEN_TERNOP_VWMACCSU,
  WIDEN_TERNOP_VWMACCUS,
};

/* Implements vwmacc<su><su>.  */
template<widen_ternop_type WIDEN_TERNOP_TYPE>
class iwmac : public function_base
{
public:
  bool has_merge_operand_p () const override { return false; }

  rtx expand (function_expander &e) const override
  {
    switch (WIDEN_TERNOP_TYPE)
      {
      case WIDEN_TERNOP_VWMACC:
	if (e.op_info->op == OP_TYPE_vx)
	  return e.use_widen_ternop_insn (
	    code_for_pred_widen_mul_plus_scalar (SIGN_EXTEND,
						 e.vector_mode ()));
	if (e.op_info->op == OP_TYPE_vv)
	  return e.use_widen_ternop_insn (
	    code_for_pred_widen_mul_plus (SIGN_EXTEND, e.vector_mode ()));
	break;
      case WIDEN_TERNOP_VWMACCU:
	if (e.op_info->op == OP_TYPE_vx)
	  return e.use_widen_ternop_insn (
	    code_for_pred_widen_mul_plus_scalar (ZERO_EXTEND,
						 e.vector_mode ()));
	if (e.op_info->op == OP_TYPE_vv)
	  return e.use_widen_ternop_insn (
	    code_for_pred_widen_mul_plus (ZERO_EXTEND, e.vector_mode ()));
	break;
      case WIDEN_TERNOP_VWMACCSU:
	if (e.op_info->op == OP_TYPE_vx)
	  return e.use_widen_ternop_insn (
	    code_for_pred_widen_mul_plussu_scalar (e.vector_mode ()));
	if (e.op_info->op == OP_TYPE_vv)
	  return e.use_widen_ternop_insn (
	    code_for_pred_widen_mul_plussu (e.vector_mode ()));
	break;
      case WIDEN_TERNOP_VWMACCUS:
	return e.use_widen_ternop_insn (
	  code_for_pred_widen_mul_plusus_scalar (e.vector_mode ()));
      default:
	break;
      }
    gcc_unreachable ();
  }
};

/* Implements vmand/vmnand/vmandn/vmxor/vmor/vmnor/vmorn/vmxnor  */
template<rtx_code CODE>
class mask_logic : public function_base
{
public:
  bool apply_tail_policy_p () const override { return false; }
  bool apply_mask_policy_p () const override { return false; }

  rtx expand (function_expander &e) const override
  {
    return e.use_exact_insn (code_for_pred (CODE, e.vector_mode ()));
  }
};
template<rtx_code CODE>
class mask_nlogic : public function_base
{
public:
  bool apply_tail_policy_p () const override { return false; }
  bool apply_mask_policy_p () const override { return false; }

  rtx expand (function_expander &e) const override
  {
    return e.use_exact_insn (code_for_pred_n (CODE, e.vector_mode ()));
  }
};
template<rtx_code CODE>
class mask_notlogic : public function_base
{
public:
  bool apply_tail_policy_p () const override { return false; }
  bool apply_mask_policy_p () const override { return false; }

  rtx expand (function_expander &e) const override
  {
    return e.use_exact_insn (code_for_pred_not (CODE, e.vector_mode ()));
  }
};

/* Implements vmmv.  */
class vmmv : public function_base
{
public:
  bool apply_tail_policy_p () const override { return false; }
  bool apply_mask_policy_p () const override { return false; }

  rtx expand (function_expander &e) const override
  {
    return e.use_exact_insn (code_for_pred_mov (e.vector_mode ()));
  }
};

/* Implements vmclr.  */
class vmclr : public function_base
{
public:
  bool can_be_overloaded_p (enum predication_type_index) const override
  {
    return false;
  }

  rtx expand (function_expander &e) const override
  {
    machine_mode mode = TYPE_MODE (TREE_TYPE (e.exp));
    e.add_all_one_mask_operand (mode);
    e.add_vundef_operand (mode);
    e.add_input_operand (mode, CONST0_RTX (mode));
    e.add_input_operand (call_expr_nargs (e.exp) - 1);
    e.add_input_operand (Pmode, get_avl_type_rtx (avl_type::NONVLMAX));
    return e.generate_insn (code_for_pred_mov (e.vector_mode ()));
  }
};

/* Implements vmset.  */
class vmset : public function_base
{
public:
  bool can_be_overloaded_p (enum predication_type_index) const override
  {
    return false;
  }

  rtx expand (function_expander &e) const override
  {
    machine_mode mode = TYPE_MODE (TREE_TYPE (e.exp));
    e.add_all_one_mask_operand (mode);
    e.add_vundef_operand (mode);
    e.add_input_operand (mode, CONSTM1_RTX (mode));
    e.add_input_operand (call_expr_nargs (e.exp) - 1);
    e.add_input_operand (Pmode, get_avl_type_rtx (avl_type::NONVLMAX));
    return e.generate_insn (code_for_pred_mov (e.vector_mode ()));
  }
};

/* Implements vmnot.  */
class vmnot : public function_base
{
public:
  bool apply_tail_policy_p () const override { return false; }
  bool apply_mask_policy_p () const override { return false; }

  rtx expand (function_expander &e) const override
  {
    return e.use_exact_insn (code_for_pred_not (e.vector_mode ()));
  }
};

/* Implements vcpop.  */
class vcpop : public function_base
{
public:
  bool apply_tail_policy_p () const override { return false; }
  bool apply_mask_policy_p () const override { return false; }
  bool has_merge_operand_p () const override { return false; }

  rtx expand (function_expander &e) const override
  {
    return e.use_exact_insn (code_for_pred_popcount (e.vector_mode (), Pmode));
  }
};

/* Implements vfirst.  */
class vfirst : public function_base
{
public:
  bool apply_tail_policy_p () const override { return false; }
  bool apply_mask_policy_p () const override { return false; }
  bool has_merge_operand_p () const override { return false; }

  rtx expand (function_expander &e) const override
  {
    return e.use_exact_insn (code_for_pred_ffs (e.vector_mode (), Pmode));
  }
};

/* Implements vmsbf/vmsif/vmsof.  */
template<int UNSPEC>
class mask_misc : public function_base
{
public:
  bool apply_tail_policy_p () const override { return false; }

  rtx expand (function_expander &e) const override
  {
    return e.use_exact_insn (code_for_pred (UNSPEC, e.vector_mode ()));
  }
};

/* Implements viota.  */
class viota : public function_base
{
public:
  bool can_be_overloaded_p (enum predication_type_index pred) const override
  {
    return pred == PRED_TYPE_tu || pred == PRED_TYPE_tum
	   || pred == PRED_TYPE_tumu;
  }

  rtx expand (function_expander &e) const override
  {
    return e.use_exact_insn (code_for_pred_iota (e.vector_mode ()));
  }
};

/* Implements vid.  */
class vid : public function_base
{
public:
  bool can_be_overloaded_p (enum predication_type_index pred) const override
  {
    return pred == PRED_TYPE_tu || pred == PRED_TYPE_tum
	   || pred == PRED_TYPE_tumu;
  }

  rtx expand (function_expander &e) const override
  {
    return e.use_exact_insn (code_for_pred_series (e.vector_mode ()));
  }
};

static CONSTEXPR const vsetvl<false> vsetvl_obj;
static CONSTEXPR const vsetvl<true> vsetvlmax_obj;
static CONSTEXPR const loadstore<false, LST_UNIT_STRIDE, false> vle_obj;
static CONSTEXPR const loadstore<true, LST_UNIT_STRIDE, false> vse_obj;
static CONSTEXPR const loadstore<false, LST_UNIT_STRIDE, false> vlm_obj;
static CONSTEXPR const loadstore<true, LST_UNIT_STRIDE, false> vsm_obj;
static CONSTEXPR const loadstore<false, LST_STRIDED, false> vlse_obj;
static CONSTEXPR const loadstore<true, LST_STRIDED, false> vsse_obj;
static CONSTEXPR const loadstore<false, LST_INDEXED, false> vluxei8_obj;
static CONSTEXPR const loadstore<false, LST_INDEXED, false> vluxei16_obj;
static CONSTEXPR const loadstore<false, LST_INDEXED, false> vluxei32_obj;
static CONSTEXPR const loadstore<false, LST_INDEXED, false> vluxei64_obj;
static CONSTEXPR const loadstore<false, LST_INDEXED, true> vloxei8_obj;
static CONSTEXPR const loadstore<false, LST_INDEXED, true> vloxei16_obj;
static CONSTEXPR const loadstore<false, LST_INDEXED, true> vloxei32_obj;
static CONSTEXPR const loadstore<false, LST_INDEXED, true> vloxei64_obj;
static CONSTEXPR const loadstore<true, LST_INDEXED, false> vsuxei8_obj;
static CONSTEXPR const loadstore<true, LST_INDEXED, false> vsuxei16_obj;
static CONSTEXPR const loadstore<true, LST_INDEXED, false> vsuxei32_obj;
static CONSTEXPR const loadstore<true, LST_INDEXED, false> vsuxei64_obj;
static CONSTEXPR const loadstore<true, LST_INDEXED, true> vsoxei8_obj;
static CONSTEXPR const loadstore<true, LST_INDEXED, true> vsoxei16_obj;
static CONSTEXPR const loadstore<true, LST_INDEXED, true> vsoxei32_obj;
static CONSTEXPR const loadstore<true, LST_INDEXED, true> vsoxei64_obj;
static CONSTEXPR const binop<PLUS> vadd_obj;
static CONSTEXPR const binop<MINUS> vsub_obj;
static CONSTEXPR const vrsub vrsub_obj;
static CONSTEXPR const binop<AND> vand_obj;
static CONSTEXPR const binop<IOR> vor_obj;
static CONSTEXPR const binop<XOR> vxor_obj;
static CONSTEXPR const binop<ASHIFT> vsll_obj;
static CONSTEXPR const binop<ASHIFTRT> vsra_obj;
static CONSTEXPR const binop<LSHIFTRT> vsrl_obj;
static CONSTEXPR const binop<SMIN> vmin_obj;
static CONSTEXPR const binop<SMAX> vmax_obj;
static CONSTEXPR const binop<UMIN> vminu_obj;
static CONSTEXPR const binop<UMAX> vmaxu_obj;
static CONSTEXPR const binop<MULT> vmul_obj;
static CONSTEXPR const vmulh<UNSPEC_VMULHS> vmulh_obj;
static CONSTEXPR const vmulh<UNSPEC_VMULHU> vmulhu_obj;
static CONSTEXPR const vmulh<UNSPEC_VMULHSU> vmulhsu_obj;
static CONSTEXPR const binop<DIV> vdiv_obj;
static CONSTEXPR const binop<MOD> vrem_obj;
static CONSTEXPR const binop<UDIV> vdivu_obj;
static CONSTEXPR const binop<UMOD> vremu_obj;
static CONSTEXPR const unop<NEG> vneg_obj;
static CONSTEXPR const unop<NOT> vnot_obj;
static CONSTEXPR const ext<SIGN_EXTEND> vsext_obj;
static CONSTEXPR const ext<ZERO_EXTEND> vzext_obj;
static CONSTEXPR const widen_binop<PLUS, SIGN_EXTEND>vwadd_obj;
static CONSTEXPR const widen_binop<MINUS, SIGN_EXTEND>vwsub_obj;
static CONSTEXPR const widen_binop<MULT, SIGN_EXTEND>vwmul_obj;
static CONSTEXPR const widen_binop<PLUS, ZERO_EXTEND>vwaddu_obj;
static CONSTEXPR const widen_binop<MINUS, ZERO_EXTEND>vwsubu_obj;
static CONSTEXPR const widen_binop<MULT, ZERO_EXTEND>vwmulu_obj;
static CONSTEXPR const vwmulsu vwmulsu_obj;
static CONSTEXPR const vwcvt<SIGN_EXTEND> vwcvt_x_obj;
static CONSTEXPR const vwcvt<ZERO_EXTEND> vwcvtu_x_obj;
static CONSTEXPR const vadc vadc_obj;
static CONSTEXPR const vsbc vsbc_obj;
static CONSTEXPR const vmadc vmadc_obj;
static CONSTEXPR const vmsbc vmsbc_obj;
static CONSTEXPR const vnshift<LSHIFTRT> vnsrl_obj;
static CONSTEXPR const vnshift<ASHIFTRT> vnsra_obj;
static CONSTEXPR const vncvt_x vncvt_x_obj;
static CONSTEXPR const vmerge vmerge_obj;
static CONSTEXPR const vmv_v vmv_v_obj;
static CONSTEXPR const icmp<EQ> vmseq_obj;
static CONSTEXPR const icmp<NE> vmsne_obj;
static CONSTEXPR const icmp<LT> vmslt_obj;
static CONSTEXPR const icmp<GT> vmsgt_obj;
static CONSTEXPR const icmp<LE> vmsle_obj;
static CONSTEXPR const icmp<GE> vmsge_obj;
static CONSTEXPR const icmp<LTU> vmsltu_obj;
static CONSTEXPR const icmp<GTU> vmsgtu_obj;
static CONSTEXPR const icmp<LEU> vmsleu_obj;
static CONSTEXPR const icmp<GEU> vmsgeu_obj;
static CONSTEXPR const imac<TERNOP_VMACC> vmacc_obj;
static CONSTEXPR const imac<TERNOP_VNMSAC> vnmsac_obj;
static CONSTEXPR const imac<TERNOP_VMADD> vmadd_obj;
static CONSTEXPR const imac<TERNOP_VNMSUB> vnmsub_obj;
static CONSTEXPR const iwmac<WIDEN_TERNOP_VWMACC> vwmacc_obj;
static CONSTEXPR const iwmac<WIDEN_TERNOP_VWMACCU> vwmaccu_obj;
static CONSTEXPR const iwmac<WIDEN_TERNOP_VWMACCSU> vwmaccsu_obj;
static CONSTEXPR const iwmac<WIDEN_TERNOP_VWMACCUS> vwmaccus_obj;
static CONSTEXPR const binop<SS_PLUS> vsadd_obj;
static CONSTEXPR const binop<SS_MINUS> vssub_obj;
static CONSTEXPR const binop<US_PLUS> vsaddu_obj;
static CONSTEXPR const binop<US_MINUS> vssubu_obj;
static CONSTEXPR const sat_op<UNSPEC_VAADDU> vaaddu_obj;
static CONSTEXPR const sat_op<UNSPEC_VAADD> vaadd_obj;
static CONSTEXPR const sat_op<UNSPEC_VASUBU> vasubu_obj;
static CONSTEXPR const sat_op<UNSPEC_VASUB> vasub_obj;
static CONSTEXPR const sat_op<UNSPEC_VSMUL> vsmul_obj;
static CONSTEXPR const sat_op<UNSPEC_VSSRL> vssrl_obj;
static CONSTEXPR const sat_op<UNSPEC_VSSRA> vssra_obj;
static CONSTEXPR const vnclip<UNSPEC_VNCLIP> vnclip_obj;
static CONSTEXPR const vnclip<UNSPEC_VNCLIPU> vnclipu_obj;
static CONSTEXPR const mask_logic<AND> vmand_obj;
static CONSTEXPR const mask_nlogic<AND> vmnand_obj;
static CONSTEXPR const mask_notlogic<AND> vmandn_obj;
static CONSTEXPR const mask_logic<XOR> vmxor_obj;
static CONSTEXPR const mask_logic<IOR> vmor_obj;
static CONSTEXPR const mask_nlogic<IOR> vmnor_obj;
static CONSTEXPR const mask_notlogic<IOR> vmorn_obj;
static CONSTEXPR const mask_nlogic<XOR> vmxnor_obj;
static CONSTEXPR const vmmv vmmv_obj;
static CONSTEXPR const vmclr vmclr_obj;
static CONSTEXPR const vmset vmset_obj;
static CONSTEXPR const vmnot vmnot_obj;
static CONSTEXPR const vcpop vcpop_obj;
static CONSTEXPR const vfirst vfirst_obj;
static CONSTEXPR const mask_misc<UNSPEC_VMSBF> vmsbf_obj;
static CONSTEXPR const mask_misc<UNSPEC_VMSIF> vmsif_obj;
static CONSTEXPR const mask_misc<UNSPEC_VMSOF> vmsof_obj;
static CONSTEXPR const viota viota_obj;
static CONSTEXPR const vid vid_obj;

/* Declare the function base NAME, pointing it to an instance
   of class <NAME>_obj.  */
#define BASE(NAME) \
  namespace bases { const function_base *const NAME = &NAME##_obj; }

BASE (vsetvl)
BASE (vsetvlmax)
BASE (vle)
BASE (vse)
BASE (vlm)
BASE (vsm)
BASE (vlse)
BASE (vsse)
BASE (vluxei8)
BASE (vluxei16)
BASE (vluxei32)
BASE (vluxei64)
BASE (vloxei8)
BASE (vloxei16)
BASE (vloxei32)
BASE (vloxei64)
BASE (vsuxei8)
BASE (vsuxei16)
BASE (vsuxei32)
BASE (vsuxei64)
BASE (vsoxei8)
BASE (vsoxei16)
BASE (vsoxei32)
BASE (vsoxei64)
BASE (vadd)
BASE (vsub)
BASE (vrsub)
BASE (vand)
BASE (vor)
BASE (vxor)
BASE (vsll)
BASE (vsra)
BASE (vsrl)
BASE (vmin)
BASE (vmax)
BASE (vminu)
BASE (vmaxu)
BASE (vmul)
BASE (vmulh)
BASE (vmulhu)
BASE (vmulhsu)
BASE (vdiv)
BASE (vrem)
BASE (vdivu)
BASE (vremu)
BASE (vneg)
BASE (vnot)
BASE (vsext)
BASE (vzext)
BASE (vwadd)
BASE (vwsub)
BASE (vwmul)
BASE (vwaddu)
BASE (vwsubu)
BASE (vwmulu)
BASE (vwmulsu)
BASE (vwcvt_x)
BASE (vwcvtu_x)
BASE (vadc)
BASE (vsbc)
BASE (vmadc)
BASE (vmsbc)
BASE (vnsrl)
BASE (vnsra)
BASE (vncvt_x)
BASE (vmerge)
BASE (vmv_v)
BASE (vmseq)
BASE (vmsne)
BASE (vmslt)
BASE (vmsgt)
BASE (vmsle)
BASE (vmsge)
BASE (vmsltu)
BASE (vmsgtu)
BASE (vmsleu)
BASE (vmsgeu)
BASE (vmacc)
BASE (vnmsac)
BASE (vmadd)
BASE (vnmsub)
BASE (vwmacc)
BASE (vwmaccu)
BASE (vwmaccsu)
BASE (vwmaccus)
BASE (vsadd)
BASE (vssub)
BASE (vsaddu)
BASE (vssubu)
BASE (vaadd)
BASE (vasub)
BASE (vaaddu)
BASE (vasubu)
BASE (vsmul)
BASE (vssra)
BASE (vssrl)
BASE (vnclip)
BASE (vnclipu)
BASE (vmand)
BASE (vmnand)
BASE (vmandn)
BASE (vmxor)
BASE (vmor)
BASE (vmnor)
BASE (vmorn)
BASE (vmxnor)
BASE (vmmv)
BASE (vmclr)
BASE (vmset)
BASE (vmnot)
BASE (vcpop)
BASE (vfirst)
BASE (vmsbf)
BASE (vmsif)
BASE (vmsof)
BASE (viota)
BASE (vid)

} // end namespace riscv_vector
