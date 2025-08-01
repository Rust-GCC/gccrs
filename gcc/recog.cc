/* Subroutines used by or related to instruction recognition.
   Copyright (C) 1987-2025 Free Software Foundation, Inc.

This file is part of GCC.

GCC is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free
Software Foundation; either version 3, or (at your option) any later
version.

GCC is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or
FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
for more details.

You should have received a copy of the GNU General Public License
along with GCC; see the file COPYING3.  If not see
<http://www.gnu.org/licenses/>.  */


#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "backend.h"
#include "target.h"
#include "rtl.h"
#include "tree.h"
#include "stmt.h"
#include "cfghooks.h"
#include "df.h"
#include "memmodel.h"
#include "tm_p.h"
#include "insn-config.h"
#include "regs.h"
#include "emit-rtl.h"
#include "recog.h"
#include "insn-attr.h"
#include "addresses.h"
#include "cfgrtl.h"
#include "cfgbuild.h"
#include "cfgcleanup.h"
#include "reload.h"
#include "tree-pass.h"
#include "function-abi.h"
#include "rtl-iter.h"

#ifndef STACK_POP_CODE
#if STACK_GROWS_DOWNWARD
#define STACK_POP_CODE POST_INC
#else
#define STACK_POP_CODE POST_DEC
#endif
#endif

static void validate_replace_rtx_1 (rtx *, rtx, rtx, rtx_insn *, bool);
static void validate_replace_src_1 (rtx *, void *);
static rtx_insn *split_insn (rtx_insn *);

struct target_recog default_target_recog;
#if SWITCHABLE_TARGET
struct target_recog *this_target_recog = &default_target_recog;
#endif

/* Nonzero means allow operands to be volatile.
   This should be 0 if you are generating rtl, such as if you are calling
   the functions in optabs.cc and expmed.cc (most of the time).
   This should be 1 if all valid insns need to be recognized,
   such as in reginfo.cc and final.cc and reload.cc.

   init_recog and init_recog_no_volatile are responsible for setting this.  */

int volatile_ok;

struct recog_data_d recog_data;

/* Contains a vector of operand_alternative structures, such that
   operand OP of alternative A is at index A * n_operands + OP.
   Set up by preprocess_constraints.  */
const operand_alternative *recog_op_alt;

/* Used to provide recog_op_alt for asms.  */
static operand_alternative asm_op_alt[MAX_RECOG_OPERANDS
				      * MAX_RECOG_ALTERNATIVES];

/* On return from `constrain_operands', indicate which alternative
   was satisfied.  */

int which_alternative;

/* True for inline asm operands with - constraint modifier.  */
bool raw_constraint_p;

/* Nonzero after end of reload pass.
   Set to 1 or 0 by toplev.cc.
   Controls the significance of (SUBREG (MEM)).  */

int reload_completed;

/* Nonzero after thread_prologue_and_epilogue_insns has run.  */
int epilogue_completed;

/* Initialize data used by the function `recog'.
   This must be called once in the compilation of a function
   before any insn recognition may be done in the function.  */

void
init_recog_no_volatile (void)
{
  volatile_ok = 0;
}

void
init_recog (void)
{
  volatile_ok = 1;
}


/* Return true if labels in asm operands BODY are LABEL_REFs.  */

static bool
asm_labels_ok (rtx body)
{
  rtx asmop;
  int i;

  asmop = extract_asm_operands (body);
  if (asmop == NULL_RTX)
    return true;

  for (i = 0; i < ASM_OPERANDS_LABEL_LENGTH (asmop); i++)
    if (GET_CODE (ASM_OPERANDS_LABEL (asmop, i)) != LABEL_REF)
      return false;

  return true;
}

/* Check that X is an insn-body for an `asm' with operands
   and that the operands mentioned in it are legitimate.  */

bool
check_asm_operands (rtx x)
{
  int noperands;
  rtx *operands;
  const char **constraints;
  int i;

  if (!asm_labels_ok (x))
    return false;

  /* Post-reload, be more strict with things.  */
  if (reload_completed)
    {
      /* ??? Doh!  We've not got the wrapping insn.  Cook one up.  */
      rtx_insn *insn = make_insn_raw (x);
      extract_insn (insn);
      constrain_operands (1, get_enabled_alternatives (insn));
      return which_alternative >= 0;
    }

  noperands = asm_noperands (x);
  if (noperands < 0)
    return false;
  if (noperands == 0)
    return true;

  operands = XALLOCAVEC (rtx, noperands);
  constraints = XALLOCAVEC (const char *, noperands);

  decode_asm_operands (x, operands, NULL, constraints, NULL, NULL);

  for (i = 0; i < noperands; i++)
    {
      const char *c = constraints[i];
      if (c[0] == '%')
	c++;
      if (! asm_operand_ok (operands[i], c, constraints))
	return false;
    }

  return true;
}

/* Static data for the next two routines.  */

struct change_t
{
  rtx object;
  int old_code;
  int old_len;
  bool unshare;
  rtx *loc;
  rtx old;
};

static change_t *changes;
static int changes_allocated;

static int num_changes = 0;
int undo_recog_changes::s_num_changes = 0;

/* Validate a proposed change to OBJECT.  LOC is the location in the rtl
   at which NEW_RTX will be placed.  If NEW_LEN is >= 0, XVECLEN (NEW_RTX, 0)
   will also be changed to NEW_LEN, which is no greater than the current
   XVECLEN.  If OBJECT is zero, no validation is done, the change is
   simply made.

   Two types of objects are supported:  If OBJECT is a MEM, memory_address_p
   will be called with the address and mode as parameters.  If OBJECT is
   an INSN, CALL_INSN, or JUMP_INSN, the insn will be re-recognized with
   the change in place.

   IN_GROUP is nonzero if this is part of a group of changes that must be
   performed as a group.  In that case, the changes will be stored.  The
   function `apply_change_group' will validate and apply the changes.

   If IN_GROUP is zero, this is a single change.  Try to recognize the insn
   or validate the memory reference with the change applied.  If the result
   is not valid for the machine, suppress the change and return false.
   Otherwise, perform the change and return true.  */

static bool
validate_change_1 (rtx object, rtx *loc, rtx new_rtx, bool in_group,
		   bool unshare, int new_len = -1)
{
  gcc_assert (!undo_recog_changes::is_active ());
  rtx old = *loc;

  /* Single-element parallels aren't valid and won't match anything.
     Replace them with the single element.  */
  if (new_len == 1 && GET_CODE (new_rtx) == PARALLEL)
    {
      new_rtx = XVECEXP (new_rtx, 0, 0);
      new_len = -1;
    }

  /* When a change is part of a group, callers expect to be able to change
     INSN_CODE after making the change and have the code reset to its old
     value by a later cancel_changes.  We therefore need to register group
     changes even if they're no-ops.  */
  if (!in_group
      && (old == new_rtx || rtx_equal_p (old, new_rtx))
      && (new_len < 0 || XVECLEN (new_rtx, 0) == new_len))
    return true;

  gcc_assert ((in_group != 0 || num_changes == 0)
	      && (new_len < 0 || new_rtx == *loc));

  *loc = new_rtx;

  /* Save the information describing this change.  */
  if (num_changes >= changes_allocated)
    {
      if (changes_allocated == 0)
	/* This value allows for repeated substitutions inside complex
	   indexed addresses, or changes in up to 5 insns.  */
	changes_allocated = MAX_RECOG_OPERANDS * 5;
      else
	changes_allocated *= 2;

      changes = XRESIZEVEC (change_t, changes, changes_allocated);
    }

  changes[num_changes].object = object;
  changes[num_changes].loc = loc;
  changes[num_changes].old = old;
  changes[num_changes].old_len = (new_len >= 0 ? XVECLEN (new_rtx, 0) : -1);
  changes[num_changes].unshare = unshare;

  if (new_len >= 0)
    XVECLEN (new_rtx, 0) = new_len;

  if (object && !MEM_P (object))
    {
      /* Set INSN_CODE to force rerecognition of insn.  Save old code in
	 case invalid.  */
      changes[num_changes].old_code = INSN_CODE (object);
      INSN_CODE (object) = -1;
    }

  num_changes++;

  /* If we are making a group of changes, return 1.  Otherwise, validate the
     change group we made.  */

  if (in_group)
    return true;
  else
    return apply_change_group ();
}

/* Wrapper for validate_change_1 without the UNSHARE argument defaulting
   UNSHARE to false.  */

bool
validate_change (rtx object, rtx *loc, rtx new_rtx, bool in_group)
{
  return validate_change_1 (object, loc, new_rtx, in_group, false);
}

/* Wrapper for validate_change_1 without the UNSHARE argument defaulting
   UNSHARE to true.  */

bool
validate_unshare_change (rtx object, rtx *loc, rtx new_rtx, bool in_group)
{
  return validate_change_1 (object, loc, new_rtx, in_group, true);
}

/* Change XVECLEN (*LOC, 0) to NEW_LEN.  OBJECT, IN_GROUP and the return
   value are as for validate_change_1.  */

bool
validate_change_xveclen (rtx object, rtx *loc, int new_len, bool in_group)
{
  return validate_change_1 (object, loc, *loc, in_group, false, new_len);
}

/* Keep X canonicalized if some changes have made it non-canonical; only
   modifies the operands of X, not (for example) its code.  Simplifications
   are not the job of this routine.

   Return true if anything was changed.  */
bool
canonicalize_change_group (rtx_insn *insn, rtx x)
{
  if (COMMUTATIVE_P (x)
      && swap_commutative_operands_p (XEXP (x, 0), XEXP (x, 1)))
    {
      /* Oops, the caller has made X no longer canonical.
	 Let's redo the changes in the correct order.  */
      rtx tem = XEXP (x, 0);
      validate_unshare_change (insn, &XEXP (x, 0), XEXP (x, 1), 1);
      validate_unshare_change (insn, &XEXP (x, 1), tem, 1);
      return true;
    }
  else
    return false;
}

/* Check if REG_INC argument in *data overlaps a stored REG.  */

static void
check_invalid_inc_dec (rtx reg, const_rtx, void *data)
{
  rtx *pinc = (rtx *) data;
  if (*pinc == NULL_RTX || MEM_P (reg))
    return;
  if (reg_overlap_mentioned_p (reg, *pinc))
    *pinc = NULL_RTX;
}

/* This subroutine of apply_change_group verifies whether the changes to INSN
   were valid; i.e. whether INSN can still be recognized.

   If IN_GROUP is true clobbers which have to be added in order to
   match the instructions will be added to the current change group.
   Otherwise the changes will take effect immediately.  */

bool
insn_invalid_p (rtx_insn *insn, bool in_group)
{
  rtx pat = PATTERN (insn);
  int num_clobbers = 0;
  /* If we are before reload and the pattern is a SET, see if we can add
     clobbers.  */
  int icode = recog (pat, insn,
		     (GET_CODE (pat) == SET
		      && ! reload_completed
                      && ! reload_in_progress)
		     ? &num_clobbers : 0);
  bool is_asm = icode < 0 && asm_noperands (PATTERN (insn)) >= 0;


  /* If this is an asm and the operand aren't legal, then fail.  Likewise if
     this is not an asm and the insn wasn't recognized.  */
  if ((is_asm && ! check_asm_operands (PATTERN (insn)))
      || (!is_asm && icode < 0))
    return true;

  /* If we have to add CLOBBERs, fail if we have to add ones that reference
     hard registers since our callers can't know if they are live or not.
     Otherwise, add them.  */
  if (num_clobbers > 0)
    {
      rtx newpat;

      if (added_clobbers_hard_reg_p (icode))
	return true;

      newpat = gen_rtx_PARALLEL (VOIDmode, rtvec_alloc (num_clobbers + 1));
      XVECEXP (newpat, 0, 0) = pat;
      add_clobbers (newpat, icode);
      if (in_group)
	validate_change (insn, &PATTERN (insn), newpat, 1);
      else
	PATTERN (insn) = pat = newpat;
    }

  /* After reload, verify that all constraints are satisfied.  */
  if (reload_completed)
    {
      extract_insn (insn);

      if (! constrain_operands (1, get_preferred_alternatives (insn)))
	return true;
    }

  /* Punt if REG_INC argument overlaps some stored REG.  */
  for (rtx link = FIND_REG_INC_NOTE (insn, NULL_RTX);
       link; link = XEXP (link, 1))
    if (REG_NOTE_KIND (link) == REG_INC)
      {
	rtx reg = XEXP (link, 0);
	note_stores (insn, check_invalid_inc_dec, &reg);
	if (reg == NULL_RTX)
	  return true;
      }

  INSN_CODE (insn) = icode;
  return false;
}

/* Return number of changes made and not validated yet.  */
int
num_changes_pending (void)
{
  return num_changes;
}

/* Tentatively apply the changes numbered NUM and up.
   Return true if all changes are valid, false otherwise.  */

bool
verify_changes (int num)
{
  int i;
  rtx last_validated = NULL_RTX;

  /* The changes have been applied and all INSN_CODEs have been reset to force
     rerecognition.

     The changes are valid if we aren't given an object, or if we are
     given a MEM and it still is a valid address, or if this is in insn
     and it is recognized.  In the latter case, if reload has completed,
     we also require that the operands meet the constraints for
     the insn.  */

  for (i = num; i < num_changes; i++)
    {
      rtx object = changes[i].object;

      /* If there is no object to test or if it is the same as the one we
         already tested, ignore it.  */
      if (object == 0 || object == last_validated)
	continue;

      if (MEM_P (object))
	{
	  if (! memory_address_addr_space_p (GET_MODE (object),
					     XEXP (object, 0),
					     MEM_ADDR_SPACE (object)))
	    break;
	}
      else if (/* changes[i].old might be zero, e.g. when putting a
	       REG_FRAME_RELATED_EXPR into a previously empty list.  */
	       changes[i].old
	       && REG_P (changes[i].old)
	       && asm_noperands (PATTERN (object)) > 0
	       && register_asm_p (changes[i].old))
	{
	  /* Don't allow changes of hard register operands to inline
	     assemblies if they have been defined as register asm ("x").  */
	  break;
	}
      else if (DEBUG_INSN_P (object))
	continue;
      else if (insn_invalid_p (as_a <rtx_insn *> (object), true))
	{
	  rtx pat = PATTERN (object);

	  /* Perhaps we couldn't recognize the insn because there were
	     extra CLOBBERs at the end.  If so, try to re-recognize
	     without the last CLOBBER (later iterations will cause each of
	     them to be eliminated, in turn).  But don't do this if we
	     have an ASM_OPERAND.  */
	  if (GET_CODE (pat) == PARALLEL
	      && GET_CODE (XVECEXP (pat, 0, XVECLEN (pat, 0) - 1)) == CLOBBER
	      && asm_noperands (PATTERN (object)) < 0)
	    {
	      rtx newpat;

	      if (XVECLEN (pat, 0) == 2)
		newpat = XVECEXP (pat, 0, 0);
	      else
		{
		  int j;

		  newpat
		    = gen_rtx_PARALLEL (VOIDmode,
					rtvec_alloc (XVECLEN (pat, 0) - 1));
		  for (j = 0; j < XVECLEN (newpat, 0); j++)
		    XVECEXP (newpat, 0, j) = XVECEXP (pat, 0, j);
		}

	      /* Add a new change to this group to replace the pattern
		 with this new pattern.  Then consider this change
		 as having succeeded.  The change we added will
		 cause the entire call to fail if things remain invalid.

		 Note that this can lose if a later change than the one
		 we are processing specified &XVECEXP (PATTERN (object), 0, X)
		 but this shouldn't occur.  */

	      validate_change (object, &PATTERN (object), newpat, 1);
	      continue;
	    }
	  else if (GET_CODE (pat) == USE || GET_CODE (pat) == CLOBBER
		   || GET_CODE (pat) == VAR_LOCATION)
	    /* If this insn is a CLOBBER or USE, it is always valid, but is
	       never recognized.  */
	    continue;
	  else
	    break;
	}
      last_validated = object;
    }

  return (i == num_changes);
}

/* A group of changes has previously been issued with validate_change
   and verified with verify_changes.  Call df_insn_rescan for each of
   the insn changed and clear num_changes.  */

void
confirm_change_group (void)
{
  int i;
  rtx last_object = NULL;

  gcc_assert (!undo_recog_changes::is_active ());
  for (i = 0; i < num_changes; i++)
    {
      rtx object = changes[i].object;

      if (changes[i].unshare)
	*changes[i].loc = copy_rtx (*changes[i].loc);

      /* Avoid unnecessary rescanning when multiple changes to same instruction
         are made.  */
      if (object)
	{
	  if (object != last_object && last_object && INSN_P (last_object))
	    df_insn_rescan (as_a <rtx_insn *> (last_object));
	  last_object = object;
	}
    }

  if (last_object && INSN_P (last_object))
    df_insn_rescan (as_a <rtx_insn *> (last_object));
  num_changes = 0;
}

/* Apply a group of changes previously issued with `validate_change'.
   If all changes are valid, call confirm_change_group and return true,
   otherwise, call cancel_changes and return false.  */

bool
apply_change_group (void)
{
  if (verify_changes (0))
    {
      confirm_change_group ();
      return true;
    }
  else
    {
      cancel_changes (0);
      return false;
    }
}


/* Return the number of changes so far in the current group.  */

int
num_validated_changes (void)
{
  return num_changes;
}

/* Retract the changes numbered NUM and up.  */

void
cancel_changes (int num)
{
  gcc_assert (!undo_recog_changes::is_active ());
  int i;

  /* Back out all the changes.  Do this in the opposite order in which
     they were made.  */
  for (i = num_changes - 1; i >= num; i--)
    {
      if (changes[i].old_len >= 0)
	XVECLEN (*changes[i].loc, 0) = changes[i].old_len;
      else
	*changes[i].loc = changes[i].old;
      if (changes[i].object && !MEM_P (changes[i].object))
	INSN_CODE (changes[i].object) = changes[i].old_code;
    }
  num_changes = num;
}

/* Swap the status of change NUM from being applied to not being applied,
   or vice versa.  */

static void
swap_change (int num)
{
  if (changes[num].old_len >= 0)
    std::swap (XVECLEN (*changes[num].loc, 0), changes[num].old_len);
  else
    std::swap (*changes[num].loc, changes[num].old);
  if (changes[num].object && !MEM_P (changes[num].object))
    {
      std::swap (INSN_CODE (changes[num].object), changes[num].old_code);
      if (recog_data.insn == changes[num].object)
	recog_data.insn = nullptr;
    }
}

undo_recog_changes::undo_recog_changes (int num)
  : m_old_num_changes (s_num_changes)
{
  gcc_assert (num <= num_changes - s_num_changes);
  for (int i = num_changes - s_num_changes - 1; i >= num; i--)
    swap_change (i);
  s_num_changes = num_changes - num;
}

undo_recog_changes::~undo_recog_changes ()
{
  for (int i = num_changes - s_num_changes;
       i < num_changes - m_old_num_changes; ++i)
    swap_change (i);
  s_num_changes = m_old_num_changes;
}

/* Reduce conditional compilation elsewhere.  */
/* A subroutine of validate_replace_rtx_1 that tries to simplify the resulting
   rtx.  */

static void
simplify_while_replacing (rtx *loc, rtx to, rtx_insn *object,
                          machine_mode op0_mode)
{
  rtx x = *loc;
  enum rtx_code code = GET_CODE (x);
  rtx new_rtx = NULL_RTX;
  scalar_int_mode is_mode;

  if (SWAPPABLE_OPERANDS_P (x)
      && swap_commutative_operands_p (XEXP (x, 0), XEXP (x, 1)))
    {
      validate_unshare_change (object, loc,
			       gen_rtx_fmt_ee (COMMUTATIVE_ARITH_P (x) ? code
					       : swap_condition (code),
					       GET_MODE (x), XEXP (x, 1),
					       XEXP (x, 0)), 1);
      x = *loc;
      code = GET_CODE (x);
    }

  /* Canonicalize arithmetics with all constant operands.  */
  switch (GET_RTX_CLASS (code))
    {
    case RTX_UNARY:
      if (CONSTANT_P (XEXP (x, 0)))
	new_rtx = simplify_unary_operation (code, GET_MODE (x), XEXP (x, 0),
					    op0_mode);
      break;
    case RTX_COMM_ARITH:
    case RTX_BIN_ARITH:
      if (CONSTANT_P (XEXP (x, 0)) && CONSTANT_P (XEXP (x, 1)))
	new_rtx = simplify_binary_operation (code, GET_MODE (x), XEXP (x, 0),
					     XEXP (x, 1));
      break;
    case RTX_COMPARE:
    case RTX_COMM_COMPARE:
      if (CONSTANT_P (XEXP (x, 0)) && CONSTANT_P (XEXP (x, 1)))
	new_rtx = simplify_relational_operation (code, GET_MODE (x), op0_mode,
						 XEXP (x, 0), XEXP (x, 1));
      break;
    default:
      break;
    }
  if (new_rtx)
    {
      validate_change (object, loc, new_rtx, 1);
      return;
    }

  switch (code)
    {
    case PLUS:
      /* If we have a PLUS whose second operand is now a CONST_INT, use
         simplify_gen_binary to try to simplify it.
         ??? We may want later to remove this, once simplification is
         separated from this function.  */
      if (CONST_INT_P (XEXP (x, 1)) && XEXP (x, 1) == to)
	validate_change (object, loc,
			 simplify_gen_binary
			 (PLUS, GET_MODE (x), XEXP (x, 0), XEXP (x, 1)), 1);
      break;
    case MINUS:
      if (CONST_SCALAR_INT_P (XEXP (x, 1)))
	validate_change (object, loc,
			 simplify_gen_binary
			 (PLUS, GET_MODE (x), XEXP (x, 0),
			  simplify_gen_unary (NEG,
					      GET_MODE (x), XEXP (x, 1),
					      GET_MODE (x))), 1);
      break;
    case ZERO_EXTEND:
    case SIGN_EXTEND:
      if (GET_MODE (XEXP (x, 0)) == VOIDmode)
	{
	  new_rtx = simplify_gen_unary (code, GET_MODE (x), XEXP (x, 0),
				    op0_mode);
	  /* If any of the above failed, substitute in something that
	     we know won't be recognized.  */
	  if (!new_rtx)
	    new_rtx = gen_rtx_CLOBBER (GET_MODE (x), const0_rtx);
	  validate_change (object, loc, new_rtx, 1);
	}
      break;
    case SUBREG:
      /* All subregs possible to simplify should be simplified.  */
      new_rtx = simplify_subreg (GET_MODE (x), SUBREG_REG (x), op0_mode,
			     SUBREG_BYTE (x));

      /* Subregs of VOIDmode operands are incorrect.  */
      if (!new_rtx && GET_MODE (SUBREG_REG (x)) == VOIDmode)
	new_rtx = gen_rtx_CLOBBER (GET_MODE (x), const0_rtx);
      if (new_rtx)
	validate_change (object, loc, new_rtx, 1);
      break;
    case ZERO_EXTRACT:
    case SIGN_EXTRACT:
      /* If we are replacing a register with memory, try to change the memory
         to be the mode required for memory in extract operations (this isn't
         likely to be an insertion operation; if it was, nothing bad will
         happen, we might just fail in some cases).  */

      if (MEM_P (XEXP (x, 0))
	  && is_a <scalar_int_mode> (GET_MODE (XEXP (x, 0)), &is_mode)
	  && CONST_INT_P (XEXP (x, 1))
	  && CONST_INT_P (XEXP (x, 2))
	  && !mode_dependent_address_p (XEXP (XEXP (x, 0), 0),
					MEM_ADDR_SPACE (XEXP (x, 0)))
	  && !MEM_VOLATILE_P (XEXP (x, 0)))
	{
	  int pos = INTVAL (XEXP (x, 2));
	  machine_mode new_mode = is_mode;
	  if (GET_CODE (x) == ZERO_EXTRACT && targetm.have_extzv ())
	    new_mode = insn_data[targetm.code_for_extzv].operand[1].mode;
	  else if (GET_CODE (x) == SIGN_EXTRACT && targetm.have_extv ())
	    new_mode = insn_data[targetm.code_for_extv].operand[1].mode;
	  scalar_int_mode wanted_mode = (new_mode == VOIDmode
					 ? word_mode
					 : as_a <scalar_int_mode> (new_mode));

	  /* If we have a narrower mode, we can do something.  */
	  if (GET_MODE_SIZE (wanted_mode) < GET_MODE_SIZE (is_mode))
	    {
	      int offset = pos / BITS_PER_UNIT;
	      rtx newmem;

	      /* If the bytes and bits are counted differently, we
	         must adjust the offset.  */
	      if (BYTES_BIG_ENDIAN != BITS_BIG_ENDIAN)
		offset =
		  (GET_MODE_SIZE (is_mode) - GET_MODE_SIZE (wanted_mode) -
		   offset);

	      gcc_assert (GET_MODE_PRECISION (wanted_mode)
			  == GET_MODE_BITSIZE (wanted_mode));
	      pos %= GET_MODE_BITSIZE (wanted_mode);

	      newmem = adjust_address_nv (XEXP (x, 0), wanted_mode, offset);

	      validate_change (object, &XEXP (x, 2), GEN_INT (pos), 1);
	      validate_change (object, &XEXP (x, 0), newmem, 1);
	    }
	}

      break;

    default:
      break;
    }
}

/* Replace every occurrence of FROM in X with TO.  Mark each change with
   validate_change passing OBJECT.  */

static void
validate_replace_rtx_1 (rtx *loc, rtx from, rtx to, rtx_insn *object,
                        bool simplify)
{
  int i, j;
  const char *fmt;
  rtx x = *loc;
  enum rtx_code code;
  machine_mode op0_mode = VOIDmode;
  int prev_changes = num_changes;

  if (!x)
    return;

  code = GET_CODE (x);
  fmt = GET_RTX_FORMAT (code);
  if (fmt[0] == 'e')
    op0_mode = GET_MODE (XEXP (x, 0));

  /* X matches FROM if it is the same rtx or they are both referring to the
     same register in the same mode.  Avoid calling rtx_equal_p unless the
     operands look similar.  */

  if (x == from
      || (REG_P (x) && REG_P (from)
	  && GET_MODE (x) == GET_MODE (from)
	  && REGNO (x) == REGNO (from))
      || (GET_CODE (x) == GET_CODE (from) && GET_MODE (x) == GET_MODE (from)
	  && rtx_equal_p (x, from)))
    {
      validate_unshare_change (object, loc, to, 1);
      return;
    }

  /* Call ourself recursively to perform the replacements.
     We must not replace inside already replaced expression, otherwise we
     get infinite recursion for replacements like (reg X)->(subreg (reg X))
     so we must special case shared ASM_OPERANDS.  */

  if (GET_CODE (x) == PARALLEL)
    {
      for (j = XVECLEN (x, 0) - 1; j >= 0; j--)
	{
	  if (j && GET_CODE (XVECEXP (x, 0, j)) == SET
	      && GET_CODE (SET_SRC (XVECEXP (x, 0, j))) == ASM_OPERANDS)
	    {
	      /* Verify that operands are really shared.  */
	      gcc_assert (ASM_OPERANDS_INPUT_VEC (SET_SRC (XVECEXP (x, 0, 0)))
			  == ASM_OPERANDS_INPUT_VEC (SET_SRC (XVECEXP
							      (x, 0, j))));
	      validate_replace_rtx_1 (&SET_DEST (XVECEXP (x, 0, j)),
				      from, to, object, simplify);
	    }
	  else
	    validate_replace_rtx_1 (&XVECEXP (x, 0, j), from, to, object,
                                    simplify);
	}
    }
  else
    for (i = GET_RTX_LENGTH (code) - 1; i >= 0; i--)
      {
	if (fmt[i] == 'e')
	  validate_replace_rtx_1 (&XEXP (x, i), from, to, object, simplify);
	else if (fmt[i] == 'E')
	  for (j = XVECLEN (x, i) - 1; j >= 0; j--)
	    validate_replace_rtx_1 (&XVECEXP (x, i, j), from, to, object,
                                    simplify);
      }

  /* If we didn't substitute, there is nothing more to do.  */
  if (num_changes == prev_changes)
    return;

  /* ??? The regmove is no more, so is this aberration still necessary?  */
  /* Allow substituted expression to have different mode.  This is used by
     regmove to change mode of pseudo register.  */
  if (fmt[0] == 'e' && GET_MODE (XEXP (x, 0)) != VOIDmode)
    op0_mode = GET_MODE (XEXP (x, 0));

  /* Do changes needed to keep rtx consistent.  Don't do any other
     simplifications, as it is not our job.  */
  if (simplify)
    simplify_while_replacing (loc, to, object, op0_mode);
}

/* Try replacing every occurrence of FROM in subexpression LOC of INSN
   with TO.  After all changes have been made, validate by seeing
   if INSN is still valid.  */

bool
validate_replace_rtx_subexp (rtx from, rtx to, rtx_insn *insn, rtx *loc)
{
  validate_replace_rtx_1 (loc, from, to, insn, true);
  return apply_change_group ();
}

/* Try replacing every occurrence of FROM in INSN with TO.  After all
   changes have been made, validate by seeing if INSN is still valid.  */

bool
validate_replace_rtx (rtx from, rtx to, rtx_insn *insn)
{
  validate_replace_rtx_1 (&PATTERN (insn), from, to, insn, true);
  return apply_change_group ();
}

/* Try replacing every occurrence of FROM in WHERE with TO.  Assume that WHERE
   is a part of INSN.  After all changes have been made, validate by seeing if
   INSN is still valid.
   validate_replace_rtx (from, to, insn) is equivalent to
   validate_replace_rtx_part (from, to, &PATTERN (insn), insn).  */

bool
validate_replace_rtx_part (rtx from, rtx to, rtx *where, rtx_insn *insn)
{
  validate_replace_rtx_1 (where, from, to, insn, true);
  return apply_change_group ();
}

/* Same as above, but do not simplify rtx afterwards.  */
bool
validate_replace_rtx_part_nosimplify (rtx from, rtx to, rtx *where,
				      rtx_insn *insn)
{
  validate_replace_rtx_1 (where, from, to, insn, false);
  return apply_change_group ();

}

/* Try replacing every occurrence of FROM in INSN with TO.  This also
   will replace in REG_EQUAL and REG_EQUIV notes.  */

void
validate_replace_rtx_group (rtx from, rtx to, rtx_insn *insn)
{
  rtx note;
  validate_replace_rtx_1 (&PATTERN (insn), from, to, insn, true);
  for (note = REG_NOTES (insn); note; note = XEXP (note, 1))
    if (REG_NOTE_KIND (note) == REG_EQUAL
	|| REG_NOTE_KIND (note) == REG_EQUIV)
      validate_replace_rtx_1 (&XEXP (note, 0), from, to, insn, true);
}

/* Function called by note_uses to replace used subexpressions.  */
struct validate_replace_src_data
{
  rtx from;			/* Old RTX */
  rtx to;			/* New RTX */
  rtx_insn *insn;		/* Insn in which substitution is occurring.  */
};

static void
validate_replace_src_1 (rtx *x, void *data)
{
  struct validate_replace_src_data *d
    = (struct validate_replace_src_data *) data;

  validate_replace_rtx_1 (x, d->from, d->to, d->insn, true);
}

/* Try replacing every occurrence of FROM in INSN with TO, avoiding
   SET_DESTs.  */

void
validate_replace_src_group (rtx from, rtx to, rtx_insn *insn)
{
  struct validate_replace_src_data d;

  d.from = from;
  d.to = to;
  d.insn = insn;
  note_uses (&PATTERN (insn), validate_replace_src_1, &d);
}

/* Try simplify INSN.
   Invoke simplify_rtx () on every SET_SRC and SET_DEST inside the INSN's
   pattern and return true if something was simplified.  */

bool
validate_simplify_insn (rtx_insn *insn)
{
  int i;
  rtx pat = NULL;
  rtx newpat = NULL;

  pat = PATTERN (insn);

  if (GET_CODE (pat) == SET)
    {
      newpat = simplify_rtx (SET_SRC (pat));
      if (newpat && !rtx_equal_p (SET_SRC (pat), newpat))
	validate_change (insn, &SET_SRC (pat), newpat, 1);
      newpat = simplify_rtx (SET_DEST (pat));
      if (newpat && !rtx_equal_p (SET_DEST (pat), newpat))
	validate_change (insn, &SET_DEST (pat), newpat, 1);
    }
  else if (GET_CODE (pat) == PARALLEL)
    for (i = 0; i < XVECLEN (pat, 0); i++)
      {
	rtx s = XVECEXP (pat, 0, i);

	if (GET_CODE (XVECEXP (pat, 0, i)) == SET)
	  {
	    newpat = simplify_rtx (SET_SRC (s));
	    if (newpat && !rtx_equal_p (SET_SRC (s), newpat))
	      validate_change (insn, &SET_SRC (s), newpat, 1);
	    newpat = simplify_rtx (SET_DEST (s));
	    if (newpat && !rtx_equal_p (SET_DEST (s), newpat))
	      validate_change (insn, &SET_DEST (s), newpat, 1);
	  }
      }
  return ((num_changes_pending () > 0) && (apply_change_group () > 0));
}

/* Try to process the address of memory expression MEM.  Return true on
   success; leave the caller to clean up on failure.  */

bool
insn_propagation::apply_to_mem_1 (rtx mem)
{
  auto old_num_changes = num_validated_changes ();
  mem_depth += 1;
  bool res = apply_to_rvalue_1 (&XEXP (mem, 0));
  mem_depth -= 1;
  if (!res)
    return false;

  if (old_num_changes != num_validated_changes ()
      && should_check_mems
      && !check_mem (old_num_changes, mem))
    return false;

  return true;
}

/* Try to process the rvalue expression at *LOC.  Return true on success;
   leave the caller to clean up on failure.  */

bool
insn_propagation::apply_to_rvalue_1 (rtx *loc)
{
  rtx x = *loc;
  enum rtx_code code = GET_CODE (x);
  machine_mode mode = GET_MODE (x);

  auto old_num_changes = num_validated_changes ();
  if (from
      && GET_CODE (x) == GET_CODE (from)
      && (REG_P (x)
	  ? REGNO (x) == REGNO (from)
	  : rtx_equal_p (x, from)))
    {
      /* Don't replace register asms in asm statements; we mustn't
	 change the user's register allocation.  */
      if (REG_P (x)
	  && HARD_REGISTER_P (x)
	  && register_asm_p (x)
	  && asm_noperands (PATTERN (insn)) > 0)
	return false;

      rtx newval = to;
      if (GET_MODE (x) != GET_MODE (from))
	{
	  gcc_assert (REG_P (x) && HARD_REGISTER_P (x));
	  if (REG_NREGS (x) != REG_NREGS (from)
	      || !REG_CAN_CHANGE_MODE_P (REGNO (x), GET_MODE (from),
					 GET_MODE (x)))
	    return false;

	  /* If the reference is paradoxical and the replacement
	     value contains registers, we would need to check that the
	     simplification below does not increase REG_NREGS for those
	     registers either.  It seems simpler to punt on nonconstant
	     values instead.  */
	  if (paradoxical_subreg_p (GET_MODE (x), GET_MODE (from))
	      && !CONSTANT_P (to))
	    return false;

	  newval = simplify_subreg (GET_MODE (x), to, GET_MODE (from),
				    subreg_lowpart_offset (GET_MODE (x),
							   GET_MODE (from)));
	  if (!newval)
	    return false;

	  /* Check that the simplification didn't just push an explicit
	     subreg down into subexpressions.  In particular, for a register
	     R that has a fixed mode, such as the stack pointer, a subreg of:

	       (plus:M (reg:M R) (const_int C))

	     would be:

	       (plus:N (subreg:N (reg:M R) ...) (const_int C'))

	     But targets can legitimately assume that subregs of hard registers
	     will not be created after RA (except in special circumstances,
	     such as strict_low_part).  */
	  subrtx_iterator::array_type array;
	  FOR_EACH_SUBRTX (iter, array, newval, NONCONST)
	    if (GET_CODE (*iter) == SUBREG)
	      return false;
	}

      if (should_unshare)
	validate_unshare_change (insn, loc, newval, 1);
      else
	validate_change (insn, loc, newval, 1);
      if (mem_depth && !REG_P (newval) && !CONSTANT_P (newval))
	{
	  /* We're substituting into an address, but TO will have the
	     form expected outside an address.  Canonicalize it if
	     necessary.  */
	  insn_propagation subprop (insn);
	  subprop.mem_depth += 1;
	  if (!subprop.apply_to_rvalue (loc))
	    gcc_unreachable ();
	  if (should_unshare
	      && num_validated_changes () != old_num_changes + 1)
	    {
	      /* TO is owned by someone else, so create a copy and
		 return TO to its original form.  */
	      newval = copy_rtx (*loc);
	      cancel_changes (old_num_changes);
	      validate_change (insn, loc, newval, 1);
	    }
	}
      num_replacements += 1;
      should_unshare = true;
      result_flags |= UNSIMPLIFIED;
      return true;
    }

  /* Recursively apply the substitution and see if we can simplify
     the result.  This specifically shouldn't use simplify_gen_* for
     speculative simplifications, since we want to avoid generating new
     expressions where possible.  */
  auto old_result_flags = result_flags;
  rtx newx = NULL_RTX;
  bool recurse_p = false;
  switch (GET_RTX_CLASS (code))
    {
    case RTX_UNARY:
      {
	machine_mode op0_mode = GET_MODE (XEXP (x, 0));
	if (!apply_to_rvalue_1 (&XEXP (x, 0)))
	  return false;
	if (from && old_num_changes == num_validated_changes ())
	  return true;

	newx = simplify_unary_operation (code, mode, XEXP (x, 0), op0_mode);
	break;
      }

    case RTX_BIN_ARITH:
    case RTX_COMM_ARITH:
      {
	if (!apply_to_rvalue_1 (&XEXP (x, 0))
	    || !apply_to_rvalue_1 (&XEXP (x, 1)))
	  return false;
	if (from && old_num_changes == num_validated_changes ())
	  return true;

	if (GET_RTX_CLASS (code) == RTX_COMM_ARITH
	    && swap_commutative_operands_p (XEXP (x, 0), XEXP (x, 1)))
	  newx = simplify_gen_binary (code, mode, XEXP (x, 1), XEXP (x, 0));
	else
	  newx = simplify_binary_operation (code, mode,
					    XEXP (x, 0), XEXP (x, 1));
	break;
      }

    case RTX_COMPARE:
    case RTX_COMM_COMPARE:
      {
	machine_mode op_mode = (GET_MODE (XEXP (x, 0)) != VOIDmode
				? GET_MODE (XEXP (x, 0))
				: GET_MODE (XEXP (x, 1)));
	if (!apply_to_rvalue_1 (&XEXP (x, 0))
	    || !apply_to_rvalue_1 (&XEXP (x, 1)))
	  return false;
	if (from && old_num_changes == num_validated_changes ())
	  return true;

	newx = simplify_relational_operation (code, mode, op_mode,
					      XEXP (x, 0), XEXP (x, 1));
	break;
      }

    case RTX_TERNARY:
    case RTX_BITFIELD_OPS:
      {
	machine_mode op0_mode = GET_MODE (XEXP (x, 0));
	if (!apply_to_rvalue_1 (&XEXP (x, 0))
	    || !apply_to_rvalue_1 (&XEXP (x, 1))
	    || !apply_to_rvalue_1 (&XEXP (x, 2)))
	  return false;
	if (from && old_num_changes == num_validated_changes ())
	  return true;

	newx = simplify_ternary_operation (code, mode, op0_mode,
					   XEXP (x, 0), XEXP (x, 1),
					   XEXP (x, 2));
	break;
      }

    case RTX_EXTRA:
      if (code == SUBREG)
	{
	  machine_mode inner_mode = GET_MODE (SUBREG_REG (x));
	  if (!apply_to_rvalue_1 (&SUBREG_REG (x)))
	    return false;
	  if (from && old_num_changes == num_validated_changes ())
	    return true;

	  rtx inner = SUBREG_REG (x);
	  newx = simplify_subreg (mode, inner, inner_mode, SUBREG_BYTE (x));
	  /* Reject the same cases that simplify_gen_subreg would.  */
	  if (!newx
	      && (GET_CODE (inner) == SUBREG
		  || GET_CODE (inner) == CONCAT
		  || GET_MODE (inner) == VOIDmode
		  || !validate_subreg (mode, inner_mode,
				       inner, SUBREG_BYTE (x))))
	    {
	      failure_reason = "would create an invalid subreg";
	      return false;
	    }
	  break;
	}
      else
	recurse_p = true;
      break;

    case RTX_OBJ:
      if (code == LO_SUM)
	{
	  if (!apply_to_rvalue_1 (&XEXP (x, 0))
	      || !apply_to_rvalue_1 (&XEXP (x, 1)))
	    return false;
	  if (from && old_num_changes == num_validated_changes ())
	    return true;

	  /* (lo_sum (high x) y) -> y where x and y have the same base.  */
	  rtx op0 = XEXP (x, 0);
	  rtx op1 = XEXP (x, 1);
	  if (GET_CODE (op0) == HIGH)
	    {
	      rtx base0, base1, offset0, offset1;
	      split_const (XEXP (op0, 0), &base0, &offset0);
	      split_const (op1, &base1, &offset1);
	      if (rtx_equal_p (base0, base1))
		newx = op1;
	    }
	}
      else if (code == REG)
	{
	  if (from && REG_P (from) && reg_overlap_mentioned_p (x, from))
	    {
	      failure_reason = "inexact register overlap";
	      return false;
	    }
	}
      else if (code == MEM)
	return apply_to_mem_1 (x);
      else
	recurse_p = true;
      break;

    case RTX_CONST_OBJ:
      break;

    case RTX_AUTOINC:
      if (from && reg_overlap_mentioned_p (XEXP (x, 0), from))
	{
	  failure_reason = "is subject to autoinc";
	  return false;
	}
      recurse_p = true;
      break;

    case RTX_MATCH:
    case RTX_INSN:
      gcc_unreachable ();
    }

  if (recurse_p)
    {
      const char *fmt = GET_RTX_FORMAT (code);
      for (int i = 0; fmt[i]; i++)
	switch (fmt[i])
	  {
	  case 'E':
	    for (int j = 0; j < XVECLEN (x, i); j++)
	      if (!apply_to_rvalue_1 (&XVECEXP (x, i, j)))
		return false;
	    break;

	  case 'e':
	    if (XEXP (x, i) && !apply_to_rvalue_1 (&XEXP (x, i)))
	      return false;
	    break;
	  }
    }
  else if (newx && !rtx_equal_p (x, newx))
    {
      /* All substitutions made by OLD_NUM_CHANGES onwards have been
	 simplified.  */
      result_flags = ((result_flags & ~UNSIMPLIFIED)
		      | (old_result_flags & UNSIMPLIFIED));

      if (should_note_simplifications)
	note_simplification (old_num_changes, old_result_flags, x, newx);

      /* There's no longer any point unsharing the substitutions made
	 for subexpressions, since we'll just copy this one instead.  */
      bool unshare = false;
      for (int i = old_num_changes; i < num_changes; ++i)
	{
	  unshare |= changes[i].unshare;
	  changes[i].unshare = false;
	}
      if (unshare)
	validate_unshare_change (insn, loc, newx, 1);
      else
	validate_change (insn, loc, newx, 1);
    }

  return true;
}

/* Try to process the lvalue expression at *LOC.  Return true on success;
   leave the caller to clean up on failure.  */

bool
insn_propagation::apply_to_lvalue_1 (rtx dest)
{
  rtx old_dest = dest;
  while (GET_CODE (dest) == SUBREG
	 || GET_CODE (dest) == ZERO_EXTRACT
	 || GET_CODE (dest) == STRICT_LOW_PART)
    {
      if (GET_CODE (dest) == ZERO_EXTRACT
	  && (!apply_to_rvalue_1 (&XEXP (dest, 1))
	      || !apply_to_rvalue_1 (&XEXP (dest, 2))))
	return false;
      dest = XEXP (dest, 0);
    }

  if (MEM_P (dest))
    return apply_to_mem_1 (dest);

  /* Check whether the substitution is safe in the presence of this lvalue.  */
  if (!from
      || dest == old_dest
      || !REG_P (dest)
      || !reg_overlap_mentioned_p (dest, from))
    return true;

  if (SUBREG_P (old_dest)
      && SUBREG_REG (old_dest) == dest
      && !read_modify_subreg_p (old_dest))
    return true;

  failure_reason = "is part of a read-write destination";
  return false;
}

/* Try to process the instruction pattern at *LOC.  Return true on success;
   leave the caller to clean up on failure.  */

bool
insn_propagation::apply_to_pattern_1 (rtx *loc)
{
  rtx body = *loc;
  switch (GET_CODE (body))
    {
    case COND_EXEC:
      return (apply_to_rvalue_1 (&COND_EXEC_TEST (body))
	      && apply_to_pattern_1 (&COND_EXEC_CODE (body)));

    case PARALLEL:
      for (int i = 0; i < XVECLEN (body, 0); ++i)
	{
	  rtx *subloc = &XVECEXP (body, 0, i);
	  if (GET_CODE (*subloc) == SET)
	    {
	      if (!apply_to_lvalue_1 (SET_DEST (*subloc)))
		return false;
	      /* ASM_OPERANDS are shared between SETs in the same PARALLEL.
		 Only process them on the first iteration.  */
	      if ((i == 0 || GET_CODE (SET_SRC (*subloc)) != ASM_OPERANDS)
		  && !apply_to_rvalue_1 (&SET_SRC (*subloc)))
		return false;
	    }
	  else
	    {
	      if (!apply_to_pattern_1 (subloc))
		return false;
	    }
	}
      return true;

    case ASM_OPERANDS:
      for (int i = 0, len = ASM_OPERANDS_INPUT_LENGTH (body); i < len; ++i)
	if (!apply_to_rvalue_1 (&ASM_OPERANDS_INPUT (body, i)))
	  return false;
      return true;

    case CLOBBER:
      return apply_to_lvalue_1 (XEXP (body, 0));

    case SET:
      return (apply_to_lvalue_1 (SET_DEST (body))
	      && apply_to_rvalue_1 (&SET_SRC (body)));

    default:
      /* All the other possibilities never store and can use a normal
	 rtx walk.  This includes:

	 - USE
	 - TRAP_IF
	 - PREFETCH
	 - UNSPEC
	 - UNSPEC_VOLATILE.  */
      return apply_to_rvalue_1 (loc);
    }
}

/* Apply this insn_propagation object's simplification or substitution
   to the instruction pattern at LOC.  */

bool
insn_propagation::apply_to_pattern (rtx *loc)
{
  unsigned int num_changes = num_validated_changes ();
  bool res = apply_to_pattern_1 (loc);
  if (!res)
    cancel_changes (num_changes);
  return res;
}

/* Apply this insn_propagation object's simplification or substitution
   to the rvalue expression at LOC.  */

bool
insn_propagation::apply_to_rvalue (rtx *loc)
{
  unsigned int num_changes = num_validated_changes ();
  bool res = apply_to_rvalue_1 (loc);
  if (!res)
    cancel_changes (num_changes);
  return res;
}

/* Like apply_to_rvalue, but specifically for the case where *LOC is in
   a note.  This never changes the INSN_CODE.  */

bool
insn_propagation::apply_to_note (rtx *loc)
{
  auto old_code = INSN_CODE (insn);
  bool res = apply_to_rvalue (loc);
  if (INSN_CODE (insn) != old_code)
    INSN_CODE (insn) = old_code;
  return res;
}

/* Check whether INSN matches a specific alternative of an .md pattern.  */

bool
valid_insn_p (rtx_insn *insn)
{
  recog_memoized (insn);
  if (INSN_CODE (insn) < 0)
    return false;
  extract_insn (insn);
  /* We don't know whether the insn will be in code that is optimized
     for size or speed, so consider all enabled alternatives.  */
  if (!constrain_operands (1, get_enabled_alternatives (insn)))
    return false;
  return true;
}

/* Return true if OP is a valid general operand for machine mode MODE.
   This is either a register reference, a memory reference,
   or a constant.  In the case of a memory reference, the address
   is checked for general validity for the target machine.

   Register and memory references must have mode MODE in order to be valid,
   but some constants have no machine mode and are valid for any mode.

   If MODE is VOIDmode, OP is checked for validity for whatever mode
   it has.

   The main use of this function is as a predicate in match_operand
   expressions in the machine description.  */

bool
general_operand (rtx op, machine_mode mode)
{
  enum rtx_code code = GET_CODE (op);

  if (mode == VOIDmode)
    mode = GET_MODE (op);

  /* Don't accept CONST_INT or anything similar
     if the caller wants something floating.  */
  if (GET_MODE (op) == VOIDmode && mode != VOIDmode
      && GET_MODE_CLASS (mode) != MODE_INT
      && GET_MODE_CLASS (mode) != MODE_PARTIAL_INT)
    return false;

  if (CONST_INT_P (op)
      && mode != VOIDmode
      && trunc_int_for_mode (INTVAL (op), mode) != INTVAL (op))
    return false;

  if (CONSTANT_P (op))
    return ((GET_MODE (op) == VOIDmode || GET_MODE (op) == mode
	     || mode == VOIDmode)
	    && (! flag_pic || LEGITIMATE_PIC_OPERAND_P (op))
	    && targetm.legitimate_constant_p (mode == VOIDmode
					      ? GET_MODE (op)
					      : mode, op));

  /* Except for certain constants with VOIDmode, already checked for,
     OP's mode must match MODE if MODE specifies a mode.  */

  if (GET_MODE (op) != mode)
    return false;

  if (code == SUBREG)
    {
      rtx sub = SUBREG_REG (op);

#ifdef INSN_SCHEDULING
      /* On machines that have insn scheduling, we want all memory
	 reference to be explicit, so outlaw paradoxical SUBREGs.
	 However, we must allow them after reload so that they can
	 get cleaned up by cleanup_subreg_operands.  */
      if (!reload_completed && MEM_P (sub)
	  && paradoxical_subreg_p (op))
	return false;
#endif
      /* Avoid memories with nonzero SUBREG_BYTE, as offsetting the memory
         may result in incorrect reference.  We should simplify all valid
         subregs of MEM anyway.  But allow this after reload because we
	 might be called from cleanup_subreg_operands.

	 ??? This is a kludge.  */
      if (!reload_completed
	  && maybe_ne (SUBREG_BYTE (op), 0)
	  && MEM_P (sub))
	return false;

      if (REG_P (sub)
	  && REGNO (sub) < FIRST_PSEUDO_REGISTER
	  && !REG_CAN_CHANGE_MODE_P (REGNO (sub), GET_MODE (sub), mode)
	  && GET_MODE_CLASS (GET_MODE (sub)) != MODE_COMPLEX_INT
	  && GET_MODE_CLASS (GET_MODE (sub)) != MODE_COMPLEX_FLOAT
	  /* LRA can generate some invalid SUBREGS just for matched
	     operand reload presentation.  LRA needs to treat them as
	     valid.  */
	  && ! LRA_SUBREG_P (op))
	return false;

      /* FLOAT_MODE subregs can't be paradoxical.  Combine will occasionally
	 create such rtl, and we must reject it.  */
      if (SCALAR_FLOAT_MODE_P (GET_MODE (op))
	  /* LRA can use subreg to store a floating point value in an
	     integer mode.  Although the floating point and the
	     integer modes need the same number of hard registers, the
	     size of floating point mode can be less than the integer
	     mode.  */
	  && ! lra_in_progress
	  && paradoxical_subreg_p (op))
	return false;

      op = sub;
      code = GET_CODE (op);
    }

  if (code == REG)
    return (REGNO (op) >= FIRST_PSEUDO_REGISTER
	    || in_hard_reg_set_p (operand_reg_set, GET_MODE (op), REGNO (op)));

  if (code == MEM)
    {
      rtx y = XEXP (op, 0);

      if (! volatile_ok && MEM_VOLATILE_P (op))
	return false;

      /* Use the mem's mode, since it will be reloaded thus.  LRA can
	 generate move insn with invalid addresses which is made valid
	 and efficiently calculated by LRA through further numerous
	 transformations.  */
      if (lra_in_progress
	  || memory_address_addr_space_p (GET_MODE (op), y, MEM_ADDR_SPACE (op)))
	return true;
    }

  return false;
}

/* Return true if OP is a valid memory address for a memory reference
   of mode MODE.

   The main use of this function is as a predicate in match_operand
   expressions in the machine description.  */

bool
address_operand (rtx op, machine_mode mode)
{
  /* Wrong mode for an address expr.  */
  if (GET_MODE (op) != VOIDmode
      && ! SCALAR_INT_MODE_P (GET_MODE (op)))
    return false;

  return memory_address_p (mode, op);
}

/* Return true if OP is a register reference of mode MODE.
   If MODE is VOIDmode, accept a register in any mode.

   The main use of this function is as a predicate in match_operand
   expressions in the machine description.  */

bool
register_operand (rtx op, machine_mode mode)
{
  if (GET_CODE (op) == SUBREG)
    {
      rtx sub = SUBREG_REG (op);

      /* Before reload, we can allow (SUBREG (MEM...)) as a register operand
	 because it is guaranteed to be reloaded into one.
	 Just make sure the MEM is valid in itself.
	 (Ideally, (SUBREG (MEM)...) should not exist after reload,
	 but currently it does result from (SUBREG (REG)...) where the
	 reg went on the stack.)  */
      if (!REG_P (sub) && (reload_completed || !MEM_P (sub)))
	return false;
    }
  else if (!REG_P (op))
    return false;
  return general_operand (op, mode);
}

/* Return true for a register in Pmode; ignore the tested mode.  */

bool
pmode_register_operand (rtx op, machine_mode mode ATTRIBUTE_UNUSED)
{
  return register_operand (op, Pmode);
}

/* Return true if OP should match a MATCH_SCRATCH, i.e., if it is a SCRATCH
   or a hard register.  */

bool
scratch_operand (rtx op, machine_mode mode)
{
  if (GET_MODE (op) != mode && mode != VOIDmode)
    return false;

  return (GET_CODE (op) == SCRATCH
	  || (REG_P (op)
	      && (lra_in_progress
		  || (REGNO (op) < FIRST_PSEUDO_REGISTER
		      && REGNO_REG_CLASS (REGNO (op)) != NO_REGS))));
}

/* Return true if OP is a valid immediate operand for mode MODE.

   The main use of this function is as a predicate in match_operand
   expressions in the machine description.  */

bool
immediate_operand (rtx op, machine_mode mode)
{
  /* Don't accept CONST_INT or anything similar
     if the caller wants something floating.  */
  if (GET_MODE (op) == VOIDmode && mode != VOIDmode
      && GET_MODE_CLASS (mode) != MODE_INT
      && GET_MODE_CLASS (mode) != MODE_PARTIAL_INT)
    return false;

  if (CONST_INT_P (op)
      && mode != VOIDmode
      && trunc_int_for_mode (INTVAL (op), mode) != INTVAL (op))
    return false;

  return (CONSTANT_P (op)
	  && (GET_MODE (op) == mode || mode == VOIDmode
	      || GET_MODE (op) == VOIDmode)
	  && (! flag_pic || LEGITIMATE_PIC_OPERAND_P (op))
	  && targetm.legitimate_constant_p (mode == VOIDmode
					    ? GET_MODE (op)
					    : mode, op));
}

/* Return true if OP is an operand that is a CONST_INT of mode MODE.  */

bool
const_int_operand (rtx op, machine_mode mode)
{
  if (!CONST_INT_P (op))
    return false;

  if (mode != VOIDmode
      && trunc_int_for_mode (INTVAL (op), mode) != INTVAL (op))
    return false;

  return true;
}

#if TARGET_SUPPORTS_WIDE_INT
/* Return true if OP is an operand that is a CONST_INT or CONST_WIDE_INT
   of mode MODE.  */
bool
const_scalar_int_operand (rtx op, machine_mode mode)
{
  if (!CONST_SCALAR_INT_P (op))
    return false;

  if (CONST_INT_P (op))
    return const_int_operand (op, mode);

  if (mode != VOIDmode)
    {
      scalar_int_mode int_mode = as_a <scalar_int_mode> (mode);
      int prec = GET_MODE_PRECISION (int_mode);
      int bitsize = GET_MODE_BITSIZE (int_mode);

      if (CONST_WIDE_INT_NUNITS (op) * HOST_BITS_PER_WIDE_INT > bitsize)
	return false;

      if (prec == bitsize)
	return true;
      else
	{
	  /* Multiword partial int.  */
	  HOST_WIDE_INT x
	    = CONST_WIDE_INT_ELT (op, CONST_WIDE_INT_NUNITS (op) - 1);
	  return (sext_hwi (x, prec & (HOST_BITS_PER_WIDE_INT - 1)) == x);
	}
    }
  return true;
}

/* Return true if OP is an operand that is a constant integer or constant
   floating-point number of MODE.  */

bool
const_double_operand (rtx op, machine_mode mode)
{
  return (GET_CODE (op) == CONST_DOUBLE)
	  && (GET_MODE (op) == mode || mode == VOIDmode);
}
#else
/* Return true if OP is an operand that is a constant integer or constant
   floating-point number of MODE.  */

bool
const_double_operand (rtx op, machine_mode mode)
{
  /* Don't accept CONST_INT or anything similar
     if the caller wants something floating.  */
  if (GET_MODE (op) == VOIDmode && mode != VOIDmode
      && GET_MODE_CLASS (mode) != MODE_INT
      && GET_MODE_CLASS (mode) != MODE_PARTIAL_INT)
    return false;

  return ((CONST_DOUBLE_P (op) || CONST_INT_P (op))
	  && (mode == VOIDmode || GET_MODE (op) == mode
	      || GET_MODE (op) == VOIDmode));
}
#endif
/* Return true if OP is a general operand that is not an immediate
   operand of mode MODE.  */

bool
nonimmediate_operand (rtx op, machine_mode mode)
{
  return (general_operand (op, mode) && ! CONSTANT_P (op));
}

/* Return true if OP is a register reference or
   immediate value of mode MODE.  */

bool
nonmemory_operand (rtx op, machine_mode mode)
{
  if (CONSTANT_P (op))
    return immediate_operand (op, mode);
  return register_operand (op, mode);
}

/* Return true if OP is a valid operand that stands for pushing a
   value of mode MODE onto the stack.

   The main use of this function is as a predicate in match_operand
   expressions in the machine description.  */

bool
push_operand (rtx op, machine_mode mode)
{
  if (!MEM_P (op))
    return false;

  if (mode != VOIDmode && GET_MODE (op) != mode)
    return false;

  poly_int64 rounded_size = GET_MODE_SIZE (mode);

#ifdef PUSH_ROUNDING
  rounded_size = PUSH_ROUNDING (MACRO_INT (rounded_size));
#endif

  op = XEXP (op, 0);

  if (known_eq (rounded_size, GET_MODE_SIZE (mode)))
    {
      if (GET_CODE (op) != STACK_PUSH_CODE)
	return false;
    }
  else
    {
      poly_int64 offset;
      if (GET_CODE (op) != PRE_MODIFY
	  || GET_CODE (XEXP (op, 1)) != PLUS
	  || XEXP (XEXP (op, 1), 0) != XEXP (op, 0)
	  || !poly_int_rtx_p (XEXP (XEXP (op, 1), 1), &offset)
	  || (STACK_GROWS_DOWNWARD
	      ? maybe_ne (offset, -rounded_size)
	      : maybe_ne (offset, rounded_size)))
	return false;
    }

  return XEXP (op, 0) == stack_pointer_rtx;
}

/* Return true if OP is a valid operand that stands for popping a
   value of mode MODE off the stack.

   The main use of this function is as a predicate in match_operand
   expressions in the machine description.  */

bool
pop_operand (rtx op, machine_mode mode)
{
  if (!MEM_P (op))
    return false;

  if (mode != VOIDmode && GET_MODE (op) != mode)
    return false;

  op = XEXP (op, 0);

  if (GET_CODE (op) != STACK_POP_CODE)
    return false;

  return XEXP (op, 0) == stack_pointer_rtx;
}

/* Return true if ADDR is a valid memory address
   for mode MODE in address space AS.  */

bool
memory_address_addr_space_p (machine_mode mode ATTRIBUTE_UNUSED, rtx addr,
			     addr_space_t as, code_helper ch ATTRIBUTE_UNUSED)
{
#ifdef GO_IF_LEGITIMATE_ADDRESS
  gcc_assert (ADDR_SPACE_GENERIC_P (as));
  GO_IF_LEGITIMATE_ADDRESS (mode, addr, win);
  return false;

 win:
  return true;
#else
  return targetm.addr_space.legitimate_address_p (mode, addr, 0, as, ch);
#endif
}

/* Return true if OP is a valid memory reference with mode MODE,
   including a valid address.

   The main use of this function is as a predicate in match_operand
   expressions in the machine description.  */

bool
memory_operand (rtx op, machine_mode mode)
{
  rtx inner;

  if (! reload_completed)
    /* Note that no SUBREG is a memory operand before end of reload pass,
       because (SUBREG (MEM...)) forces reloading into a register.  */
    return MEM_P (op) && general_operand (op, mode);

  if (mode != VOIDmode && GET_MODE (op) != mode)
    return false;

  inner = op;
  if (GET_CODE (inner) == SUBREG)
    inner = SUBREG_REG (inner);

  return (MEM_P (inner) && general_operand (op, mode));
}

/* Return true if OP is a valid indirect memory reference with mode MODE;
   that is, a memory reference whose address is a general_operand.  */

bool
indirect_operand (rtx op, machine_mode mode)
{
  /* Before reload, a SUBREG isn't in memory (see memory_operand, above).  */
  if (! reload_completed
      && GET_CODE (op) == SUBREG && MEM_P (SUBREG_REG (op)))
    {
      if (mode != VOIDmode && GET_MODE (op) != mode)
	return false;

      /* The only way that we can have a general_operand as the resulting
	 address is if OFFSET is zero and the address already is an operand
	 or if the address is (plus Y (const_int -OFFSET)) and Y is an
	 operand.  */
      poly_int64 offset;
      rtx addr = strip_offset (XEXP (SUBREG_REG (op), 0), &offset);
      return (known_eq (offset + SUBREG_BYTE (op), 0)
	      && general_operand (addr, Pmode));
    }

  return (MEM_P (op)
	  && memory_operand (op, mode)
	  && general_operand (XEXP (op, 0), Pmode));
}

/* Return true if this is an ordered comparison operator (not including
   ORDERED and UNORDERED).  */

bool
ordered_comparison_operator (rtx op, machine_mode mode)
{
  if (mode != VOIDmode && GET_MODE (op) != mode)
    return false;
  switch (GET_CODE (op))
    {
    case EQ:
    case NE:
    case LT:
    case LTU:
    case LE:
    case LEU:
    case GT:
    case GTU:
    case GE:
    case GEU:
      return true;
    default:
      return false;
    }
}

/* Return true if this is a comparison operator.  This allows the use of
   MATCH_OPERATOR to recognize all the branch insns.  */

bool
comparison_operator (rtx op, machine_mode mode)
{
  return ((mode == VOIDmode || GET_MODE (op) == mode)
	  && COMPARISON_P (op));
}

/* If BODY is an insn body that uses ASM_OPERANDS, return it.  */

rtx
extract_asm_operands (rtx body)
{
  rtx tmp;
  switch (GET_CODE (body))
    {
    case ASM_OPERANDS:
      return body;

    case SET:
      /* Single output operand: BODY is (set OUTPUT (asm_operands ...)).  */
      tmp = SET_SRC (body);
      if (GET_CODE (tmp) == ASM_OPERANDS)
	return tmp;
      break;

    case PARALLEL:
      tmp = XVECEXP (body, 0, 0);
      if (GET_CODE (tmp) == ASM_OPERANDS)
	return tmp;
      if (GET_CODE (tmp) == SET)
	{
	  tmp = SET_SRC (tmp);
	  if (GET_CODE (tmp) == ASM_OPERANDS)
	    return tmp;
	}
      break;

    default:
      break;
    }
  return NULL;
}

/* If BODY is an insn body that uses ASM_OPERANDS,
   return the number of operands (both input and output) in the insn.
   If BODY is an insn body that uses ASM_INPUT with CLOBBERS in PARALLEL,
   return 0.
   Otherwise return -1.  */

int
asm_noperands (const_rtx body)
{
  rtx asm_op = extract_asm_operands (CONST_CAST_RTX (body));
  int i, n_sets = 0;

  if (asm_op == NULL)
    {
      if (GET_CODE (body) == PARALLEL && XVECLEN (body, 0) >= 2
	  && GET_CODE (XVECEXP (body, 0, 0)) == ASM_INPUT)
	{
	  /* body is [(asm_input ...) (clobber (reg ...))...].  */
	  for (i = XVECLEN (body, 0) - 1; i > 0; i--)
	    if (GET_CODE (XVECEXP (body, 0, i)) != CLOBBER)
	      return -1;
	  return 0;
	}
      return -1;
    }

  if (GET_CODE (body) == SET)
    n_sets = 1;
  else if (GET_CODE (body) == PARALLEL)
    {
      if (GET_CODE (XVECEXP (body, 0, 0)) == SET)
	{
	  /* Multiple output operands, or 1 output plus some clobbers:
	     body is
	     [(set OUTPUT (asm_operands ...))...
	      (use (reg ...))...
	      (clobber (reg ...))...].  */
	  /* Count backwards through USEs and CLOBBERs to determine
	     number of SETs.  */
	  for (i = XVECLEN (body, 0); i > 0; i--)
	    {
	      if (GET_CODE (XVECEXP (body, 0, i - 1)) == SET)
		break;
	      if (GET_CODE (XVECEXP (body, 0, i - 1)) != USE
		  && GET_CODE (XVECEXP (body, 0, i - 1)) != CLOBBER)
		return -1;
	    }

	  /* N_SETS is now number of output operands.  */
	  n_sets = i;

	  /* Verify that all the SETs we have
	     came from a single original asm_operands insn
	     (so that invalid combinations are blocked).  */
	  for (i = 0; i < n_sets; i++)
	    {
	      rtx elt = XVECEXP (body, 0, i);
	      if (GET_CODE (elt) != SET)
		return -1;
	      if (GET_CODE (SET_SRC (elt)) != ASM_OPERANDS)
		return -1;
	      /* If these ASM_OPERANDS rtx's came from different original insns
	         then they aren't allowed together.  */
	      if (ASM_OPERANDS_INPUT_VEC (SET_SRC (elt))
		  != ASM_OPERANDS_INPUT_VEC (asm_op))
		return -1;
	    }
	}
      else
	{
	  /* 0 outputs, but some clobbers:
	     body is [(asm_operands ...)
		      (use (reg ...))...
		      (clobber (reg ...))...].  */
	  /* Make sure all the other parallel things really are clobbers.  */
	  for (i = XVECLEN (body, 0) - 1; i > 0; i--)
	    if (GET_CODE (XVECEXP (body, 0, i)) != USE
		&& GET_CODE (XVECEXP (body, 0, i)) != CLOBBER)
	      return -1;
	}
    }

  return (ASM_OPERANDS_INPUT_LENGTH (asm_op)
	  + ASM_OPERANDS_LABEL_LENGTH (asm_op) + n_sets);
}

/* Assuming BODY is an insn body that uses ASM_OPERANDS,
   copy its operands (both input and output) into the vector OPERANDS,
   the locations of the operands within the insn into the vector OPERAND_LOCS,
   and the constraints for the operands into CONSTRAINTS.
   Write the modes of the operands into MODES.
   Write the location info into LOC.
   Return the assembler-template.
   If BODY is an insn body that uses ASM_INPUT with CLOBBERS in PARALLEL,
   return the basic assembly string.

   If LOC, MODES, OPERAND_LOCS, CONSTRAINTS or OPERANDS is 0,
   we don't store that info.  */

const char *
decode_asm_operands (rtx body, rtx *operands, rtx **operand_locs,
		     const char **constraints, machine_mode *modes,
		     location_t *loc)
{
  int nbase = 0, n, i;
  rtx asmop;

  switch (GET_CODE (body))
    {
    case ASM_OPERANDS:
      /* Zero output asm: BODY is (asm_operands ...).  */
      asmop = body;
      break;

    case SET:
      /* Single output asm: BODY is (set OUTPUT (asm_operands ...)).  */
      asmop = SET_SRC (body);

      /* The output is in the SET.
	 Its constraint is in the ASM_OPERANDS itself.  */
      if (operands)
	operands[0] = SET_DEST (body);
      if (operand_locs)
	operand_locs[0] = &SET_DEST (body);
      if (constraints)
	constraints[0] = ASM_OPERANDS_OUTPUT_CONSTRAINT (asmop);
      if (modes)
	modes[0] = GET_MODE (SET_DEST (body));
      nbase = 1;
      break;

    case PARALLEL:
      {
	int nparallel = XVECLEN (body, 0); /* Includes CLOBBERs.  */

	asmop = XVECEXP (body, 0, 0);
	if (GET_CODE (asmop) == SET)
	  {
	    asmop = SET_SRC (asmop);

	    /* At least one output, plus some CLOBBERs.  The outputs are in
	       the SETs.  Their constraints are in the ASM_OPERANDS itself.  */
	    for (i = 0; i < nparallel; i++)
	      {
		if (GET_CODE (XVECEXP (body, 0, i)) == USE
		    || GET_CODE (XVECEXP (body, 0, i)) == CLOBBER)
		  break;		/* Past last SET */
		gcc_assert (GET_CODE (XVECEXP (body, 0, i)) == SET);
		if (operands)
		  operands[i] = SET_DEST (XVECEXP (body, 0, i));
		if (operand_locs)
		  operand_locs[i] = &SET_DEST (XVECEXP (body, 0, i));
		if (constraints)
		  constraints[i] = XSTR (SET_SRC (XVECEXP (body, 0, i)), 1);
		if (modes)
		  modes[i] = GET_MODE (SET_DEST (XVECEXP (body, 0, i)));
	      }
	    nbase = i;
	  }
	else if (GET_CODE (asmop) == ASM_INPUT)
	  {
	    if (loc)
	      *loc = ASM_INPUT_SOURCE_LOCATION (asmop);
	    return XSTR (asmop, 0);
	  }
	break;
      }

    default:
      gcc_unreachable ();
    }

  n = ASM_OPERANDS_INPUT_LENGTH (asmop);
  for (i = 0; i < n; i++)
    {
      if (operand_locs)
	operand_locs[nbase + i] = &ASM_OPERANDS_INPUT (asmop, i);
      if (operands)
	operands[nbase + i] = ASM_OPERANDS_INPUT (asmop, i);
      if (constraints)
	constraints[nbase + i] = ASM_OPERANDS_INPUT_CONSTRAINT (asmop, i);
      if (modes)
	modes[nbase + i] = ASM_OPERANDS_INPUT_MODE (asmop, i);
    }
  nbase += n;

  n = ASM_OPERANDS_LABEL_LENGTH (asmop);
  for (i = 0; i < n; i++)
    {
      if (operand_locs)
	operand_locs[nbase + i] = &ASM_OPERANDS_LABEL (asmop, i);
      if (operands)
	operands[nbase + i] = ASM_OPERANDS_LABEL (asmop, i);
      if (constraints)
	constraints[nbase + i] = "";
      if (modes)
	modes[nbase + i] = Pmode;
    }

  if (loc)
    *loc = ASM_OPERANDS_SOURCE_LOCATION (asmop);

  return ASM_OPERANDS_TEMPLATE (asmop);
}

/* Parse inline assembly string STRING and determine which operands are
   referenced by % markers.  For the first NOPERANDS operands, set USED[I]
   to true if operand I is referenced.

   This is intended to distinguish barrier-like asms such as:

      asm ("" : "=m" (...));

   from real references such as:

      asm ("sw\t$0, %0" : "=m" (...));  */

void
get_referenced_operands (const char *string, bool *used,
			 unsigned int noperands)
{
  memset (used, 0, sizeof (bool) * noperands);
  const char *p = string;
  while (*p)
    switch (*p)
      {
      case '%':
	p += 1;
	/* A letter followed by a digit indicates an operand number.  */
	if (ISALPHA (p[0]) && ISDIGIT (p[1]))
	  p += 1;
	if (ISDIGIT (*p))
	  {
	    char *endptr;
	    unsigned long opnum = strtoul (p, &endptr, 10);
	    if (endptr != p && opnum < noperands)
	      used[opnum] = true;
	    p = endptr;
	  }
	else
	  p += 1;
	break;

      default:
	p++;
	break;
      }
}

/* Check if an asm_operand matches its constraints.
   Return > 0 if ok, = 0 if bad, < 0 if inconclusive.  */

int
asm_operand_ok (rtx op, const char *constraint, const char **constraints)
{
  int result = 0;
  bool incdec_ok = false;

  /* Use constrain_operands after reload.  */
  gcc_assert (!reload_completed);

  /* Empty constraint string is the same as "X,...,X", i.e. X for as
     many alternatives as required to match the other operands.  */
  if (*constraint == '\0')
    result = 1;

  while (*constraint)
    {
      enum constraint_num cn;
      char c = *constraint;
      int len;
      switch (c)
	{
	case ',':
	  raw_constraint_p = false;
	  constraint++;
	  continue;

	case '0': case '1': case '2': case '3': case '4':
	case '5': case '6': case '7': case '8': case '9':
	  /* If caller provided constraints pointer, look up
	     the matching constraint.  Otherwise, our caller should have
	     given us the proper matching constraint, but we can't
	     actually fail the check if they didn't.  Indicate that
	     results are inconclusive.  */
	  if (constraints)
	    {
	      char *end;
	      unsigned long match;

	      match = strtoul (constraint, &end, 10);
	      if (!result)
		result = asm_operand_ok (op, constraints[match], NULL);
	      constraint = (const char *) end;
	    }
	  else
	    {
	      do
		constraint++;
	      while (ISDIGIT (*constraint));
	      if (! result)
		result = -1;
	    }
	  continue;

	  /* The rest of the compiler assumes that reloading the address
	     of a MEM into a register will make it fit an 'o' constraint.
	     That is, if it sees a MEM operand for an 'o' constraint,
	     it assumes that (mem (base-reg)) will fit.

	     That assumption fails on targets that don't have offsettable
	     addresses at all.  We therefore need to treat 'o' asm
	     constraints as a special case and only accept operands that
	     are already offsettable, thus proving that at least one
	     offsettable address exists.  */
	case 'o': /* offsettable */
	  if (offsettable_nonstrict_memref_p (op))
	    result = 1;
	  break;

	case 'g':
	  if (general_operand (op, VOIDmode))
	    result = 1;
	  break;

	case '-':
	  raw_constraint_p = true;
	  constraint++;
	  continue;

	case '<':
	case '>':
	  /* ??? Before auto-inc-dec, auto inc/dec insns are not supposed
	     to exist, excepting those that expand_call created.  Further,
	     on some machines which do not have generalized auto inc/dec,
	     an inc/dec is not a memory_operand.

	     Match any memory and hope things are resolved after reload.  */
	  incdec_ok = true;
	  /* FALLTHRU */
	default:
	  cn = lookup_constraint (constraint);
	  rtx mem = NULL;
	  switch (get_constraint_type (cn))
	    {
	    case CT_REGISTER:
	      if (!result
		  && (reg_class_for_constraint (cn) != NO_REGS
		      || constraint[0] == '{')
		  && GET_MODE (op) != BLKmode
		  && register_operand (op, VOIDmode))
		result = 1;
	      break;

	    case CT_CONST_INT:
	      if (!result
		  && CONST_INT_P (op)
		  && insn_const_int_ok_for_constraint (INTVAL (op), cn))
		result = 1;
	      break;

	    case CT_MEMORY:
	    case CT_RELAXED_MEMORY:
	      mem = op;
	      /* Fall through.  */
	    case CT_SPECIAL_MEMORY:
	      /* Every memory operand can be reloaded to fit.  */
	      if (!mem)
		mem = extract_mem_from_operand (op);
	      result = result || memory_operand (mem, VOIDmode);
	      break;

	    case CT_ADDRESS:
	      /* Every address operand can be reloaded to fit.  */
	      result = result || address_operand (op, VOIDmode);
	      break;

	    case CT_FIXED_FORM:
	      result = result || constraint_satisfied_p (op, cn);
	      break;
	    }
	  break;
	}
      len = CONSTRAINT_LEN (c, constraint);
      do
	constraint++;
      while (--len && *constraint && *constraint != ',');
      if (len)
	{
	  raw_constraint_p = false;
	  return 0;
	}
    }
  raw_constraint_p = false;

  /* For operands without < or > constraints reject side-effects.  */
  if (AUTO_INC_DEC && !incdec_ok && result && MEM_P (op))
    switch (GET_CODE (XEXP (op, 0)))
      {
      case PRE_INC:
      case POST_INC:
      case PRE_DEC:
      case POST_DEC:
      case PRE_MODIFY:
      case POST_MODIFY:
	return 0;
      default:
	break;
      }

  return result;
}

/* Given an rtx *P, if it is a sum containing an integer constant term,
   return the location (type rtx *) of the pointer to that constant term.
   Otherwise, return a null pointer.  */

rtx *
find_constant_term_loc (rtx *p)
{
  rtx *tem;
  enum rtx_code code = GET_CODE (*p);

  /* If *P IS such a constant term, P is its location.  */

  if (code == CONST_INT || code == SYMBOL_REF || code == LABEL_REF
      || code == CONST)
    return p;

  /* Otherwise, if not a sum, it has no constant term.  */

  if (GET_CODE (*p) != PLUS)
    return 0;

  /* If one of the summands is constant, return its location.  */

  if (XEXP (*p, 0) && CONSTANT_P (XEXP (*p, 0))
      && XEXP (*p, 1) && CONSTANT_P (XEXP (*p, 1)))
    return p;

  /* Otherwise, check each summand for containing a constant term.  */

  if (XEXP (*p, 0) != 0)
    {
      tem = find_constant_term_loc (&XEXP (*p, 0));
      if (tem != 0)
	return tem;
    }

  if (XEXP (*p, 1) != 0)
    {
      tem = find_constant_term_loc (&XEXP (*p, 1));
      if (tem != 0)
	return tem;
    }

  return 0;
}

/* Return true if OP is a memory reference whose address contains
   no side effects and remains valid after the addition of a positive
   integer less than the size of the object being referenced.

   We assume that the original address is valid and do not check it.

   This uses strict_memory_address_p as a subroutine, so
   don't use it before reload.  */

bool
offsettable_memref_p (rtx op)
{
  return ((MEM_P (op))
	  && offsettable_address_addr_space_p (1, GET_MODE (op), XEXP (op, 0),
					       MEM_ADDR_SPACE (op)));
}

/* Similar, but don't require a strictly valid mem ref:
   consider pseudo-regs valid as index or base regs.  */

bool
offsettable_nonstrict_memref_p (rtx op)
{
  return ((MEM_P (op))
	  && offsettable_address_addr_space_p (0, GET_MODE (op), XEXP (op, 0),
					       MEM_ADDR_SPACE (op)));
}

/* Return true if Y is a memory address which contains no side effects
   and would remain valid for address space AS after the addition of
   a positive integer less than the size of that mode.

   We assume that the original address is valid and do not check it.
   We do check that it is valid for narrower modes.

   If STRICTP is nonzero, we require a strictly valid address,
   for the sake of use in reload.cc.  */

bool
offsettable_address_addr_space_p (int strictp, machine_mode mode, rtx y,
				  addr_space_t as)
{
  enum rtx_code ycode = GET_CODE (y);
  rtx z;
  rtx y1 = y;
  rtx *y2;
  bool (*addressp) (machine_mode, rtx, addr_space_t, code_helper) =
    (strictp ? strict_memory_address_addr_space_p
	     : memory_address_addr_space_p);
  poly_int64 mode_sz = GET_MODE_SIZE (mode);

  if (CONSTANT_ADDRESS_P (y))
    return true;

  /* Adjusting an offsettable address involves changing to a narrower mode.
     Make sure that's OK.  */

  if (mode_dependent_address_p (y, as))
    return false;

  machine_mode address_mode = GET_MODE (y);
  if (address_mode == VOIDmode)
    address_mode = targetm.addr_space.address_mode (as);
#ifdef POINTERS_EXTEND_UNSIGNED
  machine_mode pointer_mode = targetm.addr_space.pointer_mode (as);
#endif

  /* ??? How much offset does an offsettable BLKmode reference need?
     Clearly that depends on the situation in which it's being used.
     However, the current situation in which we test 0xffffffff is
     less than ideal.  Caveat user.  */
  if (known_eq (mode_sz, 0))
    mode_sz = BIGGEST_ALIGNMENT / BITS_PER_UNIT;

  /* If the expression contains a constant term,
     see if it remains valid when max possible offset is added.  */

  if ((ycode == PLUS) && (y2 = find_constant_term_loc (&y1)))
    {
      bool good;

      y1 = *y2;
      *y2 = plus_constant (address_mode, *y2, mode_sz - 1);
      /* Use QImode because an odd displacement may be automatically invalid
	 for any wider mode.  But it should be valid for a single byte.  */
      good = (*addressp) (QImode, y, as, ERROR_MARK);

      /* In any case, restore old contents of memory.  */
      *y2 = y1;
      return good;
    }

  if (GET_RTX_CLASS (ycode) == RTX_AUTOINC)
    return false;

  /* The offset added here is chosen as the maximum offset that
     any instruction could need to add when operating on something
     of the specified mode.  We assume that if Y and Y+c are
     valid addresses then so is Y+d for all 0<d<c.  adjust_address will
     go inside a LO_SUM here, so we do so as well.  */
  if (GET_CODE (y) == LO_SUM
      && mode != BLKmode
      && known_le (mode_sz, GET_MODE_ALIGNMENT (mode) / BITS_PER_UNIT))
    z = gen_rtx_LO_SUM (address_mode, XEXP (y, 0),
			plus_constant (address_mode, XEXP (y, 1),
				       mode_sz - 1));
#ifdef POINTERS_EXTEND_UNSIGNED
  /* Likewise for a ZERO_EXTEND from pointer_mode.  */
  else if (POINTERS_EXTEND_UNSIGNED > 0
	   && GET_CODE (y) == ZERO_EXTEND
	   && GET_MODE (XEXP (y, 0)) == pointer_mode)
    z = gen_rtx_ZERO_EXTEND (address_mode,
			     plus_constant (pointer_mode, XEXP (y, 0),
					    mode_sz - 1));
#endif
  else
    z = plus_constant (address_mode, y, mode_sz - 1);

  /* Use QImode because an odd displacement may be automatically invalid
     for any wider mode.  But it should be valid for a single byte.  */
  return (*addressp) (QImode, z, as, ERROR_MARK);
}

/* Return true if ADDR is an address-expression whose effect depends
   on the mode of the memory reference it is used in.

   ADDRSPACE is the address space associated with the address.

   Autoincrement addressing is a typical example of mode-dependence
   because the amount of the increment depends on the mode.  */

bool
mode_dependent_address_p (rtx addr, addr_space_t addrspace)
{
  /* Auto-increment addressing with anything other than post_modify
     or pre_modify always introduces a mode dependency.  Catch such
     cases now instead of deferring to the target.  */
  if (GET_CODE (addr) == PRE_INC
      || GET_CODE (addr) == POST_INC
      || GET_CODE (addr) == PRE_DEC
      || GET_CODE (addr) == POST_DEC)
    return true;

  return targetm.mode_dependent_address_p (addr, addrspace);
}

/* Return true if boolean attribute ATTR is supported.  */

static bool
have_bool_attr (bool_attr attr)
{
  switch (attr)
    {
    case BA_ENABLED:
      return HAVE_ATTR_enabled;
    case BA_PREFERRED_FOR_SIZE:
      return HAVE_ATTR_enabled || HAVE_ATTR_preferred_for_size;
    case BA_PREFERRED_FOR_SPEED:
      return HAVE_ATTR_enabled || HAVE_ATTR_preferred_for_speed;
    }
  gcc_unreachable ();
}

/* Return the value of ATTR for instruction INSN.  */

static bool
get_bool_attr (rtx_insn *insn, bool_attr attr)
{
  switch (attr)
    {
    case BA_ENABLED:
      return get_attr_enabled (insn);
    case BA_PREFERRED_FOR_SIZE:
      return get_attr_enabled (insn) && get_attr_preferred_for_size (insn);
    case BA_PREFERRED_FOR_SPEED:
      return get_attr_enabled (insn) && get_attr_preferred_for_speed (insn);
    }
  gcc_unreachable ();
}

/* Like get_bool_attr_mask, but don't use the cache.  */

static alternative_mask
get_bool_attr_mask_uncached (rtx_insn *insn, bool_attr attr)
{
  /* Temporarily install enough information for get_attr_<foo> to assume
     that the insn operands are already cached.  As above, the attribute
     mustn't depend on the values of operands, so we don't provide their
     real values here.  */
  rtx_insn *old_insn = recog_data.insn;
  int old_alternative = which_alternative;

  recog_data.insn = insn;
  alternative_mask mask = ALL_ALTERNATIVES;
  int n_alternatives = insn_data[INSN_CODE (insn)].n_alternatives;
  for (int i = 0; i < n_alternatives; i++)
    {
      which_alternative = i;
      if (!get_bool_attr (insn, attr))
	mask &= ~ALTERNATIVE_BIT (i);
    }

  recog_data.insn = old_insn;
  which_alternative = old_alternative;
  return mask;
}

/* Return the mask of operand alternatives that are allowed for INSN
   by boolean attribute ATTR.  This mask depends only on INSN and on
   the current target; it does not depend on things like the values of
   operands.  */

static alternative_mask
get_bool_attr_mask (rtx_insn *insn, bool_attr attr)
{
  /* Quick exit for asms and for targets that don't use these attributes.  */
  int code = INSN_CODE (insn);
  if (code < 0 || !have_bool_attr (attr))
    return ALL_ALTERNATIVES;

  /* Calling get_attr_<foo> can be expensive, so cache the mask
     for speed.  */
  if (!this_target_recog->x_bool_attr_masks[code][attr])
    this_target_recog->x_bool_attr_masks[code][attr]
      = get_bool_attr_mask_uncached (insn, attr);
  return this_target_recog->x_bool_attr_masks[code][attr];
}

/* Return the set of alternatives of INSN that are allowed by the current
   target.  */

alternative_mask
get_enabled_alternatives (rtx_insn *insn)
{
  return get_bool_attr_mask (insn, BA_ENABLED);
}

/* Return the set of alternatives of INSN that are allowed by the current
   target and are preferred for the current size/speed optimization
   choice.  */

alternative_mask
get_preferred_alternatives (rtx_insn *insn)
{
  if (optimize_bb_for_speed_p (BLOCK_FOR_INSN (insn)))
    return get_bool_attr_mask (insn, BA_PREFERRED_FOR_SPEED);
  else
    return get_bool_attr_mask (insn, BA_PREFERRED_FOR_SIZE);
}

/* Return the set of alternatives of INSN that are allowed by the current
   target and are preferred for the size/speed optimization choice
   associated with BB.  Passing a separate BB is useful if INSN has not
   been emitted yet or if we are considering moving it to a different
   block.  */

alternative_mask
get_preferred_alternatives (rtx_insn *insn, basic_block bb)
{
  if (optimize_bb_for_speed_p (bb))
    return get_bool_attr_mask (insn, BA_PREFERRED_FOR_SPEED);
  else
    return get_bool_attr_mask (insn, BA_PREFERRED_FOR_SIZE);
}

/* Assert that the cached boolean attributes for INSN are still accurate.
   The backend is required to define these attributes in a way that only
   depends on the current target (rather than operands, compiler phase,
   etc.).  */

bool
check_bool_attrs (rtx_insn *insn)
{
  int code = INSN_CODE (insn);
  if (code >= 0)
    for (int i = 0; i <= BA_LAST; ++i)
      {
	enum bool_attr attr = (enum bool_attr) i;
	if (this_target_recog->x_bool_attr_masks[code][attr])
	  gcc_assert (this_target_recog->x_bool_attr_masks[code][attr]
		      == get_bool_attr_mask_uncached (insn, attr));
      }
  return true;
}

/* Like extract_insn, but save insn extracted and don't extract again, when
   called again for the same insn expecting that recog_data still contain the
   valid information.  This is used primary by gen_attr infrastructure that
   often does extract insn again and again.  */
void
extract_insn_cached (rtx_insn *insn)
{
  if (recog_data.insn == insn && INSN_CODE (insn) >= 0)
    return;
  extract_insn (insn);
  recog_data.insn = insn;
}

/* Do uncached extract_insn, constrain_operands and complain about failures.
   This should be used when extracting a pre-existing constrained instruction
   if the caller wants to know which alternative was chosen.  */
void
extract_constrain_insn (rtx_insn *insn)
{
  extract_insn (insn);
  if (!constrain_operands (reload_completed, get_enabled_alternatives (insn)))
    fatal_insn_not_found (insn);
}

/* Do cached extract_insn, constrain_operands and complain about failures.
   Used by insn_attrtab.  */
void
extract_constrain_insn_cached (rtx_insn *insn)
{
  extract_insn_cached (insn);
  if (which_alternative == -1
      && !constrain_operands (reload_completed,
			      get_enabled_alternatives (insn)))
    fatal_insn_not_found (insn);
}

/* Do cached constrain_operands on INSN and complain about failures.  */
bool
constrain_operands_cached (rtx_insn *insn, int strict)
{
  if (which_alternative == -1)
    return constrain_operands (strict, get_enabled_alternatives (insn));
  else
    return true;
}

/* Analyze INSN and fill in recog_data.  */

void
extract_insn (rtx_insn *insn)
{
  int i;
  int icode;
  int noperands;
  rtx body = PATTERN (insn);

  recog_data.n_operands = 0;
  recog_data.n_alternatives = 0;
  recog_data.n_dups = 0;
  recog_data.is_asm = false;

  switch (GET_CODE (body))
    {
    case USE:
    case CLOBBER:
    case ASM_INPUT:
    case ADDR_VEC:
    case ADDR_DIFF_VEC:
    case VAR_LOCATION:
    case DEBUG_MARKER:
      return;

    case SET:
      if (GET_CODE (SET_SRC (body)) == ASM_OPERANDS)
	goto asm_insn;
      else
	goto normal_insn;
    case PARALLEL:
      if ((GET_CODE (XVECEXP (body, 0, 0)) == SET
	   && GET_CODE (SET_SRC (XVECEXP (body, 0, 0))) == ASM_OPERANDS)
	  || GET_CODE (XVECEXP (body, 0, 0)) == ASM_OPERANDS
	  || GET_CODE (XVECEXP (body, 0, 0)) == ASM_INPUT)
	goto asm_insn;
      else
	goto normal_insn;
    case ASM_OPERANDS:
    asm_insn:
      recog_data.n_operands = noperands = asm_noperands (body);
      if (noperands >= 0)
	{
	  /* This insn is an `asm' with operands.  */

	  /* expand_asm_operands makes sure there aren't too many operands.  */
	  gcc_assert (noperands <= MAX_RECOG_OPERANDS);

	  /* Now get the operand values and constraints out of the insn.  */
	  decode_asm_operands (body, recog_data.operand,
			       recog_data.operand_loc,
			       recog_data.constraints,
			       recog_data.operand_mode, NULL);
	  memset (recog_data.is_operator, 0, sizeof recog_data.is_operator);
	  if (noperands > 0)
	    {
	      const char *p =  recog_data.constraints[0];
	      recog_data.n_alternatives = 1;
	      while (*p)
		recog_data.n_alternatives += (*p++ == ',');
	    }
	  recog_data.is_asm = true;
	  break;
	}
      fatal_insn_not_found (insn);

    default:
    normal_insn:
      /* Ordinary insn: recognize it, get the operands via insn_extract
	 and get the constraints.  */

      icode = recog_memoized (insn);
      if (icode < 0)
	fatal_insn_not_found (insn);

      recog_data.n_operands = noperands = insn_data[icode].n_operands;
      recog_data.n_alternatives = insn_data[icode].n_alternatives;
      recog_data.n_dups = insn_data[icode].n_dups;

      insn_extract (insn);

      for (i = 0; i < noperands; i++)
	{
	  recog_data.constraints[i] = insn_data[icode].operand[i].constraint;
	  recog_data.is_operator[i] = insn_data[icode].operand[i].is_operator;
	  recog_data.operand_mode[i] = insn_data[icode].operand[i].mode;
	  /* VOIDmode match_operands gets mode from their real operand.  */
	  if (recog_data.operand_mode[i] == VOIDmode)
	    recog_data.operand_mode[i] = GET_MODE (recog_data.operand[i]);
	}
    }
  for (i = 0; i < noperands; i++)
    recog_data.operand_type[i]
      = (recog_data.constraints[i][0] == '=' ? OP_OUT
	 : recog_data.constraints[i][0] == '+' ? OP_INOUT
	 : OP_IN);

  gcc_assert (recog_data.n_alternatives <= MAX_RECOG_ALTERNATIVES);

  recog_data.insn = NULL;
  which_alternative = -1;
}

/* Fill in OP_ALT_BASE for an instruction that has N_OPERANDS
   operands, N_ALTERNATIVES alternatives and constraint strings
   CONSTRAINTS.  OP_ALT_BASE has N_ALTERNATIVES * N_OPERANDS entries
   and CONSTRAINTS has N_OPERANDS entries.  OPLOC should be passed in
   if the insn is an asm statement and preprocessing should take the
   asm operands into account, e.g. to determine whether they could be
   addresses in constraints that require addresses; it should then
   point to an array of pointers to each operand.  */

void
preprocess_constraints (int n_operands, int n_alternatives,
			const char **constraints,
			operand_alternative *op_alt_base,
			rtx **oploc)
{
  for (int i = 0; i < n_operands; i++)
    {
      int j;
      struct operand_alternative *op_alt;
      const char *p = constraints[i];

      op_alt = op_alt_base;

      for (j = 0; j < n_alternatives; j++, op_alt += n_operands)
	{
	  op_alt[i].cl = NO_REGS;
	  op_alt[i].register_filters = 0;
	  op_alt[i].constraint = p;
	  op_alt[i].matches = -1;
	  op_alt[i].matched = -1;

	  if (*p == '\0' || *p == ',')
	    {
	      op_alt[i].anything_ok = 1;
	      continue;
	    }

	  for (;;)
	    {
	      char c = *p;
	      if (c == '#')
		do
		  c = *++p;
		while (c != ',' && c != '\0');
	      if (c == ',' || c == '\0')
		{
		  p++;
		  break;
		}

	      switch (c)
		{
		case '?':
		  op_alt[i].reject += 6;
		  break;
		case '!':
		  op_alt[i].reject += 600;
		  break;
		case '&':
		  op_alt[i].earlyclobber = 1;
		  break;

		case '0': case '1': case '2': case '3': case '4':
		case '5': case '6': case '7': case '8': case '9':
		  {
		    char *end;
		    op_alt[i].matches = strtoul (p, &end, 10);
		    op_alt[op_alt[i].matches].matched = i;
		    p = end;
		  }
		  continue;

		case 'X':
		  op_alt[i].anything_ok = 1;
		  break;

		case 'g':
		  op_alt[i].cl =
		   reg_class_subunion[(int) op_alt[i].cl][(int) GENERAL_REGS];
		  break;

		default:
		  enum constraint_num cn = lookup_constraint (p);
		  enum reg_class cl;
		  switch (get_constraint_type (cn))
		    {
		    case CT_REGISTER:
		      cl = reg_class_for_constraint (cn);
		      if (cl != NO_REGS)
			{
			  op_alt[i].cl = reg_class_subunion[op_alt[i].cl][cl];
			  auto filter_id = get_register_filter_id (cn);
			  if (filter_id >= 0)
			    op_alt[i].register_filters |= 1U << filter_id;
			}
		      break;

		    case CT_CONST_INT:
		      break;

		    case CT_MEMORY:
		    case CT_SPECIAL_MEMORY:
		    case CT_RELAXED_MEMORY:
		      op_alt[i].memory_ok = 1;
		      break;

		    case CT_ADDRESS:
		      if (oploc && !address_operand (*oploc[i], VOIDmode))
			break;

		      op_alt[i].is_address = 1;
		      op_alt[i].cl
			= (reg_class_subunion
			   [(int) op_alt[i].cl]
			   [(int) base_reg_class (VOIDmode, ADDR_SPACE_GENERIC,
						  ADDRESS, SCRATCH)]);
		      break;

		    case CT_FIXED_FORM:
		      break;
		    }
		  break;
		}
	      p += CONSTRAINT_LEN (c, p);
	    }
	}
    }
}

/* Return an array of operand_alternative instructions for
   instruction ICODE.  */

const operand_alternative *
preprocess_insn_constraints (unsigned int icode)
{
  gcc_checking_assert (IN_RANGE (icode, 0, NUM_INSN_CODES - 1));
  if (this_target_recog->x_op_alt[icode])
    return this_target_recog->x_op_alt[icode];

  int n_operands = insn_data[icode].n_operands;
  if (n_operands == 0)
    return 0;
  /* Always provide at least one alternative so that which_op_alt ()
     works correctly.  If the instruction has 0 alternatives (i.e. all
     constraint strings are empty) then each operand in this alternative
     will have anything_ok set.  */
  int n_alternatives = MAX (insn_data[icode].n_alternatives, 1);
  int n_entries = n_operands * n_alternatives;

  operand_alternative *op_alt = XCNEWVEC (operand_alternative, n_entries);
  const char **constraints = XALLOCAVEC (const char *, n_operands);

  for (int i = 0; i < n_operands; ++i)
    constraints[i] = insn_data[icode].operand[i].constraint;
  preprocess_constraints (n_operands, n_alternatives, constraints, op_alt,
			  NULL);

  this_target_recog->x_op_alt[icode] = op_alt;
  return op_alt;
}

/* After calling extract_insn, you can use this function to extract some
   information from the constraint strings into a more usable form.
   The collected data is stored in recog_op_alt.  */

void
preprocess_constraints (rtx_insn *insn)
{
  int icode = INSN_CODE (insn);
  if (icode >= 0)
    recog_op_alt = preprocess_insn_constraints (icode);
  else
    {
      int n_operands = recog_data.n_operands;
      int n_alternatives = recog_data.n_alternatives;
      int n_entries = n_operands * n_alternatives;
      memset (asm_op_alt, 0, n_entries * sizeof (operand_alternative));
      preprocess_constraints (n_operands, n_alternatives,
			      recog_data.constraints, asm_op_alt,
			      NULL);
      recog_op_alt = asm_op_alt;
    }
}

/* Check the operands of an insn against the insn's operand constraints
   and return 1 if they match any of the alternatives in ALTERNATIVES.

   The information about the insn's operands, constraints, operand modes
   etc. is obtained from the global variables set up by extract_insn.

   WHICH_ALTERNATIVE is set to a number which indicates which
   alternative of constraints was matched: 0 for the first alternative,
   1 for the next, etc.

   In addition, when two operands are required to match
   and it happens that the output operand is (reg) while the
   input operand is --(reg) or ++(reg) (a pre-inc or pre-dec),
   make the output operand look like the input.
   This is because the output operand is the one the template will print.

   This is used in final, just before printing the assembler code and by
   the routines that determine an insn's attribute.

   If STRICT is a positive nonzero value, it means that we have been
   called after reload has been completed.  In that case, we must
   do all checks strictly.  If it is zero, it means that we have been called
   before reload has completed.  In that case, we first try to see if we can
   find an alternative that matches strictly.  If not, we try again, this
   time assuming that reload will fix up the insn.  This provides a "best
   guess" for the alternative and is used to compute attributes of insns prior
   to reload.  A negative value of STRICT is used for this internal call.  */

struct funny_match
{
  int this_op, other;
};

bool
constrain_operands (int strict, alternative_mask alternatives)
{
  const char *constraints[MAX_RECOG_OPERANDS];
  int matching_operands[MAX_RECOG_OPERANDS];
  int earlyclobber[MAX_RECOG_OPERANDS];
  int c;

  struct funny_match funny_match[MAX_RECOG_OPERANDS];
  int funny_match_index;

  which_alternative = 0;
  if (recog_data.n_operands == 0 || recog_data.n_alternatives == 0)
    return true;

  for (c = 0; c < recog_data.n_operands; c++)
    constraints[c] = recog_data.constraints[c];

  do
    {
      int seen_earlyclobber_at = -1;
      int opno;
      bool lose = false;
      funny_match_index = 0;

      if (!TEST_BIT (alternatives, which_alternative))
	{
	  int i;

	  for (i = 0; i < recog_data.n_operands; i++)
	    constraints[i] = skip_alternative (constraints[i]);

	  which_alternative++;
	  continue;
	}

      for (opno = 0; opno < recog_data.n_operands; opno++)
	matching_operands[opno] = -1;

      for (opno = 0; opno < recog_data.n_operands; opno++)
	{
	  rtx op = recog_data.operand[opno];
	  machine_mode mode = GET_MODE (op);
	  const char *p = constraints[opno];
	  int offset = 0;
	  bool win = false;
	  int val;
	  int len;

	  earlyclobber[opno] = 0;

	  if (GET_CODE (op) == SUBREG)
	    {
	      if (REG_P (SUBREG_REG (op))
		  && REGNO (SUBREG_REG (op)) < FIRST_PSEUDO_REGISTER)
		offset = subreg_regno_offset (REGNO (SUBREG_REG (op)),
					      GET_MODE (SUBREG_REG (op)),
					      SUBREG_BYTE (op),
					      GET_MODE (op));
	      op = SUBREG_REG (op);
	    }

	  /* An empty constraint or empty alternative
	     allows anything which matched the pattern.  */
	  if (*p == 0 || *p == ',')
	    win = true;

	  do
	    switch (c = *p, len = CONSTRAINT_LEN (c, p), c)
	      {
	      case '\0':
		len = 0;
		break;
	      case ',':
		c = '\0';
		break;
	      case '-':
		raw_constraint_p = true;
		break;

	      case '#':
		/* Ignore rest of this alternative as far as
		   constraint checking is concerned.  */
		do
		  p++;
		while (*p && *p != ',');
		len = 0;
		break;

	      case '&':
		earlyclobber[opno] = 1;
		if (seen_earlyclobber_at < 0)
		  seen_earlyclobber_at = opno;
		break;

	      case '0':  case '1':  case '2':  case '3':  case '4':
	      case '5':  case '6':  case '7':  case '8':  case '9':
		{
		  /* This operand must be the same as a previous one.
		     This kind of constraint is used for instructions such
		     as add when they take only two operands.

		     Note that the lower-numbered operand is passed first.

		     If we are not testing strictly, assume that this
		     constraint will be satisfied.  */

		  char *end;
		  int match;

		  match = strtoul (p, &end, 10);
		  p = end;

		  if (strict < 0)
		    val = 1;
		  else
		    {
		      rtx op1 = recog_data.operand[match];
		      rtx op2 = recog_data.operand[opno];
		      val = operands_match_p (op1, op2);
		    }

		  matching_operands[opno] = match;
		  matching_operands[match] = opno;

		  if (val != 0)
		    win = true;

		  /* If output is *x and input is *--x, arrange later
		     to change the output to *--x as well, since the
		     output op is the one that will be printed.  */
		  if (val == 2 && strict > 0)
		    {
		      funny_match[funny_match_index].this_op = opno;
		      funny_match[funny_match_index++].other = match;
		    }
		}
		len = 0;
		break;

	      case 'p':
		/* p is used for address_operands.  When we are called by
		   gen_reload, no one will have checked that the address is
		   strictly valid, i.e., that all pseudos requiring hard regs
		   have gotten them.  We also want to make sure we have a
		   valid mode.  */
		{
		  auto mem_mode = (recog_data.is_asm
				   ? VOIDmode
				   : recog_data.operand_mode[opno]);
		  if ((GET_MODE (op) == VOIDmode
		       || SCALAR_INT_MODE_P (GET_MODE (op)))
		      && (strict <= 0
			  || strict_memory_address_p (mem_mode, op)))
		    win = true;
		  break;
		}

		/* No need to check general_operand again;
		   it was done in insn-recog.cc.  Well, except that reload
		   doesn't check the validity of its replacements, but
		   that should only matter when there's a bug.  */
	      case 'g':
		/* Anything goes unless it is a REG and really has a hard reg
		   but the hard reg is not in the class GENERAL_REGS.  */
		if (REG_P (op))
		  {
		    if (strict < 0
			|| GENERAL_REGS == ALL_REGS
			|| (reload_in_progress
			    && REGNO (op) >= FIRST_PSEUDO_REGISTER)
			|| reg_fits_class_p (op, GENERAL_REGS, offset, mode))
		      win = true;
		  }
		else if (strict < 0 || general_operand (op, mode))
		  win = true;
		break;

	      case '{':
		if ((REG_P (op) && HARD_REGISTER_P (op)
		     && (int) REGNO (op) == decode_hard_reg_constraint (p))
		    || !reload_completed)
		  win = true;
		break;

	      default:
		{
		  enum constraint_num cn = lookup_constraint (p);
		  enum reg_class cl = reg_class_for_constraint (cn);
		  if (cl != NO_REGS)
		    {
		      auto *filter = get_register_filter (cn);
		      if (strict < 0
			  || (strict == 0
			      && REG_P (op)
			      && REGNO (op) >= FIRST_PSEUDO_REGISTER)
			  || (strict == 0 && GET_CODE (op) == SCRATCH)
			  || (REG_P (op)
			      && reg_fits_class_p (op, cl, offset, mode)
			      && (!filter
				  || TEST_HARD_REG_BIT (*filter,
							REGNO (op) + offset))))
			win = true;
		    }

		  else if (constraint_satisfied_p (op, cn))
		    win = true;

		  else if ((insn_extra_memory_constraint (cn)
			    || insn_extra_relaxed_memory_constraint (cn))
			   /* Every memory operand can be reloaded to fit.  */
			   && ((strict < 0 && MEM_P (op))
			       /* Before reload, accept what reload can turn
				  into a mem.  */
			       || (strict < 0 && CONSTANT_P (op))
			       /* Before reload, accept a pseudo or hard register,
				  since LRA can turn it into a mem.  */
			       || (strict < 0 && targetm.lra_p () && REG_P (op))
			       /* During reload, accept a pseudo  */
			       || (reload_in_progress && REG_P (op)
				   && REGNO (op) >= FIRST_PSEUDO_REGISTER)))
		    win = true;
		  else if (insn_extra_address_constraint (cn)
			   /* Every address operand can be reloaded to fit.  */
			   && strict < 0)
		    win = true;
		  /* Cater to architectures like IA-64 that define extra memory
		     constraints without using define_memory_constraint.  */
		  else if (reload_in_progress
			   && REG_P (op)
			   && REGNO (op) >= FIRST_PSEUDO_REGISTER
			   && reg_renumber[REGNO (op)] < 0
			   && reg_equiv_mem (REGNO (op)) != 0
			   && constraint_satisfied_p
			      (reg_equiv_mem (REGNO (op)), cn))
		    win = true;
		  break;
		}
	      }
	  while (p += len, c);

	  raw_constraint_p = false;
	  constraints[opno] = p;
	  /* If this operand did not win somehow,
	     this alternative loses.  */
	  if (! win)
	    lose = true;
	}
      /* This alternative won; the operands are ok.
	 Change whichever operands this alternative says to change.  */
      if (! lose)
	{
	  int opno, eopno;

	  /* See if any earlyclobber operand conflicts with some other
	     operand.  */

	  if (strict > 0  && seen_earlyclobber_at >= 0)
	    for (eopno = seen_earlyclobber_at;
		 eopno < recog_data.n_operands;
		 eopno++)
	      /* Ignore earlyclobber operands now in memory,
		 because we would often report failure when we have
		 two memory operands, one of which was formerly a REG.  */
	      if (earlyclobber[eopno]
		  && REG_P (recog_data.operand[eopno]))
		for (opno = 0; opno < recog_data.n_operands; opno++)
		  if ((MEM_P (recog_data.operand[opno])
		       || recog_data.operand_type[opno] != OP_OUT)
		      && opno != eopno
		      /* Ignore things like match_operator operands.  */
		      && *recog_data.constraints[opno] != 0
		      && ! (matching_operands[opno] == eopno
			    && operands_match_p (recog_data.operand[opno],
						 recog_data.operand[eopno]))
		      && ! safe_from_earlyclobber (recog_data.operand[opno],
						   recog_data.operand[eopno]))
		    lose = true;

	  if (! lose)
	    {
	      while (--funny_match_index >= 0)
		{
		  recog_data.operand[funny_match[funny_match_index].other]
		    = recog_data.operand[funny_match[funny_match_index].this_op];
		}

	      /* For operands without < or > constraints reject side-effects.  */
	      if (AUTO_INC_DEC && recog_data.is_asm)
		{
		  for (opno = 0; opno < recog_data.n_operands; opno++)
		    if (MEM_P (recog_data.operand[opno]))
		      switch (GET_CODE (XEXP (recog_data.operand[opno], 0)))
			{
			case PRE_INC:
			case POST_INC:
			case PRE_DEC:
			case POST_DEC:
			case PRE_MODIFY:
			case POST_MODIFY:
			  if (strchr (recog_data.constraints[opno], '<') == NULL
			      && strchr (recog_data.constraints[opno], '>')
				 == NULL)
			    return false;
			  break;
			default:
			  break;
			}
		}

	      return true;
	    }
	}

      which_alternative++;
    }
  while (which_alternative < recog_data.n_alternatives);

  which_alternative = -1;
  /* If we are about to reject this, but we are not to test strictly,
     try a very loose test.  Only return failure if it fails also.  */
  if (strict == 0)
    return constrain_operands (-1, alternatives);
  else
    return false;
}

/* Return true iff OPERAND (assumed to be a REG rtx)
   is a hard reg in class CLASS when its regno is offset by OFFSET
   and changed to mode MODE.
   If REG occupies multiple hard regs, all of them must be in CLASS.  */

bool
reg_fits_class_p (const_rtx operand, reg_class_t cl, int offset,
		  machine_mode mode)
{
  unsigned int regno = REGNO (operand);

  if (cl == NO_REGS)
    return false;

  /* Regno must not be a pseudo register.  Offset may be negative.  */
  return (HARD_REGISTER_NUM_P (regno)
	  && HARD_REGISTER_NUM_P (regno + offset)
	  && in_hard_reg_set_p (reg_class_contents[(int) cl], mode,
				regno + offset));
}

/* Split single instruction.  Helper function for split_all_insns and
   split_all_insns_noflow.  Return last insn in the sequence if successful,
   or NULL if unsuccessful.  */

static rtx_insn *
split_insn (rtx_insn *insn)
{
  /* Split insns here to get max fine-grain parallelism.  */
  rtx_insn *first = PREV_INSN (insn);
  rtx_insn *last = try_split (PATTERN (insn), insn, 1);
  rtx insn_set, last_set, note;

  if (last == insn)
    return NULL;

  /* If the original instruction was a single set that was known to be
     equivalent to a constant, see if we can say the same about the last
     instruction in the split sequence.  The two instructions must set
     the same destination.  */
  insn_set = single_set (insn);
  if (insn_set)
    {
      last_set = single_set (last);
      if (last_set && rtx_equal_p (SET_DEST (last_set), SET_DEST (insn_set)))
	{
	  note = find_reg_equal_equiv_note (insn);
	  if (note && CONSTANT_P (XEXP (note, 0)))
	    set_unique_reg_note (last, REG_EQUAL, XEXP (note, 0));
	  else if (CONSTANT_P (SET_SRC (insn_set)))
	    set_unique_reg_note (last, REG_EQUAL,
				 copy_rtx (SET_SRC (insn_set)));
	}
    }

  /* try_split returns the NOTE that INSN became.  */
  SET_INSN_DELETED (insn);

  /* ??? Coddle to md files that generate subregs in post-reload
     splitters instead of computing the proper hard register.  */
  if (reload_completed && first != last)
    {
      first = NEXT_INSN (first);
      for (;;)
	{
	  if (INSN_P (first))
	    cleanup_subreg_operands (first);
	  if (first == last)
	    break;
	  first = NEXT_INSN (first);
	}
    }

  return last;
}

/* Split all insns in the function.  If UPD_LIFE, update life info after.  */

void
split_all_insns (void)
{
  bool changed;
  bool need_cfg_cleanup = false;
  basic_block bb;

  auto_sbitmap blocks (last_basic_block_for_fn (cfun));
  bitmap_clear (blocks);
  changed = false;

  FOR_EACH_BB_REVERSE_FN (bb, cfun)
    {
      rtx_insn *insn, *next;
      bool finish = false;

      rtl_profile_for_bb (bb);
      for (insn = BB_HEAD (bb); !finish ; insn = next)
	{
	  /* Can't use `next_real_insn' because that might go across
	     CODE_LABELS and short-out basic blocks.  */
	  next = NEXT_INSN (insn);
	  finish = (insn == BB_END (bb));

	  /* If INSN has a REG_EH_REGION note and we split INSN, the
	     resulting split may not have/need REG_EH_REGION notes.

	     If that happens and INSN was the last reference to the
	     given EH region, then the EH region will become unreachable.
	     We cannot leave the unreachable blocks in the CFG as that
	     will trigger a checking failure.

	     So track if INSN has a REG_EH_REGION note.  If so and we
	     split INSN, then trigger a CFG cleanup.  */
	  rtx note = find_reg_note (insn, REG_EH_REGION, NULL_RTX);
	  if (INSN_P (insn))
	    {
	      rtx set = single_set (insn);

	      /* Don't split no-op move insns.  These should silently
		 disappear later in final.  Splitting such insns would
		 break the code that handles LIBCALL blocks.  */
	      if (set && set_noop_p (set))
		{
		  /* Nops get in the way while scheduling, so delete them
		     now if register allocation has already been done.  It
		     is too risky to try to do this before register
		     allocation, and there are unlikely to be very many
		     nops then anyways.  */
		  if (reload_completed)
		      delete_insn_and_edges (insn);
		  if (note)
		    need_cfg_cleanup = true;
		}
	      else
		{
		  if (split_insn (insn))
		    {
		      bitmap_set_bit (blocks, bb->index);
		      changed = true;
		      if (note)
			need_cfg_cleanup = true;
		    }
		}
	    }
	}
    }

  default_rtl_profile ();
  if (changed)
    {
      find_many_sub_basic_blocks (blocks);

      /* Splitting could drop an REG_EH_REGION if it potentially
	 trapped in its original form, but does not in its split
	 form.  Consider a FLOAT_TRUNCATE which splits into a memory
	 store/load pair and -fnon-call-exceptions.  */
      if (need_cfg_cleanup)
	cleanup_cfg (0);
    }

  checking_verify_flow_info ();
}

/* Same as split_all_insns, but do not expect CFG to be available.
   Used by machine dependent reorg passes.  */

void
split_all_insns_noflow (void)
{
  rtx_insn *next, *insn;

  for (insn = get_insns (); insn; insn = next)
    {
      next = NEXT_INSN (insn);
      if (INSN_P (insn))
	{
	  /* Don't split no-op move insns.  These should silently
	     disappear later in final.  Splitting such insns would
	     break the code that handles LIBCALL blocks.  */
	  rtx set = single_set (insn);
	  if (set && set_noop_p (set))
	    {
	      /* Nops get in the way while scheduling, so delete them
		 now if register allocation has already been done.  It
		 is too risky to try to do this before register
		 allocation, and there are unlikely to be very many
		 nops then anyways.

		 ??? Should we use delete_insn when the CFG isn't valid?  */
	      if (reload_completed)
		delete_insn_and_edges (insn);
	    }
	  else
	    split_insn (insn);
	}
    }
}

struct peep2_insn_data
{
  rtx_insn *insn;
  regset live_before;
};

static struct peep2_insn_data peep2_insn_data[MAX_INSNS_PER_PEEP2 + 1];
static int peep2_current;

static bool peep2_do_rebuild_jump_labels;
static bool peep2_do_cleanup_cfg;

/* The number of instructions available to match a peep2.  */
int peep2_current_count;

/* A marker indicating the last insn of the block.  The live_before regset
   for this element is correct, indicating DF_LIVE_OUT for the block.  */
#define PEEP2_EOB invalid_insn_rtx

/* Wrap N to fit into the peep2_insn_data buffer.  */

static int
peep2_buf_position (int n)
{
  if (n >= MAX_INSNS_PER_PEEP2 + 1)
    n -= MAX_INSNS_PER_PEEP2 + 1;
  return n;
}

/* Return the Nth non-note insn after `current', or return NULL_RTX if it
   does not exist.  Used by the recognizer to find the next insn to match
   in a multi-insn pattern.  */

rtx_insn *
peep2_next_insn (int n)
{
  gcc_assert (n <= peep2_current_count);

  n = peep2_buf_position (peep2_current + n);

  return peep2_insn_data[n].insn;
}

/* Return true if REGNO is dead before the Nth non-note insn
   after `current'.  */

bool
peep2_regno_dead_p (int ofs, int regno)
{
  gcc_assert (ofs < MAX_INSNS_PER_PEEP2 + 1);

  ofs = peep2_buf_position (peep2_current + ofs);

  gcc_assert (peep2_insn_data[ofs].insn != NULL_RTX);

  return ! REGNO_REG_SET_P (peep2_insn_data[ofs].live_before, regno);
}

/* Similarly for a REG.  */

bool
peep2_reg_dead_p (int ofs, rtx reg)
{
  gcc_assert (ofs < MAX_INSNS_PER_PEEP2 + 1);

  ofs = peep2_buf_position (peep2_current + ofs);

  gcc_assert (peep2_insn_data[ofs].insn != NULL_RTX);

  unsigned int end_regno = END_REGNO (reg);
  for (unsigned int regno = REGNO (reg); regno < end_regno; ++regno)
    if (REGNO_REG_SET_P (peep2_insn_data[ofs].live_before, regno))
      return false;
  return true;
}

/* Regno offset to be used in the register search.  */
static int search_ofs;

/* Try to find a hard register of mode MODE, matching the register class in
   CLASS_STR, which is available at the beginning of insn CURRENT_INSN and
   remains available until the end of LAST_INSN.  LAST_INSN may be NULL_RTX,
   in which case the only condition is that the register must be available
   before CURRENT_INSN.
   Registers that already have bits set in REG_SET will not be considered.

   If an appropriate register is available, it will be returned and the
   corresponding bit(s) in REG_SET will be set; otherwise, NULL_RTX is
   returned.  */

rtx
peep2_find_free_register (int from, int to, const char *class_str,
			  machine_mode mode, HARD_REG_SET *reg_set)
{
  enum reg_class cl;
  HARD_REG_SET live;
  df_ref def;
  int i;

  gcc_assert (from < MAX_INSNS_PER_PEEP2 + 1);
  gcc_assert (to < MAX_INSNS_PER_PEEP2 + 1);

  from = peep2_buf_position (peep2_current + from);
  to = peep2_buf_position (peep2_current + to);

  gcc_assert (peep2_insn_data[from].insn != NULL_RTX);
  REG_SET_TO_HARD_REG_SET (live, peep2_insn_data[from].live_before);

  while (from != to)
    {
      gcc_assert (peep2_insn_data[from].insn != NULL_RTX);

      /* Don't use registers set or clobbered by the insn.  */
      FOR_EACH_INSN_DEF (def, peep2_insn_data[from].insn)
	SET_HARD_REG_BIT (live, DF_REF_REGNO (def));

      from = peep2_buf_position (from + 1);
    }

  cl = reg_class_for_constraint (lookup_constraint (class_str));

  for (i = 0; i < FIRST_PSEUDO_REGISTER; i++)
    {
      int raw_regno, regno, j;
      bool success;

      /* Distribute the free registers as much as possible.  */
      raw_regno = search_ofs + i;
      if (raw_regno >= FIRST_PSEUDO_REGISTER)
	raw_regno -= FIRST_PSEUDO_REGISTER;
#ifdef REG_ALLOC_ORDER
      regno = reg_alloc_order[raw_regno];
#else
      regno = raw_regno;
#endif

      /* Can it support the mode we need?  */
      if (!targetm.hard_regno_mode_ok (regno, mode))
	continue;

      success = true;
      for (j = 0; success && j < hard_regno_nregs (regno, mode); j++)
	{
	  /* Don't allocate fixed registers.  */
	  if (fixed_regs[regno + j])
	    {
	      success = false;
	      break;
	    }
	  /* Don't allocate global registers.  */
	  if (global_regs[regno + j])
	    {
	      success = false;
	      break;
	    }
	  /* Make sure the register is of the right class.  */
	  if (! TEST_HARD_REG_BIT (reg_class_contents[cl], regno + j))
	    {
	      success = false;
	      break;
	    }
	  /* And that we don't create an extra save/restore.  */
	  if (! crtl->abi->clobbers_full_reg_p (regno + j)
	      && ! df_regs_ever_live_p (regno + j))
	    {
	      success = false;
	      break;
	    }

	  if (! targetm.hard_regno_scratch_ok (regno + j))
	    {
	      success = false;
	      break;
	    }

	  /* And we don't clobber traceback for noreturn functions.  */
	  if ((regno + j == FRAME_POINTER_REGNUM
	       || regno + j == HARD_FRAME_POINTER_REGNUM)
	      && (! reload_completed || frame_pointer_needed))
	    {
	      success = false;
	      break;
	    }

	  if (TEST_HARD_REG_BIT (*reg_set, regno + j)
	      || TEST_HARD_REG_BIT (live, regno + j))
	    {
	      success = false;
	      break;
	    }
	}

      if (success)
	{
	  add_to_hard_reg_set (reg_set, mode, regno);

	  /* Start the next search with the next register.  */
	  if (++raw_regno >= FIRST_PSEUDO_REGISTER)
	    raw_regno = 0;
	  search_ofs = raw_regno;

	  return gen_rtx_REG (mode, regno);
	}
    }

  search_ofs = 0;
  return NULL_RTX;
}

/* Forget all currently tracked instructions, only remember current
   LIVE regset.  */

static void
peep2_reinit_state (regset live)
{
  int i;

  /* Indicate that all slots except the last holds invalid data.  */
  for (i = 0; i < MAX_INSNS_PER_PEEP2; ++i)
    peep2_insn_data[i].insn = NULL;
  peep2_current_count = 0;

  /* Indicate that the last slot contains live_after data.  */
  peep2_insn_data[MAX_INSNS_PER_PEEP2].insn = PEEP2_EOB;
  peep2_current = MAX_INSNS_PER_PEEP2;

  COPY_REG_SET (peep2_insn_data[MAX_INSNS_PER_PEEP2].live_before, live);
}

/* Copies frame related info of an insn (OLD_INSN) to the single
   insn (NEW_INSN) that was obtained by splitting OLD_INSN.  */

void
copy_frame_info_to_split_insn (rtx_insn *old_insn, rtx_insn *new_insn)
{
  bool any_note = false;
  rtx note;

  if (!RTX_FRAME_RELATED_P (old_insn))
    return;

  RTX_FRAME_RELATED_P (new_insn) = 1;

  /* Allow the backend to fill in a note during the split.  */
  for (note = REG_NOTES (new_insn); note ; note = XEXP (note, 1))
    switch (REG_NOTE_KIND (note))
      {
      case REG_FRAME_RELATED_EXPR:
      case REG_CFA_DEF_CFA:
      case REG_CFA_ADJUST_CFA:
      case REG_CFA_OFFSET:
      case REG_CFA_REGISTER:
      case REG_CFA_EXPRESSION:
      case REG_CFA_RESTORE:
      case REG_CFA_SET_VDRAP:
        any_note = true;
        break;
      default:
        break;
      }

  /* If the backend didn't supply a note, copy one over.  */
  if (!any_note)
    for (note = REG_NOTES (old_insn); note ; note = XEXP (note, 1))
      switch (REG_NOTE_KIND (note))
        {
        case REG_FRAME_RELATED_EXPR:
        case REG_CFA_DEF_CFA:
        case REG_CFA_ADJUST_CFA:
        case REG_CFA_OFFSET:
        case REG_CFA_REGISTER:
        case REG_CFA_EXPRESSION:
        case REG_CFA_RESTORE:
        case REG_CFA_SET_VDRAP:
          add_reg_note (new_insn, REG_NOTE_KIND (note), XEXP (note, 0));
          any_note = true;
          break;
        default:
          break;
        }

  /* If there still isn't a note, make sure the unwind info sees the
     same expression as before the split.  */
  if (!any_note)
    {
      rtx old_set, new_set;

      /* The old insn had better have been simple, or annotated.  */
      old_set = single_set (old_insn);
      gcc_assert (old_set != NULL);

      new_set = single_set (new_insn);
      if (!new_set || !rtx_equal_p (new_set, old_set))
        add_reg_note (new_insn, REG_FRAME_RELATED_EXPR, old_set);
    }

  /* Copy prologue/epilogue status.  This is required in order to keep
     proper placement of EPILOGUE_BEG and the DW_CFA_remember_state.  */
  maybe_copy_prologue_epilogue_insn (old_insn, new_insn);
}

/* While scanning basic block BB, we found a match of length MATCH_LEN + 1,
   starting at INSN.  Perform the replacement, removing the old insns and
   replacing them with ATTEMPT.  Returns the last insn emitted, or NULL
   if the replacement is rejected.  */

static rtx_insn *
peep2_attempt (basic_block bb, rtx_insn *insn, int match_len, rtx_insn *attempt)
{
  int i;
  rtx_insn *last, *before_try, *x;
  rtx eh_note, as_note;
  rtx_insn *old_insn;
  rtx_insn *new_insn;
  bool was_call = false;

  /* If we are splitting an RTX_FRAME_RELATED_P insn, do not allow it to
     match more than one insn, or to be split into more than one insn.  */
  old_insn = peep2_insn_data[peep2_current].insn;
  if (RTX_FRAME_RELATED_P (old_insn))
    {
      if (match_len != 0)
	return NULL;

      /* Look for one "active" insn.  I.e. ignore any "clobber" insns that
	 may be in the stream for the purpose of register allocation.  */
      if (active_insn_p (attempt))
	new_insn = attempt;
      else
	new_insn = next_active_insn (attempt);
      if (next_active_insn (new_insn))
	return NULL;

      /* We have a 1-1 replacement.  Copy over any frame-related info.  */
      copy_frame_info_to_split_insn (old_insn, new_insn);
    }

  /* If we are splitting a CALL_INSN, look for the CALL_INSN
     in SEQ and copy our CALL_INSN_FUNCTION_USAGE and other
     cfg-related call notes.  */
  for (i = 0; i <= match_len; ++i)
    {
      int j;
      rtx note;

      j = peep2_buf_position (peep2_current + i);
      old_insn = peep2_insn_data[j].insn;
      if (!CALL_P (old_insn))
	continue;
      was_call = true;

      new_insn = attempt;
      while (new_insn != NULL_RTX)
	{
	  if (CALL_P (new_insn))
	    break;
	  new_insn = NEXT_INSN (new_insn);
	}

      gcc_assert (new_insn != NULL_RTX);

      CALL_INSN_FUNCTION_USAGE (new_insn)
	= CALL_INSN_FUNCTION_USAGE (old_insn);
      SIBLING_CALL_P (new_insn) = SIBLING_CALL_P (old_insn);

      for (note = REG_NOTES (old_insn);
	   note;
	   note = XEXP (note, 1))
	switch (REG_NOTE_KIND (note))
	  {
	  case REG_NORETURN:
	  case REG_SETJMP:
	  case REG_TM:
	  case REG_CALL_NOCF_CHECK:
	    add_reg_note (new_insn, REG_NOTE_KIND (note),
			  XEXP (note, 0));
	    break;
	  default:
	    /* Discard all other reg notes.  */
	    break;
	  }

      /* Croak if there is another call in the sequence.  */
      while (++i <= match_len)
	{
	  j = peep2_buf_position (peep2_current + i);
	  old_insn = peep2_insn_data[j].insn;
	  gcc_assert (!CALL_P (old_insn));
	}
      break;
    }

  /* If we matched any instruction that had a REG_ARGS_SIZE, then
     move those notes over to the new sequence.  */
  as_note = NULL;
  for (i = match_len; i >= 0; --i)
    {
      int j = peep2_buf_position (peep2_current + i);
      old_insn = peep2_insn_data[j].insn;

      as_note = find_reg_note (old_insn, REG_ARGS_SIZE, NULL);
      if (as_note)
	break;
    }

  i = peep2_buf_position (peep2_current + match_len);
  eh_note = find_reg_note (peep2_insn_data[i].insn, REG_EH_REGION, NULL_RTX);

  /* Replace the old sequence with the new.  */
  rtx_insn *peepinsn = peep2_insn_data[i].insn;
  last = emit_insn_after_setloc (attempt,
				 peep2_insn_data[i].insn,
				 INSN_LOCATION (peepinsn));
  if (JUMP_P (peepinsn) && JUMP_P (last))
    CROSSING_JUMP_P (last) = CROSSING_JUMP_P (peepinsn);
  before_try = PREV_INSN (insn);
  delete_insn_chain (insn, peep2_insn_data[i].insn, false);

  /* Re-insert the EH_REGION notes.  */
  if (eh_note || (was_call && nonlocal_goto_handler_labels))
    {
      edge eh_edge;
      edge_iterator ei;

      FOR_EACH_EDGE (eh_edge, ei, bb->succs)
	if (eh_edge->flags & (EDGE_EH | EDGE_ABNORMAL_CALL))
	  break;

      if (eh_note)
	copy_reg_eh_region_note_backward (eh_note, last, before_try);

      if (eh_edge)
	for (x = last; x != before_try; x = PREV_INSN (x))
	  if (x != BB_END (bb)
	      && (can_throw_internal (x)
		  || can_nonlocal_goto (x)))
	    {
	      edge nfte, nehe;
	      int flags;

	      nfte = split_block (bb, x);
	      flags = (eh_edge->flags
		       & (EDGE_EH | EDGE_ABNORMAL));
	      if (CALL_P (x))
		flags |= EDGE_ABNORMAL_CALL;
	      nehe = make_edge (nfte->src, eh_edge->dest,
				flags);

	      nehe->probability = eh_edge->probability;
	      nfte->probability = nehe->probability.invert ();

	      peep2_do_cleanup_cfg |= purge_dead_edges (nfte->dest);
	      bb = nfte->src;
	      eh_edge = nehe;
	    }

      /* Converting possibly trapping insn to non-trapping is
	 possible.  Zap dummy outgoing edges.  */
      peep2_do_cleanup_cfg |= purge_dead_edges (bb);
    }

  /* Re-insert the ARGS_SIZE notes.  */
  if (as_note)
    fixup_args_size_notes (before_try, last, get_args_size (as_note));

  /* Scan the new insns for embedded side effects and add appropriate
     REG_INC notes.  */
  if (AUTO_INC_DEC)
    for (x = last; x != before_try; x = PREV_INSN (x))
      if (NONDEBUG_INSN_P (x))
	add_auto_inc_notes (x, PATTERN (x));

  /* If we generated a jump instruction, it won't have
     JUMP_LABEL set.  Recompute after we're done.  */
  for (x = last; x != before_try; x = PREV_INSN (x))
    if (JUMP_P (x))
      {
	peep2_do_rebuild_jump_labels = true;
	break;
      }

  return last;
}

/* After performing a replacement in basic block BB, fix up the life
   information in our buffer.  LAST is the last of the insns that we
   emitted as a replacement.  PREV is the insn before the start of
   the replacement.  MATCH_LEN + 1 is the number of instructions that were
   matched, and which now need to be replaced in the buffer.  */

static void
peep2_update_life (basic_block bb, int match_len, rtx_insn *last,
		   rtx_insn *prev)
{
  int i = peep2_buf_position (peep2_current + match_len + 1);
  rtx_insn *x;
  regset_head live;

  INIT_REG_SET (&live);
  COPY_REG_SET (&live, peep2_insn_data[i].live_before);

  gcc_assert (peep2_current_count >= match_len + 1);
  peep2_current_count -= match_len + 1;

  x = last;
  do
    {
      if (INSN_P (x))
	{
	  df_insn_rescan (x);
	  if (peep2_current_count < MAX_INSNS_PER_PEEP2)
	    {
	      peep2_current_count++;
	      if (--i < 0)
		i = MAX_INSNS_PER_PEEP2;
	      peep2_insn_data[i].insn = x;
	      df_simulate_one_insn_backwards (bb, x, &live);
	      COPY_REG_SET (peep2_insn_data[i].live_before, &live);
	    }
	}
      x = PREV_INSN (x);
    }
  while (x != prev);
  CLEAR_REG_SET (&live);

  peep2_current = i;
}

/* Add INSN, which is in BB, at the end of the peep2 insn buffer if possible.
   Return true if we added it, false otherwise.  The caller will try to match
   peepholes against the buffer if we return false; otherwise it will try to
   add more instructions to the buffer.  */

static bool
peep2_fill_buffer (basic_block bb, rtx_insn *insn, regset live)
{
  int pos;

  /* Once we have filled the maximum number of insns the buffer can hold,
     allow the caller to match the insns against peepholes.  We wait until
     the buffer is full in case the target has similar peepholes of different
     length; we always want to match the longest if possible.  */
  if (peep2_current_count == MAX_INSNS_PER_PEEP2)
    return false;

  /* If an insn has RTX_FRAME_RELATED_P set, do not allow it to be matched with
     any other pattern, lest it change the semantics of the frame info.  */
  if (RTX_FRAME_RELATED_P (insn))
    {
      /* Let the buffer drain first.  */
      if (peep2_current_count > 0)
	return false;
      /* Now the insn will be the only thing in the buffer.  */
    }

  pos = peep2_buf_position (peep2_current + peep2_current_count);
  peep2_insn_data[pos].insn = insn;
  COPY_REG_SET (peep2_insn_data[pos].live_before, live);
  peep2_current_count++;

  df_simulate_one_insn_forwards (bb, insn, live);
  return true;
}

/* Perform the peephole2 optimization pass.  */

static void
peephole2_optimize (void)
{
  rtx_insn *insn;
  bitmap live;
  int i;
  basic_block bb;

  peep2_do_cleanup_cfg = false;
  peep2_do_rebuild_jump_labels = false;

  df_set_flags (DF_LR_RUN_DCE);
  df_note_add_problem ();
  df_analyze ();

  /* Initialize the regsets we're going to use.  */
  for (i = 0; i < MAX_INSNS_PER_PEEP2 + 1; ++i)
    peep2_insn_data[i].live_before = BITMAP_ALLOC (&reg_obstack);
  search_ofs = 0;
  live = BITMAP_ALLOC (&reg_obstack);

  FOR_EACH_BB_REVERSE_FN (bb, cfun)
    {
      bool past_end = false;
      int pos;

      rtl_profile_for_bb (bb);

      /* Start up propagation.  */
      bitmap_copy (live, DF_LR_IN (bb));
      df_simulate_initialize_forwards (bb, live);
      peep2_reinit_state (live);

      insn = BB_HEAD (bb);
      for (;;)
	{
	  rtx_insn *attempt, *head;
	  int match_len;

	  if (!past_end && !NONDEBUG_INSN_P (insn))
	    {
	    next_insn:
	      insn = NEXT_INSN (insn);
	      if (insn == NEXT_INSN (BB_END (bb)))
		past_end = true;
	      continue;
	    }
	  if (!past_end && peep2_fill_buffer (bb, insn, live))
	    goto next_insn;

	  /* If we did not fill an empty buffer, it signals the end of the
	     block.  */
	  if (peep2_current_count == 0)
	    break;

	  /* The buffer filled to the current maximum, so try to match.  */

	  pos = peep2_buf_position (peep2_current + peep2_current_count);
	  peep2_insn_data[pos].insn = PEEP2_EOB;
	  COPY_REG_SET (peep2_insn_data[pos].live_before, live);

	  /* Match the peephole.  */
	  head = peep2_insn_data[peep2_current].insn;
	  attempt = peephole2_insns (PATTERN (head), head, &match_len);
	  if (attempt != NULL)
	    {
	      rtx_insn *last = peep2_attempt (bb, head, match_len, attempt);
	      if (last)
		{
		  peep2_update_life (bb, match_len, last, PREV_INSN (attempt));
		  continue;
		}
	    }

	  /* No match: advance the buffer by one insn.  */
	  peep2_current = peep2_buf_position (peep2_current + 1);
	  peep2_current_count--;
	}
    }

  default_rtl_profile ();
  for (i = 0; i < MAX_INSNS_PER_PEEP2 + 1; ++i)
    BITMAP_FREE (peep2_insn_data[i].live_before);
  BITMAP_FREE (live);
  if (peep2_do_rebuild_jump_labels)
    rebuild_jump_labels (get_insns ());
  if (peep2_do_cleanup_cfg)
    cleanup_cfg (CLEANUP_CFG_CHANGED);
}

/* Common predicates for use with define_bypass.  */

/* Helper function for store_data_bypass_p, handle just a single SET
   IN_SET.  */

static bool
store_data_bypass_p_1 (rtx_insn *out_insn, rtx in_set)
{
  if (!MEM_P (SET_DEST (in_set)))
    return false;

  rtx out_set = single_set (out_insn);
  if (out_set)
    return !reg_mentioned_p (SET_DEST (out_set), SET_DEST (in_set));

  rtx out_pat = PATTERN (out_insn);
  if (GET_CODE (out_pat) != PARALLEL)
    return false;

  for (int i = 0; i < XVECLEN (out_pat, 0); i++)
    {
      rtx out_exp = XVECEXP (out_pat, 0, i);

      if (GET_CODE (out_exp) == CLOBBER || GET_CODE (out_exp) == USE)
	continue;

      gcc_assert (GET_CODE (out_exp) == SET);

      if (reg_mentioned_p (SET_DEST (out_exp), SET_DEST (in_set)))
	return false;
    }

  return true;
}

/* True if the dependency between OUT_INSN and IN_INSN is on the store
   data not the address operand(s) of the store.  IN_INSN and OUT_INSN
   must be either a single_set or a PARALLEL with SETs inside.  */

bool
store_data_bypass_p (rtx_insn *out_insn, rtx_insn *in_insn)
{
  rtx in_set = single_set (in_insn);
  if (in_set)
    return store_data_bypass_p_1 (out_insn, in_set);

  rtx in_pat = PATTERN (in_insn);
  if (GET_CODE (in_pat) != PARALLEL)
    return false;

  for (int i = 0; i < XVECLEN (in_pat, 0); i++)
    {
      rtx in_exp = XVECEXP (in_pat, 0, i);

      if (GET_CODE (in_exp) == CLOBBER || GET_CODE (in_exp) == USE)
	continue;

      gcc_assert (GET_CODE (in_exp) == SET);

      if (!store_data_bypass_p_1 (out_insn, in_exp))
	return false;
    }

  return true;
}

/* True if the dependency between OUT_INSN and IN_INSN is in the IF_THEN_ELSE
   condition, and not the THEN or ELSE branch.  OUT_INSN may be either a single
   or multiple set; IN_INSN should be single_set for truth, but for convenience
   of insn categorization may be any JUMP or CALL insn.  */

bool
if_test_bypass_p (rtx_insn *out_insn, rtx_insn *in_insn)
{
  rtx out_set, in_set;

  in_set = single_set (in_insn);
  if (! in_set)
    {
      gcc_assert (JUMP_P (in_insn) || CALL_P (in_insn));
      return false;
    }

  if (GET_CODE (SET_SRC (in_set)) != IF_THEN_ELSE)
    return false;
  in_set = SET_SRC (in_set);

  out_set = single_set (out_insn);
  if (out_set)
    {
      if (reg_mentioned_p (SET_DEST (out_set), XEXP (in_set, 1))
	  || reg_mentioned_p (SET_DEST (out_set), XEXP (in_set, 2)))
	return false;
    }
  else
    {
      rtx out_pat;
      int i;

      out_pat = PATTERN (out_insn);
      gcc_assert (GET_CODE (out_pat) == PARALLEL);

      for (i = 0; i < XVECLEN (out_pat, 0); i++)
	{
	  rtx exp = XVECEXP (out_pat, 0, i);

	  if (GET_CODE (exp) == CLOBBER)
	    continue;

	  gcc_assert (GET_CODE (exp) == SET);

	  if (reg_mentioned_p (SET_DEST (out_set), XEXP (in_set, 1))
	      || reg_mentioned_p (SET_DEST (out_set), XEXP (in_set, 2)))
	    return false;
	}
    }

  return true;
}

static unsigned int
rest_of_handle_peephole2 (void)
{
  if (HAVE_peephole2)
    peephole2_optimize ();

  return 0;
}

namespace {

const pass_data pass_data_peephole2 =
{
  RTL_PASS, /* type */
  "peephole2", /* name */
  OPTGROUP_NONE, /* optinfo_flags */
  TV_PEEPHOLE2, /* tv_id */
  0, /* properties_required */
  0, /* properties_provided */
  0, /* properties_destroyed */
  0, /* todo_flags_start */
  TODO_df_finish, /* todo_flags_finish */
};

class pass_peephole2 : public rtl_opt_pass
{
public:
  pass_peephole2 (gcc::context *ctxt)
    : rtl_opt_pass (pass_data_peephole2, ctxt)
  {}

  /* opt_pass methods: */
  /* The epiphany backend creates a second instance of this pass, so we need
     a clone method.  */
  opt_pass * clone () final override { return new pass_peephole2 (m_ctxt); }
  bool gate (function *) final override
  {
    return (optimize > 0 && flag_peephole2);
  }
  unsigned int execute (function *) final override
    {
      return rest_of_handle_peephole2 ();
    }

}; // class pass_peephole2

} // anon namespace

rtl_opt_pass *
make_pass_peephole2 (gcc::context *ctxt)
{
  return new pass_peephole2 (ctxt);
}

namespace {

const pass_data pass_data_split_all_insns =
{
  RTL_PASS, /* type */
  "split1", /* name */
  OPTGROUP_NONE, /* optinfo_flags */
  TV_NONE, /* tv_id */
  0, /* properties_required */
  PROP_rtl_split_insns, /* properties_provided */
  0, /* properties_destroyed */
  0, /* todo_flags_start */
  0, /* todo_flags_finish */
};

class pass_split_all_insns : public rtl_opt_pass
{
public:
  pass_split_all_insns (gcc::context *ctxt)
    : rtl_opt_pass (pass_data_split_all_insns, ctxt)
  {}

  /* opt_pass methods: */
  /* The epiphany backend creates a second instance of this pass, so
     we need a clone method.  */
  opt_pass * clone () final override
  {
    return new pass_split_all_insns (m_ctxt);
  }
  unsigned int execute (function *) final override
    {
      split_all_insns ();
      return 0;
    }

}; // class pass_split_all_insns

} // anon namespace

rtl_opt_pass *
make_pass_split_all_insns (gcc::context *ctxt)
{
  return new pass_split_all_insns (ctxt);
}

namespace {

const pass_data pass_data_split_after_reload =
{
  RTL_PASS, /* type */
  "split2", /* name */
  OPTGROUP_NONE, /* optinfo_flags */
  TV_NONE, /* tv_id */
  0, /* properties_required */
  0, /* properties_provided */
  0, /* properties_destroyed */
  0, /* todo_flags_start */
  0, /* todo_flags_finish */
};

class pass_split_after_reload : public rtl_opt_pass
{
public:
  pass_split_after_reload (gcc::context *ctxt)
    : rtl_opt_pass (pass_data_split_after_reload, ctxt)
  {}

  /* opt_pass methods: */
  bool gate (function *) final override
    {
      /* If optimizing, then go ahead and split insns now.  */
      return optimize > 0;
    }

  unsigned int execute (function *) final override
    {
      split_all_insns ();
      return 0;
    }

}; // class pass_split_after_reload

} // anon namespace

rtl_opt_pass *
make_pass_split_after_reload (gcc::context *ctxt)
{
  return new pass_split_after_reload (ctxt);
}

static bool
enable_split_before_sched2 (void)
{
#ifdef INSN_SCHEDULING
  return optimize > 0 && flag_schedule_insns_after_reload;
#else
  return false;
#endif
}

namespace {

const pass_data pass_data_split_before_sched2 =
{
  RTL_PASS, /* type */
  "split3", /* name */
  OPTGROUP_NONE, /* optinfo_flags */
  TV_NONE, /* tv_id */
  0, /* properties_required */
  0, /* properties_provided */
  0, /* properties_destroyed */
  0, /* todo_flags_start */
  0, /* todo_flags_finish */
};

class pass_split_before_sched2 : public rtl_opt_pass
{
public:
  pass_split_before_sched2 (gcc::context *ctxt)
    : rtl_opt_pass (pass_data_split_before_sched2, ctxt)
  {}

  /* opt_pass methods: */
  bool gate (function *) final override
    {
      return enable_split_before_sched2 ();
    }

  unsigned int execute (function *) final override
    {
      split_all_insns ();
      return 0;
    }

}; // class pass_split_before_sched2

} // anon namespace

rtl_opt_pass *
make_pass_split_before_sched2 (gcc::context *ctxt)
{
  return new pass_split_before_sched2 (ctxt);
}

namespace {

const pass_data pass_data_split_before_regstack =
{
  RTL_PASS, /* type */
  "split4", /* name */
  OPTGROUP_NONE, /* optinfo_flags */
  TV_NONE, /* tv_id */
  0, /* properties_required */
  0, /* properties_provided */
  0, /* properties_destroyed */
  0, /* todo_flags_start */
  0, /* todo_flags_finish */
};

class pass_split_before_regstack : public rtl_opt_pass
{
public:
  pass_split_before_regstack (gcc::context *ctxt)
    : rtl_opt_pass (pass_data_split_before_regstack, ctxt)
  {}

  /* opt_pass methods: */
  bool gate (function *) final override;
  unsigned int execute (function *) final override
    {
      split_all_insns ();
      return 0;
    }

}; // class pass_split_before_regstack

bool
pass_split_before_regstack::gate (function *)
{
#if HAVE_ATTR_length && defined (STACK_REGS)
  /* If flow2 creates new instructions which need splitting
     and scheduling after reload is not done, they might not be
     split until final which doesn't allow splitting
     if HAVE_ATTR_length.  Selective scheduling can result in
     further instructions that need splitting.  */
#ifdef INSN_SCHEDULING
  return !enable_split_before_sched2 () || flag_selective_scheduling2;
#else
  return !enable_split_before_sched2 ();
#endif
#else
  return false;
#endif
}

} // anon namespace

rtl_opt_pass *
make_pass_split_before_regstack (gcc::context *ctxt)
{
  return new pass_split_before_regstack (ctxt);
}

namespace {

const pass_data pass_data_split_for_shorten_branches =
{
  RTL_PASS, /* type */
  "split5", /* name */
  OPTGROUP_NONE, /* optinfo_flags */
  TV_NONE, /* tv_id */
  0, /* properties_required */
  0, /* properties_provided */
  0, /* properties_destroyed */
  0, /* todo_flags_start */
  0, /* todo_flags_finish */
};

class pass_split_for_shorten_branches : public rtl_opt_pass
{
public:
  pass_split_for_shorten_branches (gcc::context *ctxt)
    : rtl_opt_pass (pass_data_split_for_shorten_branches, ctxt)
  {}

  /* opt_pass methods: */
  bool gate (function *) final override
    {
      /* The placement of the splitting that we do for shorten_branches
	 depends on whether regstack is used by the target or not.  */
#if HAVE_ATTR_length && !defined (STACK_REGS)
      return true;
#else
      return false;
#endif
    }

  unsigned int execute (function *) final override
    {
      split_all_insns_noflow ();
      return 0;
    }

}; // class pass_split_for_shorten_branches

} // anon namespace

rtl_opt_pass *
make_pass_split_for_shorten_branches (gcc::context *ctxt)
{
  return new pass_split_for_shorten_branches (ctxt);
}

/* (Re)initialize the target information after a change in target.  */

void
recog_init ()
{
  /* The information is zero-initialized, so we don't need to do anything
     first time round.  */
  if (!this_target_recog->x_initialized)
    {
      this_target_recog->x_initialized = true;
      return;
    }
  memset (this_target_recog->x_bool_attr_masks, 0,
	  sizeof (this_target_recog->x_bool_attr_masks));
  for (unsigned int i = 0; i < NUM_INSN_CODES; ++i)
    if (this_target_recog->x_op_alt[i])
      {
	free (this_target_recog->x_op_alt[i]);
	this_target_recog->x_op_alt[i] = 0;
      }
}
