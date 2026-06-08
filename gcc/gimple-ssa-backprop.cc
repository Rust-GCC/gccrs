/* Back-propagation of usage information to definitions.
   Copyright (C) 2015-2026 Free Software Foundation, Inc.

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

/* This pass propagates information that is common to all uses of an SSA
   name back up through the sequence of statements that generate it,
   simplifying the statements where possible.  Sometimes this can expose
   fully or partially dead code, but the main focus is simplifying
   computations.

   At the moment the pass only handles one piece of information: whether the
   sign of a value matters, and therefore whether sign-changing operations
   can be skipped.  The pass could be extended to more interesting
   information in future, such as which bits of an integer are significant.

   For example, take the function:

     double
     f (double *a, int n, double start)
     {
       double x = fabs (start);
       for (int i = 0; i < n; ++i)
	 x *= a[i];
       return __builtin_cos (x);
     }

   cos(x) == cos(-x), so the sign of the final x doesn't matter.
   That x is the result of a series of multiplications, and if
   the sign of the result of a multiplication doesn't matter,
   the signs of the inputs don't matter either.

   The pass would replace the incoming value of x (i.e. fabs(start))
   with start.  Since there are no other uses of the fabs result,
   the call would get deleted as dead.

   The algorithm is:

   (1) Do a post-order traversal of the blocks in the function, walking
       each block backwards.  For each potentially-simplifiable statement
       that defines an SSA name X, examine all uses of X to see what
       information is actually significant.  Record this in VAR_TABLE[X].
       Optimistically ignore for now any back-edge references to
       unprocessed phis.

       (An alternative would be to record each use when we visit its
       statement and take the intersection as we go along.  However,
       this would lead to more SSA names being entered into VAR_TABLE
       unnecessarily, only to be taken out again later.  At the moment
       very few SSA names end up with useful information.)

   (2) Iteratively reduce the optimistic result of (1) until we reach
       a maximal fixed point.  First push all SSA names that used an
       optimistic assumption about a backedge phi onto a worklist.
       While the worklist is nonempty, pick off an SSA name X and recompute
       the usage information in VAR_TABLE[X].  If the value changes, push all
       SSA names used in the definition of X onto the worklist.

   (3) Iterate over each SSA name X with info in VAR_TABLE, in the
       opposite order to (1), i.e. a forward reverse-post-order walk.
       Try to use the information in VAR_TABLE[X] to replace the definition
       of X with a simpler calculation that may yield a different result.
       When deciding to do such a replacement, record that each entry in
       VAR_TABLE that directly or indirectly depends on X will then also
       need to be replaced with a different calculation.  (The information
       in VAR_TABLE guarantees that other uses of X are unaffected.)

   (4) Iterate over each SSA name X with info in VAR_TABLE, in the same
       order as (1).  Carry out any scheduled replacement of X with a
       different value, ensuring that debug uses either keep their current
       value or are reset.  (This traversal order ensures that, if a chain
       of values is being replaced, a debug bind is created for the last
       value first, exposing a new debug use of the preceding value in
       the chain.)

   (5) Iterate over each SSA name X with info in VAR_TABLE, in the same order
       as (1).  Delete any statements that are now dead.  (This traversal order
       ensures that if a sequence of statements is dead, we delete the last
       statement first.  It is not safe to do this on-the-fly during (4)
       because multiple SSA names might be replaced with the same value.)

   Note that this pass does not deal with direct redundancies,
   such as cos(-x)->cos(x).  match.pd handles those cases instead.  */

#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "backend.h"
#include "tree.h"
#include "gimple.h"
#include "gimple-iterator.h"
#include "ssa.h"
#include "fold-const.h"
#include "tree-pass.h"
#include "cfganal.h"
#include "gimple-pretty-print.h"
#include "tree-cfg.h"
#include "tree-ssa.h"
#include "tree-ssa-propagate.h"
#include "gimple-fold.h"
#include "alloc-pool.h"
#include "tree-hash-traits.h"
#include "case-cfn-macros.h"

namespace {

/* Information about a group of uses of an SSA name.  */
class usage_info
{
public:
  usage_info () : flag_word (0) {}
  usage_info &operator &= (const usage_info &);
  usage_info operator & (const usage_info &) const;
  bool operator == (const usage_info &) const;
  bool operator != (const usage_info &) const;
  bool is_useful () const;

  static usage_info intersection_identity ();

  union
  {
    struct
    {
      /* True if the uses treat x and -x in the same way.  */
      unsigned int ignore_sign : 1;
    } flags;
    /* All the flag bits as a single int.  */
    unsigned int flag_word;
  };
};

/* Return an X such that X & Y == Y for all Y.  This is the most
   optimistic assumption possible.  */

usage_info
usage_info::intersection_identity ()
{
  usage_info ret;
  ret.flag_word = -1;
  return ret;
}

/* Intersect *THIS with OTHER, so that *THIS describes all uses covered
   by the original *THIS and OTHER.  */

usage_info &
usage_info::operator &= (const usage_info &other)
{
  flag_word &= other.flag_word;
  return *this;
}

/* Return the intersection of *THIS and OTHER, i.e. a structure that
   describes all uses covered by *THIS and OTHER.  */

usage_info
usage_info::operator & (const usage_info &other) const
{
  usage_info info (*this);
  info &= other;
  return info;
}

bool
usage_info::operator == (const usage_info &other) const
{
  return flag_word == other.flag_word;
}

bool
usage_info::operator != (const usage_info &other) const
{
  return !operator == (other);
}

/* Return true if *THIS is not simply the default, safe assumption.  */

bool
usage_info::is_useful () const
{
  return flag_word != 0;
}

/* Start a dump line about SSA name VAR.  */

static void
dump_usage_prefix (FILE *file, tree var)
{
  fprintf (file, "  ");
  print_generic_expr (file, var);
  fprintf (file, ": ");
}

/* Print INFO to FILE.  */

static void
dump_usage_info (FILE *file, tree var, usage_info *info)
{
  if (info->flags.ignore_sign)
    {
      dump_usage_prefix (file, var);
      fprintf (file, "sign bit not important\n");
    }
}

/* Information about an SSA name that we might want to optimize.  */
struct var_info
{
  var_info (tree var, unsigned int index, const usage_info &info)
    : var (var), new_value (var), seq (nullptr), index (index), info (info)
  {}

  /* The SSA name itself.  */
  tree var;

  /* The value that VAR will be replaced with, or VAR itself if no
     replacement is scheduled.  */
  tree new_value;

  /* If NEW_VALUE != VAR, the definition of VAR will be replaced by this
     sequence, with an empty sequence meaning that the definition of VAR
     should simply be deleted.  */
  gimple_seq seq;

  /* The index of this entry in the M_VARS array.  */
  unsigned int index;

  /* Information about all uses of the SSA name.  */
  usage_info info;
};

/* Hash var_info based on the SSA name.  */
struct var_info_hasher : nofree_ptr_hash<var_info>
{
  using compare_type = const_tree;

  static inline hashval_t hash (const var_info *);
  static inline bool equal (const var_info *, const_tree);
};

hashval_t
var_info_hasher::hash (const var_info *v)
{
  return SSA_NAME_VERSION (v->var);
}

bool
var_info_hasher::equal (const var_info *v, const_tree var)
{
  return v->var == var;
}

/* Represents one execution of the pass.  */
class backprop
{
public:
  backprop (function *);
  ~backprop ();

  void execute ();

private:
  var_info *lookup_operand (tree);

  void push_to_worklist (var_info *);
  var_info *pop_from_worklist ();

  void process_builtin_call_use (gcall *, tree, usage_info *);
  void process_assign_use (gassign *, tree, usage_info *);
  void process_phi_use (gphi *, usage_info *);
  void process_use (gimple *, tree, usage_info *);
  bool intersect_uses (tree, usage_info *);
  void reprocess_inputs (gimple *);
  void process_var (tree, var_info *);
  void process_block (basic_block);

  void set_new_value (var_info *, tree);
  tree subst_operand (tree);
  gimple *copy_and_subst (var_info *);
  void finish_stmt (var_info *, gimple *);
  void optimize_builtin_call (gcall *, var_info *);
  void replace_assign_rhs (var_info *, tree, tree, tree);
  void optimize_assign (gassign *, var_info *);
  void optimize_phi (gphi *, var_info *);

  void propagate_change (var_info *);
  void commit_replacement (var_info *);

  /* The function we're optimizing.  */
  function *m_fn;

  /* Pool for allocating var_info structures.  */
  object_allocator<var_info> m_var_pool;

  /* All extant var_infos, hashed by SSA name.  All the usage_infos satisfy
     is_useful.

     We use a hash_table because the table is expected to be sparse
     (i.e. most SSA names won't have useful information attached to them).
     We could move to a directly-indexed array if that situation changes.  */
  hash_table<var_info_hasher> m_var_table;

  /* A post-ordered list of the contents of M_VAR_TABLE.  Entries might
     be null if we initially created a var_info and then withdrew it.  */
  auto_vec<var_info *, 128> m_vars;

  /* A bitmap of blocks that we have finished processing in the initial
     post-order walk.  */
  auto_sbitmap m_visited_blocks;

  /* A bitmap of phis that we have finished processing in the initial
     post-order walk, excluding those from blocks mentioned in
     M_VISITED_BLOCKS.  */
  auto_bitmap m_visited_phis;

  /* Two bitmaps that can be used for worklists.  The elements represent
     indices into M_VARS.  */
  auto_bitmap m_worklist1, m_worklist2;

  /* Used to perform a double-worklist update.  M_THIS_WORKLIST contains
     the elements of M_VARS that should be processed in the current pass,
     whereas M_NEXT_WORKLIST contains the elements of M_VARS that should
     be processed in the next pass.

     In a post-order traversal, any member of M_VARS beyond index
     M_WORKLIST_THRESHOLD can be added to M_THIS_WORKLIST whereas others
     should be added to M_NEXT_WORKLIST.  */
  bitmap m_this_worklist, m_next_worklist;
  unsigned int m_worklist_threshold;
};

backprop::backprop (function *fn)
  : m_fn (fn),
    m_var_pool ("var_info"),
    m_var_table (64),
    m_visited_blocks (last_basic_block_for_fn (m_fn)),
    m_this_worklist (m_worklist1),
    m_next_worklist (m_worklist2),
    m_worklist_threshold (UINT_MAX)
{
  bitmap_clear (m_visited_blocks);
  bitmap_tree_view (m_worklist1);
  bitmap_tree_view (m_worklist2);
}

backprop::~backprop ()
{
  m_var_pool.release ();
}

/* Return the var_info for general operand OP, or null if none.  */

var_info *
backprop::lookup_operand (tree op)
{
  if (op && TREE_CODE (op) == SSA_NAME)
    return m_var_table.find_with_hash (op, SSA_NAME_VERSION (op));
  return NULL;
}

/* Add V to the worklist, if it isn't on the worklist already.  */

void
backprop::push_to_worklist (var_info *v)
{
  bitmap worklist = (v->index > m_worklist_threshold
		     ? m_this_worklist
		     : m_next_worklist);
  if (bitmap_set_bit (worklist, v->index)
      && (dump_file && (dump_flags & TDF_DETAILS)))
    {
      fprintf (dump_file, "[WORKLIST] Pushing ");
      print_generic_expr (dump_file, v->var);
      fprintf (dump_file, "\n");
    }
}

/* Remove and return the next var_info from the worklist.  The worklist
   is known to be nonempty.  */

var_info *
backprop::pop_from_worklist ()
{
  m_worklist_threshold = bitmap_clear_first_set_bit (m_this_worklist);
  var_info *v = m_vars[m_worklist_threshold];
  if (dump_file && (dump_flags & TDF_DETAILS))
    {
      fprintf (dump_file, "[WORKLIST] Popping ");
      print_generic_expr (dump_file, v->var);
      fprintf (dump_file, "\n");
    }
  return v;
}

/* Make INFO describe all uses of RHS in CALL, which is a call to a
   built-in function.  */

void
backprop::process_builtin_call_use (gcall *call, tree rhs, usage_info *info)
{
  combined_fn fn = gimple_call_combined_fn (call);
  tree lhs = gimple_call_lhs (call);
  switch (fn)
    {
    case CFN_LAST:
      break;

    CASE_CFN_COS:
    CASE_CFN_COS_FN:
    CASE_CFN_COSH:
    CASE_CFN_COSH_FN:
    CASE_CFN_CCOS:
    CASE_CFN_CCOS_FN:
    CASE_CFN_CCOSH:
    CASE_CFN_CCOSH_FN:
    CASE_CFN_HYPOT:
    CASE_CFN_HYPOT_FN:
      /* The signs of all inputs are ignored.  */
      info->flags.ignore_sign = true;
      break;

    CASE_CFN_COPYSIGN:
    CASE_CFN_COPYSIGN_FN:
      /* The sign of the first input is ignored.  */
      if (rhs != gimple_call_arg (call, 1))
	info->flags.ignore_sign = true;
      break;

    CASE_CFN_POW:
    CASE_CFN_POW_FN:
      {
	/* The sign of the first input is ignored as long as the second
	   input is an even real.  */
	tree power = gimple_call_arg (call, 1);
	HOST_WIDE_INT n;
	if (TREE_CODE (power) == REAL_CST
	    && real_isinteger (&TREE_REAL_CST (power), &n)
	    && (n & 1) == 0)
	  info->flags.ignore_sign = true;
	break;
      }

    CASE_CFN_FMA:
    CASE_CFN_FMA_FN:
    case CFN_FMS:
    case CFN_FNMA:
    case CFN_FNMS:
      /* In X * X + Y, where Y is distinct from X, the sign of X doesn't
	 matter.  */
      if (gimple_call_arg (call, 0) == rhs
	  && gimple_call_arg (call, 1) == rhs
	  && gimple_call_arg (call, 2) != rhs)
	info->flags.ignore_sign = true;
      break;

    default:
      if (negate_mathfn_p (fn))
	/* The sign of the (single) input doesn't matter provided
	   that the sign of the output doesn't matter.  */
	if (var_info *lhsv = lookup_operand (lhs))
	  info->flags.ignore_sign = lhsv->info.flags.ignore_sign;
      break;
    }
}

/* Make INFO describe all uses of RHS in ASSIGN.  */

void
backprop::process_assign_use (gassign *assign, tree rhs, usage_info *info)
{
  tree lhs = gimple_assign_lhs (assign);
  switch (gimple_assign_rhs_code (assign))
    {
    case ABS_EXPR:
    case ABSU_EXPR:
      /* The sign of the input doesn't matter.  */
      info->flags.ignore_sign = true;
      break;

    case COND_EXPR:
      /* For A = B ? C : D, propagate information about all uses of A
	 to C and D.  */
      if (rhs != gimple_assign_rhs1 (assign))
	if (var_info *lhsv = lookup_operand (lhs))
	  *info = lhsv->info;
      break;

    case MULT_EXPR:
      /* In X * X, the sign of X doesn't matter.  */
      if (gimple_assign_rhs1 (assign) == rhs
	  && gimple_assign_rhs2 (assign) == rhs)
	info->flags.ignore_sign = true;
      /* Fall through.  */

    case NEGATE_EXPR:
    case RDIV_EXPR:
      /* If the sign of the result doesn't matter, the sign of the inputs
	 doesn't matter either.  */
      if (FLOAT_TYPE_P (TREE_TYPE (rhs)))
	if (var_info *lhsv = lookup_operand (lhs))
	  info->flags.ignore_sign = lhsv->info.flags.ignore_sign;
      break;

    default:
      break;
    }
}

/* Make INFO describe the uses of PHI's result.  */

void
backprop::process_phi_use (gphi *phi, usage_info *info)
{
  tree result = gimple_phi_result (phi);
  if (var_info *resultv = lookup_operand (result))
    *info = resultv->info;
}

/* Make INFO describe all uses of RHS in STMT.  */

void
backprop::process_use (gimple *stmt, tree rhs, usage_info *info)
{
  if (dump_file && (dump_flags & TDF_DETAILS))
    {
      fprintf (dump_file, "[USE] ");
      print_generic_expr (dump_file, rhs);
      fprintf (dump_file, " in ");
      print_gimple_stmt (dump_file, stmt, 0, TDF_SLIM);
    }

  if (gcall *call = dyn_cast <gcall *> (stmt))
    process_builtin_call_use (call, rhs, info);
  else if (gassign *assign = dyn_cast <gassign *> (stmt))
    process_assign_use (assign, rhs, info);
  else if (gphi *phi = dyn_cast <gphi *> (stmt))
    process_phi_use (phi, info);

  if (dump_file && (dump_flags & TDF_DETAILS))
    dump_usage_info (dump_file, rhs, info);
}

/* Make INFO describe all uses of VAR, returning true if the result
   is useful.  If the uses include phis that haven't been processed yet,
   make the most optimistic assumption possible, so that we aim for
   a maximum rather than a minimum fixed point.  */

bool
backprop::intersect_uses (tree var, usage_info *info)
{
  imm_use_iterator iter;
  use_operand_p use_p;
  *info = usage_info::intersection_identity ();
  FOR_EACH_IMM_USE_FAST (use_p, iter, var)
    {
      gimple *stmt = USE_STMT (use_p);
      if (is_gimple_debug (stmt))
	continue;
      gphi *phi = dyn_cast <gphi *> (stmt);
      if (phi
	  && !bitmap_bit_p (m_visited_blocks, gimple_bb (phi)->index)
	  && !bitmap_bit_p (m_visited_phis,
			    SSA_NAME_VERSION (gimple_phi_result (phi))))
	{
	  /* Skip unprocessed phis.  */
	  if (dump_file && (dump_flags & TDF_DETAILS))
	    {
	      fprintf (dump_file, "[BACKEDGE] ");
	      print_generic_expr (dump_file, var);
	      fprintf (dump_file, " in ");
	      print_gimple_stmt (dump_file, phi, 0, TDF_SLIM);
	    }
	}
      else
	{
	  usage_info subinfo;
	  process_use (stmt, var, &subinfo);
	  *info &= subinfo;
	  if (!info->is_useful ())
	    return false;
	}
    }
  return true;
}

/* Queue for reconsideration any input of STMT that has information
   associated with it.  This is used if that information might be
   too optimistic.  */

void
backprop::reprocess_inputs (gimple *stmt)
{
  use_operand_p use_p;
  ssa_op_iter oi;
  FOR_EACH_PHI_OR_STMT_USE (use_p, stmt, oi, SSA_OP_USE)
    if (var_info *v = lookup_operand (get_use_from_ptr (use_p)))
      push_to_worklist (v);
}

/* Say that we're recording INFO for SSA name VAR, or that we're deleting
   existing information if INFO is null.  INTRO describes the change.  */

static void
dump_var_info (tree var, usage_info *info, const char *intro)
{
  fprintf (dump_file, "[DEF] %s for ", intro);
  print_gimple_stmt (dump_file, SSA_NAME_DEF_STMT (var), 0, TDF_SLIM);
  if (info)
    dump_usage_info (dump_file, var, info);
}

/* Process all uses of VAR and record or update the result in
   M_VAR_TABLE and M_VARS.  V is the current entry for VAR, or null
   if we are looking at it for the first time.  */

void
backprop::process_var (tree var, var_info *v)
{
  if (has_zero_uses (var))
    return;

  usage_info info;

  /* Propagating along abnormal edges is delicate, punt for now.  */
  if (!SSA_NAME_OCCURS_IN_ABNORMAL_PHI (var))
    intersect_uses (var, &info);

  gimple *stmt = SSA_NAME_DEF_STMT (var);
  auto hash = SSA_NAME_VERSION (var);
  if (info.is_useful ())
    {
      if (!v)
	{
	  /* Recording information about VAR for the first time.  */
	  v = m_var_pool.allocate (var, m_vars.length (), info);
	  m_vars.safe_push (v);
	  *m_var_table.find_slot_with_hash (var, hash, INSERT) = v;

	  if (dump_file && (dump_flags & TDF_DETAILS))
	    dump_var_info (var, &v->info, "Recording new information");

	  /* If STMT is a phi, reprocess any backedge uses.  This is a
	     no-op for other uses, which won't have any information
	     associated with them.  */
	  if (is_a <gphi *> (stmt))
	    reprocess_inputs (stmt);
	}
      else if (info != v->info)
	{
	  /* Recording information that is less optimistic than before.  */
	  gcc_checking_assert ((info & v->info) == info);
	  v->info = info;
	  if (dump_file && (dump_flags & TDF_DETAILS))
	    dump_var_info (var, &v->info, "Updating information");
	  reprocess_inputs (stmt);
	}
    }
  else
    {
      if (v)
	{
	  /* Removing previously-recorded information.  */
	  m_var_table.remove_elt_with_hash (var, hash);
	  m_vars[v->index] = nullptr;
	  m_var_pool.remove (v);

	  if (dump_file && (dump_flags & TDF_DETAILS))
	    dump_var_info (var, NULL, "Deleting information");
	  reprocess_inputs (stmt);
	}
      else
	{
	  /* If STMT is a phi, remove any information recorded for
	     its arguments.  */
	  if (is_a <gphi *> (stmt))
	    reprocess_inputs (stmt);
	}
    }
}

/* Process all statements and phis in BB, during the first post-order walk.  */

void
backprop::process_block (basic_block bb)
{
  for (gimple_stmt_iterator gsi = gsi_last_bb (bb); !gsi_end_p (gsi);
       gsi_prev (&gsi))
    {
      tree lhs = gimple_get_lhs (gsi_stmt (gsi));
      if (lhs && TREE_CODE (lhs) == SSA_NAME)
	process_var (lhs, nullptr);
    }
  for (gphi_iterator gpi = gsi_start_phis (bb); !gsi_end_p (gpi);
       gsi_next (&gpi))
    {
      tree result = gimple_phi_result (gpi.phi ());
      process_var (result, nullptr);
      bitmap_set_bit (m_visited_phis, SSA_NAME_VERSION (result));
    }
  bitmap_clear (m_visited_phis);
}

/* Delete the statement at GSI, which is now dead.  Note the deletion in the
   dump file unless QUIET is true.  */

static void
remove_dead_stmt (gimple_stmt_iterator *gsi, bool quiet = false)
{
  gimple *stmt = gsi_stmt (*gsi);
  if (!quiet && dump_file && (dump_flags & TDF_DETAILS))
    {
      fprintf (dump_file, "Deleting ");
      print_gimple_stmt (dump_file, stmt, 0, TDF_SLIM);
    }
  if (gimple_code (stmt) == GIMPLE_PHI)
    remove_phi_node (gsi, true);
  else
    {
      unlink_stmt_vdef (stmt);
      gsi_remove (gsi, true);
      release_defs (stmt);
    }
}

/* If RHS is an SSA name whose definition just changes the sign of a value,
   return that other value, otherwise return null.  */

static tree
strip_sign_op_1 (tree rhs)
{
  if (TREE_CODE (rhs) != SSA_NAME)
    return NULL_TREE;

  gimple *def_stmt = SSA_NAME_DEF_STMT (rhs);
  if (gassign *assign = dyn_cast <gassign *> (def_stmt))
    switch (gimple_assign_rhs_code (assign))
      {
      case ABS_EXPR:
      case NEGATE_EXPR:
	return gimple_assign_rhs1 (assign);

      default:
	break;
      }
  else if (gcall *call = dyn_cast <gcall *> (def_stmt))
    switch (gimple_call_combined_fn (call))
      {
      CASE_CFN_COPYSIGN:
      CASE_CFN_COPYSIGN_FN:
	return gimple_call_arg (call, 0);

      default:
	break;
      }

  return NULL_TREE;
}

/* If RHS is an SSA name whose definition just changes the sign of a value,
   strip all such operations and return the ultimate input to them.
   Return null otherwise.

   Although this could in principle lead to quadratic searching,
   in practice a long sequence of sign manipulations should already
   have been folded down.  E.g. --x -> x, abs(-x) -> abs(x).  We search
   for more than one operation in order to catch cases like -abs(x).  */

static tree
strip_sign_op (tree rhs)
{
  tree new_rhs = strip_sign_op_1 (rhs);
  if (!new_rhs)
    return NULL_TREE;
  while (tree next = strip_sign_op_1 (new_rhs))
    new_rhs = next;
  return new_rhs;
}

/* Note that V's SSA name will be replaced with NEW_VALUE.  */

void
backprop::set_new_value (var_info *v, tree new_value)
{
  if (dump_file && (dump_flags & TDF_DETAILS))
    {
      if (v->var == v->new_value)
	fprintf (dump_file, "\n");
      fprintf (dump_file, "Uses of ");
      print_generic_expr (dump_file, v->var);
      fprintf (dump_file, " will be replaced by ");
      print_generic_expr (dump_file, new_value);
      fprintf (dump_file, "\n");
    }
  v->new_value = new_value;
}

/* Apply all current substitutions to gimple operand VALUE.  */

tree
backprop::subst_operand (tree value)
{
  if (const var_info *v = lookup_operand (value))
    return v->new_value;

  return value;
}

/* Assuming that V's SSA name occurs as "lhs" in a statement of the form:

      S: lhs = op (rhs1, rhs2, ...)

   create and insert a new statement:

      S': lhs'= op (rhs1', rhs2', ...)

   where:

      lhs' is a new SSA name
      rhsN' = subst_operand (rhsN)

   Arrange for lhs to be replaced with lhs'.

   Return S'.  After making any required changes to S', the caller should
   call finish_stmt (V, S') to finish scheduling the replacement.  */

gimple *
backprop::copy_and_subst (var_info *v)
{
  gcc_assert (v->new_value == v->var);
  gimple *stmt = SSA_NAME_DEF_STMT (v->var);
  set_new_value (v, copy_ssa_name_fn (m_fn, v->var, stmt));
  if (auto *phi = dyn_cast<gphi *> (stmt))
    {
      gphi *new_phi = create_phi_node (v->new_value, gimple_bb (phi));
      gimple_set_location (new_phi, gimple_location (phi));
      for (unsigned int i = 0; i < gimple_phi_num_args (phi); ++i)
	{
	  tree arg_def = subst_operand (gimple_phi_arg_def (phi, i));
	  SET_USE (gimple_phi_arg_imm_use_ptr (new_phi, i), arg_def);
	}
      return new_phi;
    }
  else
    {
      gimple *new_stmt = gimple_copy (stmt);
      for (unsigned int i = 0; i < gimple_num_ops (new_stmt); ++i)
	{
	  tree *op_ptr = gimple_op_ptr (new_stmt, i);
	  *op_ptr = subst_operand (*op_ptr);
	}
      SSA_NAME_DEF_STMT (v->new_value) = new_stmt;
      gimple_seq_add_stmt (&v->seq, new_stmt);
      return new_stmt;
    }
}

/* Finalize STMT, whose lhs will replace V's SSA name.  */

void
backprop::finish_stmt (var_info *v, gimple *stmt)
{
  if (dump_file && (dump_flags & TDF_DETAILS))
    {
      fprintf (dump_file, "Replacing ");
      print_gimple_stmt (dump_file, SSA_NAME_DEF_STMT (v->var), TDF_SLIM);
      fprintf (dump_file, "     with ");
      print_gimple_stmt (dump_file, stmt, TDF_SLIM);
    }
  tree new_value = v->new_value;
  gimple_stmt_iterator gsi;
  if (auto *phi = dyn_cast<gphi *> (stmt))
    {
      gsi = gsi_for_stmt (phi);
      new_value = degenerate_phi_result (phi);
    }
  else
    {
      gsi = gsi_for_stmt (stmt, &v->seq);
      fold_stmt (&gsi);
      stmt = gsi_stmt (gsi);
      if (gimple_assign_single_p (stmt))
	new_value = gimple_assign_rhs1 (stmt);
    }
  update_stmt (stmt);
  if (new_value && new_value != v->new_value && is_gimple_val (new_value))
    {
      remove_dead_stmt (&gsi);
      set_new_value (v, new_value);
    }
}

/* Optimize CALL, a call to a built-in function, given that V describes
   its lhs.  */

void
backprop::optimize_builtin_call (gcall *call, var_info *v)
{
  /* If we have an f such that -f(x) = f(-x), and if the sign of the result
     doesn't matter, strip any sign operations from the input.  */
  if (v->info.flags.ignore_sign
      && negate_mathfn_p (gimple_call_combined_fn (call)))
    {
      tree new_arg = strip_sign_op (gimple_call_arg (call, 0));
      if (new_arg)
	{
	  call = as_a<gcall *> (copy_and_subst (v));
	  gimple_call_set_arg (call, 0, subst_operand (new_arg));
	  finish_stmt (v, call);
	}
    }
}

/* Optimize V by replacing rhs operand N with RHS<N>, if RHS<N> is nonnull.  */

void
backprop::replace_assign_rhs (var_info *v, tree rhs1, tree rhs2, tree rhs3)
{
  if (!rhs1 && !rhs2 && !rhs3)
    return;

  auto *assign = as_a<gassign *> (copy_and_subst (v));
  if (rhs1)
    gimple_assign_set_rhs1 (assign, subst_operand (rhs1));
  if (rhs2)
    gimple_assign_set_rhs2 (assign, subst_operand (rhs2));
  if (rhs3)
    gimple_assign_set_rhs3 (assign, subst_operand (rhs3));
  finish_stmt (v, assign);
}

/* Optimize ASSIGN given that V describes its lhs.  */

void
backprop::optimize_assign (gassign *assign, var_info *v)
{
  switch (gimple_assign_rhs_code (assign))
    {
    case MULT_EXPR:
    case RDIV_EXPR:
      /* If the sign of the result doesn't matter, strip sign operations
	 from both inputs.  */
      if (v->info.flags.ignore_sign)
	replace_assign_rhs (v, strip_sign_op (gimple_assign_rhs1 (assign)),
			    strip_sign_op (gimple_assign_rhs2 (assign)),
			    NULL_TREE);
      break;

    case COND_EXPR:
      /* If the sign of A ? B : C doesn't matter, strip sign operations
	 from both B and C.  */
      if (v->info.flags.ignore_sign)
	replace_assign_rhs (v, NULL_TREE,
			    strip_sign_op (gimple_assign_rhs2 (assign)),
			    strip_sign_op (gimple_assign_rhs3 (assign)));
      break;

    default:
      break;
    }
}

/* Optimize PHI given that V describes its result.  */

void
backprop::optimize_phi (gphi *phi, var_info *v)
{
  /* If the sign of the result doesn't matter, strip sign operations
     from all arguments.  */
  if (v->info.flags.ignore_sign)
    {
      gphi *replacement = nullptr;
      for (unsigned int i = 0; i < gimple_phi_num_args (phi); ++i)
	{
	  tree arg = gimple_phi_arg_def (phi, i);
	  tree new_arg = strip_sign_op (arg);
	  if (new_arg)
	    {
	      if (!replacement)
		replacement = as_a<gphi *> (copy_and_subst (v));
	      SET_USE (gimple_phi_arg_imm_use_ptr (replacement, i),
		       subst_operand (new_arg));
	    }
	}
      if (replacement)
	finish_stmt (v, replacement);
    }
}

/* V is being replaced with something that may have a different value.
   If that would change the result of any dependent statement,
   make sure that that dependent statement will also be replaced.  */

void
backprop::propagate_change (var_info *v)
{
  use_operand_p use_p;
  imm_use_iterator use_iter;
  FOR_EACH_IMM_USE_FAST (use_p, use_iter, v->var)
    {
      tree lhs = gimple_get_lhs (USE_STMT (use_p));
      if (const var_info *lhsv = lookup_operand (lhs))
	if (lhsv->new_value == lhsv->var)
	  bitmap_set_bit (m_this_worklist, lhsv->index);
    }
}

/* Implement the scheduled replacement for V.  */

void
backprop::commit_replacement (var_info *v)
{
  if (MAY_HAVE_DEBUG_BIND_STMTS)
    insert_debug_temp_for_var_def (NULL, v->var);

  gimple *stmt = SSA_NAME_DEF_STMT (v->var);
  use_operand_p use_p;
  imm_use_iterator iterator;
  FOR_EACH_IMM_USE_STMT (stmt, iterator, v->var)
    {
      FOR_EACH_IMM_USE_ON_STMT (use_p, iterator)
	SET_USE (use_p, v->new_value);
      update_stmt (stmt);
    }

  gimple_stmt_iterator gsi = gsi_for_stmt (SSA_NAME_DEF_STMT (v->var));
  if (v->seq)
    gsi_replace_with_seq_vops (&gsi, v->seq);
  else
    remove_dead_stmt (&gsi, true);
}

void
backprop::execute ()
{
  /* Phase 1: Traverse the function, making optimistic assumptions
     about any phi whose definition we haven't seen.  Add any variables
     that need to be reconsidered to M_NEXT_WORKLIST.  */
  int *postorder = XNEWVEC (int, n_basic_blocks_for_fn (m_fn));
  unsigned int postorder_num = post_order_compute (postorder, false, false);
  for (unsigned int i = 0; i < postorder_num; ++i)
    {
      process_block (BASIC_BLOCK_FOR_FN (m_fn, postorder[i]));
      bitmap_set_bit (m_visited_blocks, postorder[i]);
    }
  XDELETEVEC (postorder);

  /* Phase 2: Use the initial (perhaps overly optimistic) information
     to create a maximal fixed point solution.  Each pass uses a post-order
     walk to reduce the number of repeat visits.  */
  while (!bitmap_empty_p (m_next_worklist))
    {
      std::swap (m_this_worklist, m_next_worklist);
      bitmap_clear (m_next_worklist);
      do
	{
	  var_info *v = pop_from_worklist ();
	  process_var (v->var, v);
	}
      while (!bitmap_empty_p (m_this_worklist));
    }

  /* Phase 3: Do a reverse post-order walk, using information about the uses of
     SSA names to see whether computing a different value would be simpler and
     would have the same effect.  Start to compute the transitive closure of
     SSA names whose definitions are due to be replaced with new
     calculations.  */
  bitmap_clear (m_this_worklist);
  for (unsigned int i = m_vars.length (); i-- > 0;)
    if (var_info *v = m_vars[i])
      {
	gimple *stmt = SSA_NAME_DEF_STMT (v->var);
	if (gcall *call = dyn_cast <gcall *> (stmt))
	  optimize_builtin_call (call, v);
	else if (gassign *assign = dyn_cast <gassign *> (stmt))
	  optimize_assign (assign, v);
	else if (gphi *phi = dyn_cast <gphi *> (stmt))
	  optimize_phi (phi, v);

	if (bitmap_clear_bit (m_this_worklist, v->index))
	  if (v->new_value == v->var)
	    finish_stmt (v, copy_and_subst (v));

	if (v->new_value != v->var)
	  propagate_change (v);
      }

  /* Complete the transitive closure started by the previous loop.  */
  while (!bitmap_empty_p (m_this_worklist))
    {
      var_info *v = m_vars[bitmap_clear_last_set_bit (m_this_worklist)];
      finish_stmt (v, copy_and_subst (v));
      propagate_change (v);
    }

  /* Now that we have a complete set of values that need to change,
     calculate the transitive closure of replacement values.  */
  bool changed = false;
  for (unsigned int i = m_vars.length (); i-- > 0;)
    if (var_info *v = m_vars[i])
      if (v->var != v->new_value)
	if (var_info *newv = lookup_operand (v->new_value))
	  {
	    gcc_assert (newv->index > i);
	    if (!changed && dump_file && (dump_flags & TDF_DETAILS))
	      fprintf (dump_file, "\n");
	    set_new_value (v, newv->new_value);
	    changed = true;
	  }

  if (dump_file && (dump_flags & TDF_DETAILS))
    fprintf (dump_file, "\n");

  /* Phase 4: Do a post-order walk, creating debug bind expressions for
     SSA names that are about to disappear.  Replace all non-debug uses
     of old SSA names with uses of whatever calculation is replacing them.  */
  for (unsigned int i = 0; i < m_vars.length (); ++i)
    if (var_info *v = m_vars[i])
      if (v->var != v->new_value)
	commit_replacement (v);

  /* Phase 5: Remove any statements that might have become dead.  */
  auto_bitmap deleted_vars;
  for (unsigned int i = 0; i < m_vars.length (); ++i)
    if (var_info *v = m_vars[i])
      if (TREE_CODE (v->new_value) == SSA_NAME
	  && has_zero_uses (v->new_value)
	  && bitmap_set_bit (deleted_vars, SSA_NAME_VERSION (v->new_value)))
	{
	  gimple *stmt = SSA_NAME_DEF_STMT (v->new_value);
	  gimple_stmt_iterator gsi = gsi_for_stmt (stmt);
	  remove_dead_stmt (&gsi);
	}

  if (dump_file && (dump_flags & TDF_DETAILS))
    fprintf (dump_file, "\n");
}

const pass_data pass_data_backprop =
{
  GIMPLE_PASS, /* type */
  "backprop", /* name */
  OPTGROUP_NONE, /* optinfo_flags */
  TV_TREE_BACKPROP, /* tv_id */
  ( PROP_cfg | PROP_ssa ), /* properties_required */
  0, /* properties_provided */
  0, /* properties_destroyed */
  0, /* todo_flags_start */
  0, /* todo_flags_finish */
};

class pass_backprop : public gimple_opt_pass
{
public:
  pass_backprop (gcc::context *ctxt)
    : gimple_opt_pass (pass_data_backprop, ctxt)
  {}

  /* opt_pass methods: */
  opt_pass * clone () final override { return new pass_backprop (m_ctxt); }
  bool gate (function *) final override { return flag_ssa_backprop; }
  unsigned int execute (function *) final override;

}; // class pass_backprop

unsigned int
pass_backprop::execute (function *fn)
{
  backprop (fn).execute ();
  return 0;
}

} // anon namespace

gimple_opt_pass *
make_pass_backprop (gcc::context *ctxt)
{
  return new pass_backprop (ctxt);
}
