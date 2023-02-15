/* Subroutines used for code generation for RISC-V 'V' Extension for
   GNU compiler.
   Copyright (C) 2022-2023 Free Software Foundation, Inc.
   Contributed by Juzhe Zhong (juzhe.zhong@rivai.ai), RiVAI Technologies Ltd.

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

#define IN_TARGET_CODE 1

#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "tm.h"
#include "backend.h"
#include "rtl.h"
#include "insn-config.h"
#include "insn-attr.h"
#include "recog.h"
#include "alias.h"
#include "tree.h"
#include "stringpool.h"
#include "attribs.h"
#include "explow.h"
#include "memmodel.h"
#include "emit-rtl.h"
#include "tm_p.h"
#include "target.h"
#include "expr.h"
#include "optabs.h"
#include "tm-constrs.h"

using namespace riscv_vector;

namespace riscv_vector {

template <int MAX_OPERANDS> class insn_expander
{
public:
  insn_expander () : m_opno (0) {}
  void add_output_operand (rtx x, machine_mode mode)
  {
    create_output_operand (&m_ops[m_opno++], x, mode);
    gcc_assert (m_opno <= MAX_OPERANDS);
  }
  void add_input_operand (rtx x, machine_mode mode)
  {
    create_input_operand (&m_ops[m_opno++], x, mode);
    gcc_assert (m_opno <= MAX_OPERANDS);
  }
  void add_all_one_mask_operand (machine_mode mode)
  {
    add_input_operand (CONSTM1_RTX (mode), mode);
  }
  void add_vundef_operand (machine_mode mode)
  {
    add_input_operand (gen_rtx_UNSPEC (mode, gen_rtvec (1, const0_rtx),
				       UNSPEC_VUNDEF),
		       mode);
  }
  void add_policy_operand (enum tail_policy vta, enum mask_policy vma)
  {
    rtx tail_policy_rtx = gen_int_mode (vta, Pmode);
    rtx mask_policy_rtx = gen_int_mode (vma, Pmode);
    add_input_operand (tail_policy_rtx, Pmode);
    add_input_operand (mask_policy_rtx, Pmode);
  }
  void add_avl_type_operand ()
  {
    add_input_operand (get_avl_type_rtx (avl_type::VLMAX), Pmode);
  }

  void expand (enum insn_code icode, bool temporary_volatile_p = false)
  {
    if (temporary_volatile_p)
      {
	temporary_volatile_ok v (true);
	expand_insn (icode, m_opno, m_ops);
      }
    else
      expand_insn (icode, m_opno, m_ops);
  }

private:
  int m_opno;
  expand_operand m_ops[MAX_OPERANDS];
};

/* Return true if X is a const_vector with all duplicate elements, which is in
   the range between MINVAL and MAXVAL.  */
bool
const_vec_all_same_in_range_p (rtx x, HOST_WIDE_INT minval,
			       HOST_WIDE_INT maxval)
{
  rtx elt;
  return (const_vec_duplicate_p (x, &elt) && CONST_INT_P (elt)
	  && IN_RANGE (INTVAL (elt), minval, maxval));
}

static rtx
emit_vlmax_vsetvl (machine_mode vmode)
{
  rtx vl = gen_reg_rtx (Pmode);
  unsigned int sew = GET_MODE_CLASS (vmode) == MODE_VECTOR_BOOL
		       ? 8
		       : GET_MODE_BITSIZE (GET_MODE_INNER (vmode));
  enum vlmul_type vlmul = get_vlmul (vmode);
  unsigned int ratio = calculate_ratio (sew, vlmul);

  if (!optimize)
    emit_insn (gen_vsetvl (Pmode, vl, RVV_VLMAX, gen_int_mode (sew, Pmode),
			   gen_int_mode (get_vlmul (vmode), Pmode), const0_rtx,
			   const0_rtx));
  else
    emit_insn (gen_vlmax_avl (Pmode, vl, gen_int_mode (ratio, Pmode)));

  return vl;
}

/* Calculate SEW/LMUL ratio.  */
unsigned int
calculate_ratio (unsigned int sew, enum vlmul_type vlmul)
{
  unsigned int ratio;
  switch (vlmul)
    {
    case LMUL_1:
      ratio = sew;
      break;
    case LMUL_2:
      ratio = sew / 2;
      break;
    case LMUL_4:
      ratio = sew / 4;
      break;
    case LMUL_8:
      ratio = sew / 8;
      break;
    case LMUL_F8:
      ratio = sew * 8;
      break;
    case LMUL_F4:
      ratio = sew * 4;
      break;
    case LMUL_F2:
      ratio = sew * 2;
      break;
    default:
      gcc_unreachable ();
    }
  return ratio;
}

/* Emit an RVV unmask && vl mov from SRC to DEST.  */
void
emit_pred_op (unsigned icode, rtx dest, rtx src, machine_mode mask_mode)
{
  insn_expander<8> e;
  machine_mode mode = GET_MODE (dest);

  e.add_output_operand (dest, mode);
  e.add_all_one_mask_operand (mask_mode);
  e.add_vundef_operand (mode);

  e.add_input_operand (src, GET_MODE (src));

  rtx vlmax = emit_vlmax_vsetvl (mode);
  e.add_input_operand (vlmax, Pmode);

  if (GET_MODE_CLASS (mode) != MODE_VECTOR_BOOL)
    e.add_policy_operand (get_prefer_tail_policy (), get_prefer_mask_policy ());

  e.add_avl_type_operand ();

  e.expand ((enum insn_code) icode, MEM_P (dest) || MEM_P (src));
}

static void
expand_const_vector (rtx target, rtx src, machine_mode mask_mode)
{
  machine_mode mode = GET_MODE (target);
  scalar_mode elt_mode = GET_MODE_INNER (mode);
  if (GET_MODE_CLASS (mode) == MODE_VECTOR_BOOL)
    {
      rtx elt;
      gcc_assert (
	const_vec_duplicate_p (src, &elt)
	&& (rtx_equal_p (elt, const0_rtx) || rtx_equal_p (elt, const1_rtx)));
      emit_pred_op (code_for_pred_mov (mode), target, src, mode);
      return;
    }

  rtx elt;
  if (const_vec_duplicate_p (src, &elt))
    {
      rtx tmp = register_operand (target, mode) ? target : gen_reg_rtx (mode);
      /* Element in range -16 ~ 15 integer or 0.0 floating-point,
	 we use vmv.v.i instruction.  */
      if (satisfies_constraint_vi (src) || satisfies_constraint_Wc0 (src))
	emit_pred_op (code_for_pred_mov (mode), tmp, src, mask_mode);
      else
	emit_pred_op (code_for_pred_broadcast (mode), tmp,
		      force_reg (elt_mode, elt), mask_mode);

      if (tmp != target)
	emit_move_insn (target, tmp);
      return;
    }

  /* TODO: We only support const duplicate vector for now. More cases
     will be supported when we support auto-vectorization:

       1. series vector.
       2. multiple elts duplicate vector.
       3. multiple patterns with multiple elts.  */
}

/* Expand a pre-RA RVV data move from SRC to DEST.
   It expands move for RVV fractional vector modes.  */
bool
legitimize_move (rtx dest, rtx src, machine_mode mask_mode)
{
  machine_mode mode = GET_MODE (dest);
  if (CONST_VECTOR_P (src))
    {
      expand_const_vector (dest, src, mask_mode);
      return true;
    }
  if (known_ge (GET_MODE_SIZE (mode), BYTES_PER_RISCV_VECTOR)
      && GET_MODE_CLASS (mode) != MODE_VECTOR_BOOL)
    {
      /* Need to force register if mem <- !reg.  */
      if (MEM_P (dest) && !REG_P (src))
	src = force_reg (mode, src);

      return false;
    }
  if (!register_operand (src, mode) && !register_operand (dest, mode))
    {
      rtx tmp = gen_reg_rtx (mode);
      if (MEM_P (src))
	emit_pred_op (code_for_pred_mov (mode), tmp, src, mask_mode);
      else
	emit_move_insn (tmp, src);
      src = tmp;
    }
  emit_pred_op (code_for_pred_mov (mode), dest, src, mask_mode);
  return true;
}

/* VTYPE information for machine_mode.  */
struct mode_vtype_group
{
  enum vlmul_type vlmul_for_min_vlen32[NUM_MACHINE_MODES];
  uint8_t ratio_for_min_vlen32[NUM_MACHINE_MODES];
  enum vlmul_type vlmul_for_min_vlen64[NUM_MACHINE_MODES];
  uint8_t ratio_for_min_vlen64[NUM_MACHINE_MODES];
  mode_vtype_group ()
  {
#define ENTRY(MODE, REQUIREMENT, VLMUL_FOR_MIN_VLEN32, RATIO_FOR_MIN_VLEN32,   \
	      VLMUL_FOR_MIN_VLEN64, RATIO_FOR_MIN_VLEN64)                      \
  vlmul_for_min_vlen32[MODE##mode] = VLMUL_FOR_MIN_VLEN32;                     \
  ratio_for_min_vlen32[MODE##mode] = RATIO_FOR_MIN_VLEN32;                     \
  vlmul_for_min_vlen64[MODE##mode] = VLMUL_FOR_MIN_VLEN64;                     \
  ratio_for_min_vlen64[MODE##mode] = RATIO_FOR_MIN_VLEN64;
#include "riscv-vector-switch.def"
  }
};

static mode_vtype_group mode_vtype_infos;

/* Get vlmul field value by comparing LMUL with BYTES_PER_RISCV_VECTOR.  */
enum vlmul_type
get_vlmul (machine_mode mode)
{
  if (TARGET_MIN_VLEN == 32)
    return mode_vtype_infos.vlmul_for_min_vlen32[mode];
  else
    return mode_vtype_infos.vlmul_for_min_vlen64[mode];
}

/* Get ratio according to machine mode.  */
unsigned int
get_ratio (machine_mode mode)
{
  if (TARGET_MIN_VLEN == 32)
    return mode_vtype_infos.ratio_for_min_vlen32[mode];
  else
    return mode_vtype_infos.ratio_for_min_vlen64[mode];
}

/* Get ta according to operand[tail_op_idx].  */
int
get_ta (rtx ta)
{
  if (INTVAL (ta) == TAIL_ANY)
    return INVALID_ATTRIBUTE;
  return INTVAL (ta);
}

/* Get ma according to operand[mask_op_idx].  */
int
get_ma (rtx ma)
{
  if (INTVAL (ma) == MASK_ANY)
    return INVALID_ATTRIBUTE;
  return INTVAL (ma);
}

/* Get prefer tail policy.  */
enum tail_policy
get_prefer_tail_policy ()
{
  /* TODO: By default, we choose to use TAIL_ANY which allows
     compiler pick up either agnostic or undisturbed. Maybe we
     will have a compile option like -mprefer=agnostic to set
     this value???.  */
  return TAIL_ANY;
}

/* Get prefer mask policy.  */
enum mask_policy
get_prefer_mask_policy ()
{
  /* TODO: By default, we choose to use MASK_ANY which allows
     compiler pick up either agnostic or undisturbed. Maybe we
     will have a compile option like -mprefer=agnostic to set
     this value???.  */
  return MASK_ANY;
}

/* Get avl_type rtx.  */
rtx
get_avl_type_rtx (enum avl_type type)
{
  return gen_int_mode (type, Pmode);
}

/* Return the RVV vector mode that has NUNITS elements of mode INNER_MODE.
   This function is not only used by builtins, but also will be used by
   auto-vectorization in the future.  */
opt_machine_mode
get_vector_mode (scalar_mode inner_mode, poly_uint64 nunits)
{
  enum mode_class mclass;
  if (inner_mode == E_BImode)
    mclass = MODE_VECTOR_BOOL;
  else if (FLOAT_MODE_P (inner_mode))
    mclass = MODE_VECTOR_FLOAT;
  else
    mclass = MODE_VECTOR_INT;
  machine_mode mode;
  FOR_EACH_MODE_IN_CLASS (mode, mclass)
    if (inner_mode == GET_MODE_INNER (mode)
	&& known_eq (nunits, GET_MODE_NUNITS (mode))
	&& riscv_v_ext_vector_mode_p (mode))
      return mode;
  return opt_machine_mode ();
}

} // namespace riscv_vector
