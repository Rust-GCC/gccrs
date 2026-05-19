/* Subroutines used for instruction fusion for RISC-V.
   Copyright (C) 2026 Free Software Foundation, Inc.

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

#define IN_TARGET_CODE 1

#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "backend.h"
#include "target.h"
#include "rtl.h"
#include "regs.h"
#include "insn-config.h"
#include "recog.h"
#include "function.h"
#include "memmodel.h"
#include "emit-rtl.h"
#include "tm_p.h"
#include "riscv-protos.h"

/* Implement TARGET_SCHED_MACRO_FUSION_P.  Return true if target supports
   instruction fusion of some sort.  */

bool
riscv_macro_fusion_p (void)
{
  return riscv_get_fusible_ops () != RISCV_FUSE_NOTHING;
}

/* Return true iff the instruction fusion described by OP is enabled.  */

static bool
riscv_fusion_enabled_p (enum riscv_fusion_pairs op)
{
  return riscv_get_fusible_ops () & op;
}

/* Matches an add:
   (set (reg:DI rd) (plus:SI (reg:SI rs1) (reg:SI rs2))) */

static bool
riscv_set_is_add (rtx set)
{
  return (GET_CODE (SET_SRC (set)) == PLUS
	  && REG_P (XEXP (SET_SRC (set), 0))
	  && REG_P (XEXP (SET_SRC (set), 1))
	  && REG_P (SET_DEST (set)));
}

/* Matches an addi:
   (set (reg:DI rd) (plus:SI (reg:SI rs1) (const_int imm))) */

static bool
riscv_set_is_addi (rtx set)
{
  return (GET_CODE (SET_SRC (set)) == PLUS
	  && REG_P (XEXP (SET_SRC (set), 0))
	  && CONST_INT_P (XEXP (SET_SRC (set), 1))
	  && REG_P (SET_DEST (set)));
}

/* Matches an add.uw:
  (set (reg:DI rd)
    (plus:DI (zero_extend:DI (reg:SI rs1)) (reg:DI rs2))) */

static bool
riscv_set_is_adduw (rtx set)
{
  return (GET_CODE (SET_SRC (set)) == PLUS
	  && GET_CODE (XEXP (SET_SRC (set), 0)) == ZERO_EXTEND
	  && REG_P (XEXP (XEXP (SET_SRC (set), 0), 0))
	  && REG_P (XEXP (SET_SRC (set), 1))
	  && REG_P (SET_DEST (set)));
}

/* Matches a shNadd:
  (set (reg:DI rd)
       (plus:DI (ashift:DI (reg:DI rs1) (const_int N)) (reg:DI rS2)) */

static bool
riscv_set_is_shNadd (rtx set)
{
  return (GET_CODE (SET_SRC (set)) == PLUS
	  && GET_CODE (XEXP (SET_SRC (set), 0)) == ASHIFT
	  && REG_P (XEXP (XEXP (SET_SRC (set), 0), 0))
	  && CONST_INT_P (XEXP (XEXP (SET_SRC (set), 0), 1))
	  && (INTVAL (XEXP (XEXP (SET_SRC (set), 0), 1)) == 1
	      || INTVAL (XEXP (XEXP (SET_SRC (set), 0), 1)) == 2
	      || INTVAL (XEXP (XEXP (SET_SRC (set), 0), 1)) == 3)
	  && REG_P (SET_DEST (set)));
}

/* Matches a shNadd.uw:
  (set (reg:DI rd)
       (plus:DI (and:DI (ashift:DI (reg:DI rs1) (const_int N))
			(const_int N))
		(reg:DI rs2)) */

static bool
riscv_set_is_shNadduw (rtx set)
{
  return (GET_CODE (SET_SRC (set)) == PLUS
	  && GET_CODE (XEXP (SET_SRC (set), 0)) == AND
	  && GET_CODE (XEXP (XEXP (SET_SRC (set), 0), 0)) == ASHIFT
	  && REG_P (XEXP (XEXP (XEXP (SET_SRC (set), 0), 0), 0))
	  && CONST_INT_P (XEXP (XEXP (XEXP (SET_SRC (set), 0), 0), 1))
	  && (INTVAL (XEXP (XEXP (XEXP (SET_SRC (set), 0), 0), 1)) == 1
	      || INTVAL (XEXP (XEXP (XEXP (SET_SRC (set), 0), 0), 1)) == 2
	      || INTVAL (XEXP (XEXP (XEXP (SET_SRC (set), 0), 0), 1)) == 3)
	  && REG_P (SET_DEST (set)));
}

/* Implement TARGET_SCHED_MACRO_FUSION_PAIR_P.  Return true if PREV and CURR
   should be kept together during scheduling.  */

bool
riscv_macro_fusion_pair_p (rtx_insn *prev, rtx_insn *curr)
{
  /* If fusion is not enabled, then there's nothing to do.  */
  if (!riscv_macro_fusion_p ())
    return false;

  /* If PREV is already marked as fused, then we can't fuse CURR with PREV
     and if we were to fuse them we'd end up with a blob of insns that
     essentially are an atomic unit which is bad for scheduling.  */
  if (SCHED_GROUP_P (prev))
    return false;

  rtx prev_set = single_set (prev);
  rtx curr_set = single_set (curr);
  /* prev and curr are simple SET insns i.e. no flag setting or branching.  */
  bool simple_sets_p = prev_set && curr_set && !any_condjump_p (curr);
  bool sched1 = can_create_pseudo_p ();

  unsigned int prev_dest_regno = (prev_set && REG_P (SET_DEST (prev_set))
				  ? REGNO (SET_DEST (prev_set))
				  : FIRST_PSEUDO_REGISTER);
  unsigned int curr_dest_regno = (curr_set && REG_P (SET_DEST (curr_set))
				  ? REGNO (SET_DEST (curr_set))
				  : FIRST_PSEUDO_REGISTER);

  if (simple_sets_p
      && (riscv_fusion_enabled_p (RISCV_FUSE_ZEXTW)
	  || riscv_fusion_enabled_p (RISCV_FUSE_ZEXTWS))
      && (sched1 || prev_dest_regno == curr_dest_regno))
    {
      /* We are trying to match the following:
	   prev (slli) == (set (reg:DI rD)
			       (ashift:DI (reg:DI rS) (const_int 32)))
	   curr (slri) == (set (reg:DI rD)
			       (lshiftrt:DI (reg:DI rD) (const_int <shift>)))
	 with <shift> being either 32 for FUSE_ZEXTW, or
			 `less than 32 for FUSE_ZEXTWS. */

      if (GET_CODE (SET_SRC (prev_set)) == ASHIFT
	  && GET_CODE (SET_SRC (curr_set)) == LSHIFTRT
	  && REG_P (SET_DEST (prev_set))
	  && REG_P (SET_DEST (curr_set))
	  && REG_P (XEXP (SET_SRC (curr_set), 0))
	  && REGNO (XEXP (SET_SRC (curr_set), 0)) == curr_dest_regno
	  && CONST_INT_P (XEXP (SET_SRC (prev_set), 1))
	  && CONST_INT_P (XEXP (SET_SRC (curr_set), 1))
	  && INTVAL (XEXP (SET_SRC (prev_set), 1)) == 32
	  && ((INTVAL (XEXP (SET_SRC (curr_set), 1)) == 32
	       && riscv_fusion_enabled_p (RISCV_FUSE_ZEXTW) )
	      || (INTVAL (XEXP (SET_SRC (curr_set), 1)) < 32
		  && riscv_fusion_enabled_p (RISCV_FUSE_ZEXTWS))))
	{
	  if (dump_file)
	    fprintf (dump_file, "RISCV_FUSE_ZEXTWS\n");
	  return true;
	}
    }

  if (simple_sets_p && riscv_fusion_enabled_p (RISCV_FUSE_ZEXTH)
      && (sched1 || prev_dest_regno == curr_dest_regno))
    {
      /* We are trying to match the following:
	   prev (slli) == (set (reg:DI rD)
			       (ashift:DI (reg:DI rS) (const_int 48)))
	   curr (slri) == (set (reg:DI rD)
			       (lshiftrt:DI (reg:DI rD) (const_int 48))) */

      if (GET_CODE (SET_SRC (prev_set)) == ASHIFT
	  && GET_CODE (SET_SRC (curr_set)) == LSHIFTRT
	  && REG_P (SET_DEST (prev_set))
	  && REG_P (SET_DEST (curr_set))
	  && REG_P (XEXP (SET_SRC (curr_set), 0))
	  && REGNO (XEXP (SET_SRC (curr_set), 0)) == curr_dest_regno
	  && CONST_INT_P (XEXP (SET_SRC (prev_set), 1))
	  && CONST_INT_P (XEXP (SET_SRC (curr_set), 1))
	  && INTVAL (XEXP (SET_SRC (prev_set), 1)) == 48
	  && INTVAL (XEXP (SET_SRC (curr_set), 1)) == 48)
	{
	  if (dump_file)
	    fprintf (dump_file,"RISCV_FUSE_ZEXTH\n");
	  return true;
	}
    }

  if (simple_sets_p && riscv_fusion_enabled_p (RISCV_FUSE_LDINDEXED)
      && (sched1 || prev_dest_regno == curr_dest_regno))
    {
      /* We are trying to match the following:
	   prev (add) == (set (reg:DI rD)
			      (plus:DI (reg:DI rS1) (reg:DI rS2))
	   curr (ld)  == (set (reg:DI rD)
			      (mem:DI (reg:DI rD))) */

      if (MEM_P (SET_SRC (curr_set))
	  && SCALAR_INT_MODE_P (GET_MODE (SET_DEST (curr_set)))
	  && REG_P (XEXP (SET_SRC (curr_set), 0))
	  && REGNO (XEXP (SET_SRC (curr_set), 0)) == prev_dest_regno
	  && GET_CODE (SET_SRC (prev_set)) == PLUS
	  && REG_P (XEXP (SET_SRC (prev_set), 0))
	  && REG_P (XEXP (SET_SRC (prev_set), 1)))
	{
	  if (dump_file)
	    fprintf (dump_file, "RISCV_FUSE_LDINDEXED\n");
	  return true;
	}

      /* We are trying to match the following:
	   prev (add) == (set (reg:DI rD)
			      (plus:DI (reg:DI rS1) (reg:DI rS2)))
	   curr (lw)  == (set (any_extend:DI (mem:SUBX (reg:DI rD)))) */

      if ((GET_CODE (SET_SRC (curr_set)) == SIGN_EXTEND
	   || (GET_CODE (SET_SRC (curr_set)) == ZERO_EXTEND))
	  && MEM_P (XEXP (SET_SRC (curr_set), 0))
	  && SCALAR_INT_MODE_P (GET_MODE (SET_DEST (curr_set)))
	  && REG_P (XEXP (XEXP (SET_SRC (curr_set), 0), 0))
	  && REGNO (XEXP (XEXP (SET_SRC (curr_set), 0), 0)) == prev_dest_regno
	  && GET_CODE (SET_SRC (prev_set)) == PLUS
	  && REG_P (XEXP (SET_SRC (prev_set), 0))
	  && REG_P (XEXP (SET_SRC (prev_set), 1)))
	{
	  if (dump_file)
	    fprintf (dump_file, "RISCV_FUSE_LDINDEXED\n");
	  return true;
	}
    }

  if (simple_sets_p && riscv_fusion_enabled_p (RISCV_FUSE_EXPANDED_LD)
      && (sched1 || prev_dest_regno == curr_dest_regno))
    {
      /* For the "expanded add/load fusion" family we have 2 main
	 categories: memory loads with displacement (i.e. with imm offset)
	 and loads without displacement (i.e. with offset = x0).

	 For loads without displacement we'll need:
	 - add + ld (done in RISCV_FUSE_LDINDEXED)
	 - addi + ld (done in RISCV_FUSE_LDPREINCREMENT)
	 - shNadd + ld
	 - add.uw + lw
	 - shNadd.uw + lw

	 For loads with displacement/immediates:
	 with lw with immediate):
	 - add + ld with displacement
	 - addi + ld with displacement
	 - shNadd + ld with displacement
	 - add.uw + lw with displacement
	 - shNadd.uw + lw with displacement */

      /* We're trying to match a curr_set ld with displacement:
	  prev (add|addi) = (set (reg:DI rd) (...))
	  curr (ld)  == (set (reg:DI rD)
		(mem:DI (plus:DI (reg:DI rD) (const_int IMM12)))) */
      if (MEM_P (SET_SRC (curr_set))
	  && SCALAR_INT_MODE_P (GET_MODE (SET_DEST (curr_set)))
	  && GET_CODE (XEXP (SET_SRC (curr_set), 0)) == PLUS
	  && REG_P (XEXP (XEXP (SET_SRC (curr_set), 0), 0))
	  && REGNO (XEXP (XEXP (SET_SRC (curr_set), 0), 0)) == prev_dest_regno)
	{
	  if (riscv_set_is_add (prev_set))
	    {
	      if (dump_file)
		fprintf (dump_file, "RISCV_FUSE_EXPANDED_LD\n");
	      return true;
	    }

	  if (riscv_set_is_addi (prev_set))
	    {
	      if (dump_file)
		fprintf (dump_file, "RISCV_FUSE_EXPANDED_LD\n");
	      return true;
	    }

	  if (riscv_set_is_shNadd (prev_set))
	    {
	      if (dump_file)
		fprintf (dump_file, "RISCV_FUSE_EXPANDED_LD\n");
	      return true;
	    }
	}

      /* We're trying to match a ld without displacement:
	  prev (addi|shNadd) = (reg:DI rD) (...))
	  curr (ld)  == (set (reg:DI rD)
			     (mem:DI (reg:DI rD))) */
      if (MEM_P (SET_SRC (curr_set))
	  && SCALAR_INT_MODE_P (GET_MODE (SET_DEST (curr_set)))
	  && REG_P (XEXP (SET_SRC (curr_set), 0))
	  && REGNO (XEXP (SET_SRC (curr_set), 0)) == prev_dest_regno)
	{
	  if (riscv_set_is_addi (prev_set))
	    {
	      if (dump_file)
		fprintf (dump_file, "RISCV_FUSE_EXPANDED_LD\n");
	      return true;
	    }

	  if (riscv_set_is_shNadd (prev_set))
	    {
	      if (dump_file)
		fprintf (dump_file, "RISCV_FUSE_EXPANDED_LD\n");
	      return true;
	    }
	}

      /* We're trying to match a curr_set lw with displacement:
	  prev (add.uw|shNadd.uw) = (set (reg:DI rd) (...))
	  curr (lw)  == (set (reg:DI rd)
		(any_extend:DI (mem:SUBX (plus:DI ((reg:DI rd)
						   (const_int IMM)))) */
      if ((GET_CODE (SET_SRC (curr_set)) == SIGN_EXTEND
	   || (GET_CODE (SET_SRC (curr_set)) == ZERO_EXTEND))
	  && MEM_P (XEXP (SET_SRC (curr_set), 0))
	  && SCALAR_INT_MODE_P (GET_MODE (SET_DEST (curr_set)))
	  && GET_CODE (XEXP (XEXP (SET_SRC (curr_set), 0), 0)) == PLUS
	  && REG_P (XEXP (XEXP (XEXP (SET_SRC (curr_set), 0), 0), 0))
	  && (REGNO (XEXP (XEXP (XEXP (SET_SRC (curr_set), 0), 0), 0))
	      == prev_dest_regno))
	{
	  if (riscv_set_is_adduw (prev_set))
	    {
	      if (dump_file)
		fprintf (dump_file, "RISCV_FUSE_EXPANDED_LD\n");
	      return true;
	    }

	  if (riscv_set_is_shNadduw (prev_set))
	    {
	      if (dump_file)
		fprintf (dump_file, "RISCV_FUSE_EXPANDED_LD\n");
	      return true;
	    }
	}

      /* We're trying to match a curr_set lw without displacement:
	  prev (add.uw|shNadd.uw) = (set (reg:DI rd) (...))
	  curr (ld|lh|lw)  == (set (reg:DI rd)
		(any_extend:DI (mem:SUBX (reg:DI rsd)))) */
      if ((GET_CODE (SET_SRC (curr_set)) == SIGN_EXTEND
	   || (GET_CODE (SET_SRC (curr_set)) == ZERO_EXTEND))
	  && MEM_P (XEXP (SET_SRC (curr_set), 0))
	  && SCALAR_INT_MODE_P (GET_MODE (SET_DEST (curr_set)))
	  && REG_P (XEXP (XEXP (SET_SRC (curr_set), 0), 0))
	  && REGNO (XEXP (XEXP (SET_SRC (curr_set), 0), 0)) == prev_dest_regno)
	{
	  if (riscv_set_is_adduw (prev_set))
	    {
	      if (dump_file)
		fprintf (dump_file, "RISCV_FUSE_EXPANDED_LD\n");
	      return true;
	    }

	  if (riscv_set_is_shNadduw (prev_set))
	    {
	      if (dump_file)
		fprintf (dump_file, "RISCV_FUSE_EXPANDED_LD\n");
	      return true;
	    }
	}
    }

  if (simple_sets_p && riscv_fusion_enabled_p (RISCV_FUSE_LDPREINCREMENT)
      && (sched1 || prev_dest_regno == curr_dest_regno))
    {
      /* We are trying to match the following:
	   prev (add) == (set (reg:DI rS)
			      (plus:DI (reg:DI rS) (const_int))
	   curr (ld)  == (set (reg:DI rD)
			      (mem:DI (reg:DI rS))) */

      if (MEM_P (SET_SRC (curr_set))
	  && SCALAR_INT_MODE_P (GET_MODE (SET_DEST (curr_set)))
	  && REG_P (XEXP (SET_SRC (curr_set), 0))
	  && REGNO (XEXP (SET_SRC (curr_set), 0)) == prev_dest_regno
	  && GET_CODE (SET_SRC (prev_set)) == PLUS
	  && REG_P (XEXP (SET_SRC (prev_set), 0))
	  && CONST_INT_P (XEXP (SET_SRC (prev_set), 1)))
	{
	  if (dump_file)
	    fprintf (dump_file, "RISCV_FUSE_LDPREINCREMENT\n");
	  return true;
	}
    }

  if (simple_sets_p && riscv_fusion_enabled_p (RISCV_FUSE_LUI_ADDI)
      && (sched1 || prev_dest_regno == curr_dest_regno))
    {
      /* We are trying to match the following:
	   prev (lui)  == (set (reg:DI rD) (const_int UPPER_IMM_20))
	   curr (addi) == (set (reg:DI rD)
			       (plus:DI (reg:DI rD) (const_int IMM12))) */

      if ((GET_CODE (SET_SRC (curr_set)) == LO_SUM
	   || (GET_CODE (SET_SRC (curr_set)) == PLUS
	       && CONST_INT_P (XEXP (SET_SRC (curr_set), 1))
	       && SMALL_OPERAND (INTVAL (XEXP (SET_SRC (curr_set), 1)))))
	  && (GET_CODE (SET_SRC (prev_set)) == HIGH
	      || (CONST_INT_P (SET_SRC (prev_set))
		  && LUI_OPERAND (INTVAL (SET_SRC (prev_set))))))
	{
	  if (dump_file)
	    fprintf (dump_file, "RISCV_FUSE_LUI_ADDI\n");
	  return true;
	}
    }

  if (simple_sets_p && riscv_fusion_enabled_p (RISCV_FUSE_AUIPC_ADDI)
      && (sched1 || prev_dest_regno == curr_dest_regno))
    {
      /* We are trying to match the following:
	   prev (auipc) == (set (reg:DI rD) (unspec:DI [...] UNSPEC_AUIPC))
	   curr (addi)  == (set (reg:DI rD)
				(plus:DI (reg:DI rD) (const_int IMM12)))
	 and
	   prev (auipc) == (set (reg:DI rD) (unspec:DI [...] UNSPEC_AUIPC))
	   curr (addi)  == (set (reg:DI rD)
				(lo_sum:DI (reg:DI rD) (const_int IMM12))) */

      if (GET_CODE (SET_SRC (prev_set)) == UNSPEC
	  && XINT (SET_SRC (prev_set), 1) == UNSPEC_AUIPC
	  && (GET_CODE (SET_SRC (curr_set)) == LO_SUM
	      || (GET_CODE (SET_SRC (curr_set)) == PLUS
		  && CONST_INT_P (XEXP (SET_SRC (curr_set), 1))
		  && SMALL_OPERAND (INTVAL (XEXP (SET_SRC (curr_set), 1))))))

	{
	  if (dump_file)
	    fprintf (dump_file, "RISCV_FUSE_AUIPC_ADDI\n");
	  return true;
	}
    }

  if (simple_sets_p && riscv_fusion_enabled_p (RISCV_FUSE_LUI_LD)
      && (sched1 || prev_dest_regno == curr_dest_regno))
    {
      /* We are trying to match the following:
	   prev (lui)  == (set (reg:DI rD) (const_int UPPER_IMM_20))
	   curr (ld)  == (set (reg:DI rD)
			      (mem:DI (plus:DI (reg:DI rD) (const_int IMM12)))) */

      /* A LUI_OPERAND accepts (const_int 0), but we won't emit that as LUI.  So
	 reject that case explicitly.  */
      if (CONST_INT_P (SET_SRC (prev_set))
	  && SET_SRC (prev_set) != CONST0_RTX (GET_MODE (SET_DEST (prev_set)))
	  && LUI_OPERAND (INTVAL (SET_SRC (prev_set)))
	  && MEM_P (SET_SRC (curr_set))
	  && SCALAR_INT_MODE_P (GET_MODE (SET_DEST (curr_set)))
	  && GET_CODE (XEXP (SET_SRC (curr_set), 0)) == PLUS
	  && REG_P (XEXP (XEXP (SET_SRC (curr_set), 0), 0))
	  && REGNO (XEXP (XEXP (SET_SRC (curr_set), 0), 0)) == prev_dest_regno)
	{
	  if (dump_file)
	    fprintf (dump_file, "RISCV_FUSE_LUI_LD\n");
	  return true;
	}

      if (GET_CODE (SET_SRC (prev_set)) == HIGH
	  && MEM_P (SET_SRC (curr_set))
	  && SCALAR_INT_MODE_P (GET_MODE (SET_DEST (curr_set)))
	  && GET_CODE (XEXP (SET_SRC (curr_set), 0)) == LO_SUM
	  && REG_P (XEXP (XEXP (SET_SRC (curr_set), 0), 0))
	  && REGNO (XEXP (XEXP (SET_SRC (curr_set), 0), 0)) == prev_dest_regno)
	{
	  if (dump_file)
	    fprintf (dump_file, "RISCV_FUSE_LUI_LD\n");
	  return true;
	}

      if (GET_CODE (SET_SRC (prev_set)) == HIGH
	  && (GET_CODE (SET_SRC (curr_set)) == SIGN_EXTEND
	      || GET_CODE (SET_SRC (curr_set)) == ZERO_EXTEND)
	  && MEM_P (XEXP (SET_SRC (curr_set), 0))
	  && SCALAR_INT_MODE_P (GET_MODE (SET_DEST (curr_set)))
	  && (GET_CODE (XEXP (XEXP (SET_SRC (curr_set), 0), 0)) == LO_SUM
	      && REG_P (XEXP (XEXP (XEXP (SET_SRC (curr_set), 0), 0), 0))
	      && (REGNO (XEXP (XEXP (XEXP (SET_SRC (curr_set), 0), 0), 0))
		  == prev_dest_regno)))
	{
	  if (dump_file)
	    fprintf (dump_file, "RISCV_FUSE_LUI_LD\n");
	  return true;
	}
    }

  if (simple_sets_p && riscv_fusion_enabled_p (RISCV_FUSE_AUIPC_LD)
      && (sched1 || prev_dest_regno == curr_dest_regno))
    {
      /* We are trying to match the following:
	   prev (auipc) == (set (reg:DI rD) (unspec:DI [...] UNSPEC_AUIPC))
	   curr (ld)  == (set (reg:DI rD)
			      (mem:DI (plus:DI (reg:DI rD) (const_int IMM12)))) */

      if (GET_CODE (SET_SRC (prev_set)) == UNSPEC
	  && XINT (SET_SRC (prev_set), 1) == UNSPEC_AUIPC
	  && MEM_P (SET_SRC (curr_set))
	  && SCALAR_INT_MODE_P (GET_MODE (SET_DEST (curr_set)))
	  && GET_CODE (XEXP (SET_SRC (curr_set), 0)) == PLUS)
	{
	  if (dump_file)
	    fprintf (dump_file, "RISCV_FUSE_AUIPC_LD\n");
	  return true;
	}
    }

  if (simple_sets_p && riscv_fusion_enabled_p (RISCV_FUSE_CACHE_ALIGNED_STD))
    {
      /* We are trying to match the following:
	   prev (sd) == (set (mem (plus (reg sp|fp) (const_int)))
			      (reg rS1))
	   curr (sd) == (set (mem (plus (reg sp|fp) (const_int)))
			      (reg rS2)) */

      if (MEM_P (SET_DEST (prev_set))
	  && MEM_P (SET_DEST (curr_set))
	  && SCALAR_INT_MODE_P (GET_MODE (SET_DEST (curr_set)))
	  /* We can probably relax this condition.  The documentation is a bit
	     unclear about sub-word cases.  So we just model DImode for now.  */
	  && GET_MODE (SET_DEST (curr_set)) == DImode
	  && GET_MODE (SET_DEST (prev_set)) == DImode)
	{
	  rtx base_prev, base_curr, offset_prev, offset_curr;

	  extract_base_offset_in_addr (SET_DEST (prev_set), &base_prev, &offset_prev);
	  extract_base_offset_in_addr (SET_DEST (curr_set), &base_curr, &offset_curr);

	  /* Proceed only if we find both bases, both bases are register and
	     bases are the same register.  */
	  if (base_prev != NULL_RTX && base_curr != NULL_RTX
	      && REG_P (base_prev) && REG_P (base_curr)
	      && REGNO (base_prev) == REGNO (base_curr)
	      /* The alignment of the base pointer is more useful than the
		 alignment of the memory reference for determining if we're
		 on opposite sides of a cache line.  */
	      && REGNO_POINTER_ALIGN (ORIGINAL_REGNO (base_prev)) >= 128)
	    {
	      /* The two stores must be contained within opposite halves of the
		 same 16 byte aligned block of memory.  We know the pointer
		 has suitable alignment, so we just need to check the offsets
		 of the two stores for suitable alignment.  */

	      /* Get the smaller offset into OFFSET_PREV.  */
	      if (INTVAL (offset_prev) > INTVAL (offset_curr))
		std::swap (offset_prev, offset_curr);

	      /* We have a match if the smaller offset (OFFSET_PREV) is 16
		 byte aligned and the higher offset is 8 bytes more than the
		 lower offset.  */
	      if ((INTVAL (offset_prev) % 16) == 0
		  && (INTVAL (offset_prev) + 8 == INTVAL (offset_curr)))
		{
		  if (dump_file)
		    fprintf (dump_file, "RISCV_FUSE_CACHE_ALIGNED_STD\n");
		  return true;
		}
	    }
	}
    }

  /* More general form of the RISCV_FUSE_CACHE_ALIGNED_STD.  The
     major difference is the dependency on the stores being opposite
     halves of a cache line is dropped.  Instead the lowest address
     needs 2X the alignment of the object and the higher address
     immediately followed the first object.  */
  if (simple_sets_p && riscv_fusion_enabled_p (RISCV_FUSE_ALIGNED_STD))
    {
      /* We are trying to match the following:
	prev (sd) == (set (mem (plus (reg rS1) (const_int)))
			  (reg rS2))
	curr (sd) == (set (mem (plus (reg rS1) (const_int)))
			  (reg rS3)) */

      if (MEM_P (SET_DEST (prev_set))
	  && SCALAR_INT_MODE_P (GET_MODE (SET_DEST (curr_set)))
	  && MEM_P (SET_DEST (curr_set))
	  /* Stores must have the same width */
	  && GET_MODE (SET_DEST (curr_set)) == GET_MODE (SET_DEST (prev_set)))
	{
	  rtx base_prev, base_curr, offset_prev, offset_curr;
	  unsigned mode_size;

	  extract_base_offset_in_addr (SET_DEST (prev_set),
				       &base_prev, &offset_prev);
	  extract_base_offset_in_addr (SET_DEST (curr_set),
				       &base_curr, &offset_curr);

	  /* Proceed only if we find both bases, both bases
	     are registers and bases are the same register.  */
	  if (base_prev != NULL_RTX && base_curr != NULL_RTX
	      && REG_P (base_prev) && REG_P (base_curr)
	      && REGNO (base_prev) == REGNO (base_curr))
	    {
	      machine_mode mode = GET_MODE (SET_DEST (curr_set));
	      mode_size = estimated_poly_value (GET_MODE_SIZE (mode));

	      HOST_WIDE_INT offset_prev_int = INTVAL (offset_prev);
	      HOST_WIDE_INT offset_curr_int = INTVAL (offset_curr);

	      /* Get the smaller offset into OFFSET_PREV_INT.  */
	      if (offset_prev_int > offset_curr_int)
		std::swap (offset_prev_int, offset_curr_int);

	      /* We've normalized, so we need to check that the lower
		 address is aligned to 2X the size of the object.  The
		 higher address must be the lower address plus the
		 size of the object.  */
	      if (((offset_prev_int % (2 * mode_size)) == 0)
		  && offset_prev_int + mode_size == offset_curr_int)
		{
		  if (dump_file)
		    fprintf (dump_file, "RISCV_FUSE_ALIGNED_STD\n");
		  return true;
		}
	    }
	}
    }

  if (simple_sets_p && riscv_fusion_enabled_p (RISCV_FUSE_BFEXT)
      && (sched1 || prev_dest_regno == curr_dest_regno))
    {
      /* We are trying to match the following:
	   prev (slli) == (set (reg:DI rD)
			       (ashift:DI (reg:DI rS) (const_int)))
	   curr (srli) == (set (reg:DI rD)
			       (lshiftrt:DI (reg:DI rD) (const_int))) */

      if (GET_CODE (SET_SRC (prev_set)) == ASHIFT
	  && (GET_CODE (SET_SRC (curr_set)) == LSHIFTRT
	      || GET_CODE (SET_SRC (curr_set)) == ASHIFTRT)
	  && REG_P (SET_DEST (prev_set))
	  && REG_P (SET_DEST (curr_set))
	  && REGNO (XEXP (SET_SRC (curr_set), 0)) == prev_dest_regno
	  && CONST_INT_P (XEXP (SET_SRC (prev_set), 1))
	  && CONST_INT_P (XEXP (SET_SRC (curr_set), 1)))
	{
	  if (dump_file)
	    fprintf (dump_file, "RISCV_FUSE_BFEXT\n");
	  return true;
	}
    }

  if (simple_sets_p && riscv_fusion_enabled_p (RISCV_FUSE_B_ALUI)
      && (sched1 || prev_dest_regno == curr_dest_regno))
    {
      /* We are trying to match the following:
	  prev (orc.b) == (set (reg rD)
				(unspec (reg rS1)))
	  curr (not) == (set (reg rD2) (not (reg rD))) */

      if (GET_CODE (SET_SRC (prev_set)) == UNSPEC
	  && GET_CODE (SET_SRC (curr_set)) == NOT
	  && XINT (SET_SRC (prev_set), 1) == UNSPEC_ORC_B
	  && REG_P (SET_DEST (prev_set))
	  && REG_P (SET_DEST (curr_set))
	  && REG_P (XEXP (SET_SRC (curr_set), 0))
	  && REGNO (XEXP (SET_SRC (curr_set), 0)) == prev_dest_regno)
	{
	  if (dump_file)
	    fprintf (dump_file, "RISCV_FUSE_B_ALUI\n");
	  return true;
	}

      /* We are trying to match the following:
	  prev (ctz) == (set (reg rD) (ctz (reg rS1)))
	  curr (andi) == (set (reg rD)
				(and (reg rD) (const_int 63))) */

      if (GET_CODE (SET_SRC (prev_set)) == CTZ
	  && GET_CODE (SET_SRC (curr_set)) == AND
	  && CONST_INT_P (XEXP (SET_SRC (curr_set), 1))
	  && INTVAL (XEXP (SET_SRC (curr_set), 1)) == 63
	  && REG_P (SET_DEST (prev_set))
	  && REG_P (SET_DEST (curr_set))
	  && REG_P (XEXP (SET_SRC (curr_set), 0))
	  && REGNO (XEXP (SET_SRC (curr_set), 0)) == prev_dest_regno)
	{
	  if (dump_file)
	    fprintf (dump_file, "RISCV_FUSE_B_ALUI\n");
	  return true;
	}

      /* We are trying to match the following:
	  prev (sub) == (set (reg rD)
				(minus (const_int 0) (reg rS2))
	  curr (max) == (set (reg rD)
				(smax (reg rD) (reg rS2))) */

      if (GET_CODE (SET_SRC (prev_set)) == MINUS
	  && (XEXP (SET_SRC (prev_set), 0)
	      == CONST0_RTX (GET_MODE (SET_SRC (prev_set))))
	  && GET_CODE (SET_SRC (curr_set)) == SMAX
	  && REG_P (SET_DEST (prev_set))
	  && REG_P (SET_DEST (curr_set))
	  && REG_P (XEXP (SET_SRC (curr_set), 0))
	  && REGNO (XEXP (SET_SRC (curr_set), 0)) == prev_dest_regno
	  && REG_P (XEXP (SET_SRC (prev_set), 1))
	  && REG_P (XEXP (SET_SRC (curr_set), 1))
	  && (REGNO (XEXP (SET_SRC (prev_set), 1))
	      == REGNO (XEXP (SET_SRC (curr_set), 1))))
	{
	  if (dump_file)
	    fprintf (dump_file, "RISCV_FUSE_B_ALUI\n");
	  return true;
	}

      /* We are trying to match the following:
	  prev (neg) == (set (reg rD) (neg (reg rS1)))
	  curr (max) == (set (reg rD)
				(smax (reg rD) (reg rS1))) */

      if (GET_CODE (SET_SRC (prev_set)) == NEG
	  && GET_CODE (SET_SRC (curr_set)) == SMAX
	  && REG_P (SET_DEST (prev_set))
	  && REG_P (SET_DEST (curr_set))
	  && REG_P (XEXP (SET_SRC (curr_set), 0))
	  && REGNO (XEXP (SET_SRC (curr_set), 0)) == prev_dest_regno
	  && REG_P (XEXP (SET_SRC (prev_set), 0))
	  && REG_P (XEXP (SET_SRC (curr_set), 1))
	  && (REGNO (XEXP (SET_SRC (prev_set), 0))
	      == REGNO (XEXP (SET_SRC (curr_set), 1))))
	{
	  if (dump_file)
	    fprintf (dump_file, "RISCV_FUSE_B_ALUI\n");
	  return true;
	}
    }

  return false;
}
