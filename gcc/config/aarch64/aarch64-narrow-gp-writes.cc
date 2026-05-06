/* GP register writes narrowing pass.
   Copyright The GNU Toolchain Authors.

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

#define INCLUDE_ALGORITHM
#define INCLUDE_FUNCTIONAL
#define INCLUDE_ARRAY

#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "backend.h"
#include "rtl.h"
#include "df.h"
#include "hash-map.h"
#include "rtl-ssa.h"
#include "rtlhooks-def.h"
#include "rtl-iter.h"
#include "tree-pass.h"
#include "insn-attr.h"

using namespace rtl_ssa;

namespace {

/* This pass converts 64-bit (X-register) operations to 32-bit (W-register)
   operations when the upper 32 bits of the result are known to be zero.

   In AArch64, each 64-bit X register has a corresponding 32-bit W register
   that maps to its lower half.  When we can guarantee that the upper 32 bits
   are never used, we can safely narrow operations to use W registers instead.

   For example, the following code:
     uint64_t foo (uint64_t a) {
	 return (a & 255) + 3;
     }

   Without this pass, compiles to:
	and	x0, x0, 255
	add	x0, x0, 3
	ret

   But can be optimized to:
	and	x0, x0, 255
	add	w0, w0, 3
	ret

   The pass makes a single forward sweep over EBBs in RPO:
      - Computes a nonzero-bit mask for every GP register definition.
      - A custom reg_nonzero_bits RTL hook feeds the per-definition masks
      (stored in nzero_map) into nonzero_bits (), making it context-aware.
      - At control-flow joins, combine_mask_from_phi ORs the masks of all phi
      inputs; inputs not yet processed (backedges) receive a conservative mask.
      - Instructions whose results fit in 32 bits are narrowed after verifying
      with rtl_ssa::recog and rtl_ssa::change_is_worthwhile.
      - Both plain SET and flag-setting PARALLEL patterns (ADDS, SUBS, ANDS,
      etc.) are handled.  */

const pass_data pass_data_narrow_gp_writes = {
  RTL_PASS,	      // type
  "narrow_gp_writes", // name
  OPTGROUP_NONE,      // optinfo_flags
  TV_MACH_DEP,	      // tv_id
  0,		      // properties_required
  0,		      // properties_provided
  0,		      // properties_destroyed
  0,		      // todo_flags_start
  TODO_df_finish,     // todo_flags_finish
};

using mask_t = unsigned HOST_WIDE_INT;

class narrow_gp_writes
{
public:
  narrow_gp_writes ();
  ~narrow_gp_writes ();
  void execute ();

  static narrow_gp_writes *get_instance () { return instance; }
  mask_t lookup_mask_from_def (def_info *def);

  /* Current instruction being analyzed, set during the main loop and
     read by the reg_nonzero_bits hook.  */
  insn_info *m_curr_insn = nullptr;

private:
  static narrow_gp_writes *instance;

  /* Map from register definitions to their nonzero bit masks.  */
  hash_map<def_info *, mask_t> m_nzero_map;

  mask_t combine_mask_from_phi (phi_info *phi);
  rtx optimize_single_set_insn (insn_info *, rtx);
  rtx optimize_compare_arith_insn (insn_info *);
  bool optimize_insn (insn_info *);
};

narrow_gp_writes *narrow_gp_writes::instance = nullptr;

/* Return the nonzero bit mask for definition DEF.  If we haven't computed
   a mask for this definition yet, return a conservative mask based on the
   mode.  */

mask_t
narrow_gp_writes::lookup_mask_from_def (def_info *def)
{
  mask_t *res = m_nzero_map.get (def);
  return res ? *res : GET_MODE_MASK (def->mode ());
}

/* Compute the nonzero bit mask for phi node PHI by combining the masks
   of all incoming values.  Inputs not yet in m_nzero_map (e.g. from
   backedges) are given a conservative mode mask.

   The caller is expected to call this once per phi and store the result;
   calling it on demand would re-walk all phi inputs for every use, giving
   O(in-degree * num-uses) cost.  */

mask_t
narrow_gp_writes::combine_mask_from_phi (phi_info *phi)
{
  mask_t phi_mode_mask = GET_MODE_MASK (phi->mode ());
  mask_t combined_mask = 0;
  for (use_info *phi_use : phi->inputs ())
    {
      set_info *phi_set = phi_use->def ();
      if (!phi_set)
	{
	  combined_mask |= phi_mode_mask;
	  break;
	}
      combined_mask |= lookup_mask_from_def (phi_set);
      if ((combined_mask & phi_mode_mask) == phi_mode_mask)
	break;
    }

  return phi_mode_mask & combined_mask;
}

/* RTL hooks callback for computing nonzero bits of registers.
   Updates NONZERO with the tracked nonzero bits for register X.  */

static rtx
reg_nonzero_bits_for_narrow_gp_writes (const_rtx x, scalar_int_mode xmode,
				       scalar_int_mode,
				       unsigned HOST_WIDE_INT *nonzero)
{
  auto pass = narrow_gp_writes::get_instance ();
  if (auto use = find_access (pass->m_curr_insn->uses (), REGNO (x)))
    if (use->mode () == xmode)
      {
	if (def_info *set = use->def ())
	  {
	    mask_t mask = pass->lookup_mask_from_def (set);
	    /* If the def is in a narrower mode than the use, RTL semantics
	       say the upper bits are undefined.  Treat them conservatively
	       as potentially nonzero.  */
	    if (known_lt (GET_MODE_SIZE (set->mode ()),
			  GET_MODE_SIZE (use->mode ())))
	      mask |= ~GET_MODE_MASK (set->mode ());
	    *nonzero &= mask;
	  }
	return NULL_RTX;
      }

  return NULL_RTX;
}

/* Convert a DImode RTL expression X to its SImode equivalent.
   Recursively narrows operands of supported operations.  */

static rtx
narrow_dimode_src (rtx x)
{
  rtx op = lowpart_subreg (SImode, x, DImode);
  rtx_code code = GET_CODE (op);

  /* If the generic lowpart logic simplifies it then use that.
     If it just results in wrapping X in a subreg or truncate then try harder
     below.  */
  if (code == SUBREG && subreg_lowpart_p (op)
      && GET_MODE (SUBREG_REG (op)) == DImode)
    op = SUBREG_REG (op);
  else if (code == TRUNCATE && GET_MODE (XEXP (op, 0)) == DImode)
    op = XEXP (op, 0);
  else
    return op;

  code = GET_CODE (op);
  switch (code)
    {
    case AND:
    case IOR:
    case XOR:
    case ASHIFT:
      {
	rtx op0 = narrow_dimode_src (XEXP (op, 0));
	rtx op1 = narrow_dimode_src (XEXP (op, 1));
	if (GET_MODE (op0) == DImode || GET_MODE (op1) == DImode)
	  return op;
	return simplify_gen_binary (code, SImode, op0, op1);
      }

    case IF_THEN_ELSE:
      {
	rtx trueop = narrow_dimode_src (XEXP (op, 1));
	rtx falseop = narrow_dimode_src (XEXP (op, 2));
	if (GET_MODE (trueop) == DImode || GET_MODE (falseop) == DImode)
	  return op;
	return simplify_gen_ternary (code, SImode, GET_MODE (XEXP (op, 0)),
				     XEXP (op, 0), trueop, falseop);
      }

    default:
      return op;
    }
}

/* Override the reg_nonzero_bits hook so that nonzero_bits () consults our
   per-definition mask map when it encounters a register operand.  Without
   this, nonzero_bits () conservatively returns the full mode mask for any
   register.  */
#undef RTL_HOOKS_REG_NONZERO_REG_BITS
#define RTL_HOOKS_REG_NONZERO_REG_BITS reg_nonzero_bits_for_narrow_gp_writes
static const struct rtl_hooks narrow_gp_writes_rtl_hooks
  = RTL_HOOKS_INITIALIZER;

narrow_gp_writes::narrow_gp_writes ()
{
  timevar_push (TV_MACH_DEP);
  calculate_dominance_info (CDI_DOMINATORS);
  df_analyze ();
  crtl->ssa = new rtl_ssa::function_info (cfun);
  rtl_hooks = narrow_gp_writes_rtl_hooks;
  instance = this;
}

narrow_gp_writes::~narrow_gp_writes ()
{
  instance = nullptr;
  rtl_hooks = general_rtl_hooks;
  crtl->ssa->perform_pending_updates ();
  delete crtl->ssa;
  crtl->ssa = nullptr;
  free_dominance_info (CDI_DOMINATORS);
  timevar_pop (TV_MACH_DEP);
}

/* Return true if we can track nonzero bits and potentially narrow INSN.  */

static bool
can_track_nonzero_bits_p (insn_info *insn)
{
  return insn->can_be_optimized () && !insn->is_asm () && !insn->is_jump ()
	 && !insn->has_pre_post_modify ();
}

/* Attempt to replace INSN's pattern with NEW_PAT.  Returns true if the
   replacement was successful.  */

static bool
narrow_dimode_ops (insn_info *insn, rtx new_pat)
{
  if (dump_file)
    {
      fprintf (dump_file, "Trying to narrow insn:\n");
      print_rtl_single (dump_file, insn->rtl ());
      fprintf (dump_file, "with new pattern:\n");
      print_rtl_single (dump_file, new_pat);
    }

  auto attempt = crtl->ssa->new_change_attempt ();
  rtl_ssa::insn_change change (insn);
  rtx_insn *rtl = insn->rtl ();
  insn_change_watermark watermark;
  validate_change (rtl, &PATTERN (rtl), new_pat, 1);

  if (!rtl_ssa::recog (attempt, change))
    {
      if (dump_file)
	fprintf (dump_file, "Narrowed pattern not recognised, skipping\n");
      return false;
    }

  if (!rtl_ssa::change_is_worthwhile (change))
    {
      if (dump_file)
	fprintf (dump_file, "Narrowed pattern not worthwhile, skipping\n");
      return false;
    }

  confirm_change_group ();
  crtl->ssa->change_insn (change);
  if (dump_file)
    {
      fprintf (dump_file, "Successfully narrowed insn:\n");
      print_rtl_single (dump_file, new_pat);
    }
  return true;
}

/* Try to narrow flag-setting arithmetic operations (e.g., ADDS, SUBS, ANDS).
   These are represented as PARALLEL patterns with a compare and a set.
   Returns the narrowed pattern or NULL_RTX if narrowing is not possible.  */

rtx
narrow_gp_writes::optimize_compare_arith_insn (insn_info *insn)
{
  rtx pat = PATTERN (insn->rtl ());
  if (GET_CODE (pat) != PARALLEL || XVECLEN (pat, 0) != 2)
    return NULL_RTX;

  rtx cmp_set = XVECEXP (pat, 0, 0);
  rtx set = XVECEXP (pat, 0, 1);
  if (GET_CODE (cmp_set) != SET || GET_CODE (set) != SET)
    return NULL_RTX;

  if (!REG_P (SET_DEST (cmp_set)) || REGNO (SET_DEST (cmp_set)) != CC_REGNUM
      || GET_CODE (SET_SRC (cmp_set)) != COMPARE)
    return NULL_RTX;

  rtx set_src = SET_SRC (set);
  rtx set_dst = SET_DEST (set);
  if (!REG_P (set_dst) || !GP_REGNUM_P (REGNO (set_dst)))
    return NULL_RTX;

  /* Only record masks narrower than the full mode; lookup_mask_from_def
     already returns GET_MODE_MASK for defs not in the map.  */
  mask_t mask = nonzero_bits (set_src, GET_MODE (set_dst));
  if (mask != GET_MODE_MASK (GET_MODE (set_dst)))
    if (auto def = find_access (insn->defs (), REGNO (set_dst)))
      m_nzero_map.put (def, mask);

  if (GET_MODE (set_dst) != DImode)
    return NULL_RTX;

  /* Skip sources that would get simplified to the original instruction
     after zero extending to DImode and using simplify_gen_unary.  */
  if (CONST_INT_P (set_src) || GET_CODE (set_src) == ZERO_EXTEND)
    return NULL_RTX;

  rtx cmp_op0 = XEXP (SET_SRC (cmp_set), 0);
  rtx cmp_op1 = XEXP (SET_SRC (cmp_set), 1);
  if (cmp_op1 != CONST0_RTX (DImode) || !rtx_equal_p (set_src, cmp_op0))
    return NULL_RTX;

  /* CC_Zmode tests the Z flag, which is unaffected by narrowing as long as
     the upper 32 bits are known zero.

     CC_NZmode and CC_NZVmode also test the N flag.  Narrowing moves the
     sign bit from bit 63 to bit 31, so if bit 31 could be set, narrowing
     would flip N from 0 to 1.  We therefore require bit 31 to be zero
     as well.

     allowed_mask describes which bits are allowed to be nonzero for
     narrowing to be safe.  */
  machine_mode cc_mode = GET_MODE (SET_SRC (cmp_set));
  mask_t allowed_mask = 0;
  if (cc_mode == CC_Zmode)
    allowed_mask = GET_MODE_MASK (SImode);
  else if (cc_mode == CC_NZmode || cc_mode == CC_NZVmode)
    allowed_mask = GET_MODE_MASK (SImode) >> 1;
  else
    return NULL_RTX;

  if (mask & ~allowed_mask)
    {
      if (dump_file)
	fprintf (dump_file,
		 "Cannot narrow insn %d: mask " HOST_WIDE_INT_PRINT_HEX "\n",
		 INSN_UID (insn->rtl ()), mask);
      return NULL_RTX;
    }

  rtx narrowed_src = narrow_dimode_src (set_src);
  if (GET_MODE (narrowed_src) == DImode)
    return NULL_RTX;

  /* Same as optimize_single_set_insn: wrap in ZERO_EXTEND via
     simplify_gen_unary for canonical form, and bail out if the
     result reproduces the original.  */
  rtx simplified_src
    = simplify_gen_unary (ZERO_EXTEND, DImode, narrowed_src, SImode);

  /* If the simplified result is identical to the original source before
     narrowing, skip further processing.  */
  if (rtx_equal_p (simplified_src, set_src))
    {
      if (dump_file)
	fprintf (dump_file,
		 "Simplification reproduced original, "
		 "skipping insn %d\n",
		 INSN_UID (insn->rtl ()));
      return NULL_RTX;
    }

  rtx new_set = gen_rtx_SET (set_dst, simplified_src);
  rtx new_cmp_set
    = gen_rtx_SET (SET_DEST (cmp_set),
		   gen_rtx_COMPARE (cc_mode, copy_rtx (narrowed_src),
				    CONST0_RTX (SImode)));
  return gen_rtx_PARALLEL (VOIDmode, gen_rtvec (2, new_cmp_set, new_set));
}

/* Try to narrow single SET instructions.
   Returns the narrowed pattern or NULL_RTX if narrowing is not possible.  */

rtx
narrow_gp_writes::optimize_single_set_insn (insn_info *insn, rtx set)
{
  set_info *sinfo = single_set_info (insn);
  if (!sinfo)
    return NULL_RTX;
  rtx dst = SET_DEST (set);
  rtx src = SET_SRC (set);
  if (!REG_P (dst) || !GP_REGNUM_P (REGNO (dst)))
    return NULL_RTX;
  gcc_checking_assert (sinfo->regno () == REGNO (dst));

  /* Only record masks narrower than the full mode; lookup_mask_from_def
     already returns GET_MODE_MASK for defs not in the map.  */
  mask_t mask = nonzero_bits (src, GET_MODE (dst));
  if (mask != GET_MODE_MASK (GET_MODE (dst)))
    m_nzero_map.put (sinfo, mask);

  if (GET_MODE (dst) != DImode)
    return NULL_RTX;

  /* Skip sources that would get simplified to the original instruction
     after zero extending to DImode and using simplify_gen_unary.  */
  if (CONST_INT_P (src) || GET_CODE (src) == ZERO_EXTEND)
    return NULL_RTX;

  /* If mask overlaps with top 32 bits we can't narrow.  */
  if (~GET_MODE_MASK (SImode) & mask)
    {
      if (dump_file)
	fprintf (dump_file,
		 "Cannot narrow insn %d: mask " HOST_WIDE_INT_PRINT_HEX "\n",
		 INSN_UID (insn->rtl ()), mask);
      return NULL_RTX;
    }

  rtx narrowed_src = narrow_dimode_src (src);

  if (GET_MODE (narrowed_src) == DImode)
    return NULL_RTX;

  /* Wrap the narrowed source in a ZERO_EXTEND back to DImode.  RTL
     semantics say that an SImode write leaves the upper 32 bits undefined,
     but on AArch64 writing a W register zeros them.  Without the
     ZERO_EXTEND a later pass could see that the SImode result matches the
     lower 32 bits, delete the write as redundant, and silently lose the
     upper-bit clearing.

     Use simplify_gen_unary so that the result is in canonical form.  */
  rtx simplified_src
    = simplify_gen_unary (ZERO_EXTEND, DImode, narrowed_src, SImode);

  /* If the simplified result is identical to the original source before
     narrowing, skip further processing.  */
  if (rtx_equal_p (simplified_src, src))
    {
      if (dump_file)
	fprintf (dump_file,
		 "Simplification reproduced original, "
		 "skipping insn %d\n",
		 INSN_UID (insn->rtl ()));
      return NULL_RTX;
    }

  if (dump_file)
    {
      fprintf (dump_file, "Can narrow insn %d:\n", INSN_UID (insn->rtl ()));
      print_rtl_single (dump_file, insn->rtl ());
    }

  return gen_rtx_SET (dst, simplified_src);
}

/* Analyze INSN for narrowing opportunities.  Tracks nonzero bits for GP
   register definitions.  Returns true if INSN was narrowed.  */

bool
narrow_gp_writes::optimize_insn (insn_info *insn)
{
  if (!can_track_nonzero_bits_p (insn))
    return false;

  rtx new_pat;
  if (auto set = single_set (insn->rtl ()))
    new_pat = optimize_single_set_insn (insn, set);
  else
    new_pat = optimize_compare_arith_insn (insn);

  /* Volatile instructions can be tracked above, but should not be narrowed.  */
  if (!new_pat || insn->has_volatile_refs ())
    return false;

  return narrow_dimode_ops (insn, new_pat);
}

void
narrow_gp_writes::execute ()
{
  unsigned HOST_WIDE_INT narrowed = 0;

  /* RTL-SSA provides EBBs in RPO, so definitions are visited before their
     forward-edge uses.  Backedge inputs to phis are conservatively handled
     by using the full mode mask.  */
  for (auto ebb : crtl->ssa->ebbs ())
    {
      for (auto *phi : ebb->phis ())
	{
	  if (dump_file && (dump_flags & TDF_DETAILS))
	    {
	      fprintf (dump_file, "Processing phi:\n");
	      dump (dump_file, phi);
	    }
	  if (!GP_REGNUM_P (phi->regno ()))
	    continue;

	  /* Only record masks narrower than the full mode;
	     lookup_mask_from_def already returns GET_MODE_MASK for defs
	     not in the map.  */
	  mask_t phi_mask = combine_mask_from_phi (phi);
	  if (phi_mask != GET_MODE_MASK (phi->mode ()))
	    m_nzero_map.put (phi, phi_mask);
	}
      for (auto insn : ebb->nondebug_insns ())
	{
	  if (dump_file && (dump_flags & TDF_DETAILS))
	    {
	      fprintf (dump_file, "\nProcessing insn:\n");
	      dump (dump_file, insn);
	    }

	  m_curr_insn = insn;
	  if (optimize_insn (insn))
	    narrowed++;
	}
    }
  if (dump_file)
    fprintf (dump_file,
	     "Narrowed " HOST_WIDE_INT_PRINT_UNSIGNED " instructions\n",
	     narrowed);
}

class pass_narrow_gp_writes : public rtl_opt_pass
{
public:
  pass_narrow_gp_writes (gcc::context *ctxt)
    : rtl_opt_pass (pass_data_narrow_gp_writes, ctxt)
  {}

  /* opt_pass methods:  */
  virtual bool gate (function *)
  {
    /* aarch64_narrow_gp_writes takes three values:
       0: explicitly disabled by -mno-narrow-gp-writes.
       1: explicitly enabled by -mnarrow-gp-writes.
       2: default (Init(2)), enabled only if tuning flags request it.  */
    if (optimize < 2 || !aarch64_narrow_gp_writes)
      return false;

    return aarch64_narrow_gp_writes == 1
	   || (aarch64_tune_params.extra_tuning_flags
	       & AARCH64_EXTRA_TUNE_NARROW_GP_WRITES);
  }
  virtual unsigned int execute (function *);
};

unsigned int
pass_narrow_gp_writes::execute (function *)
{
  narrow_gp_writes ().execute ();
  return 0;
}

} // end namespace

/* Create a new narrow gp writes pass instance.  */
rtl_opt_pass *
make_pass_narrow_gp_writes (gcc::context *ctxt)
{
  return new pass_narrow_gp_writes (ctxt);
}