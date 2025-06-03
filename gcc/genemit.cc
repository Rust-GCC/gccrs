/* Generate code from machine description to emit insns as rtl.
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


#include "bconfig.h"
#include "system.h"
#include "coretypes.h"
#include "tm.h"
#include "rtl.h"
#include "errors.h"
#include "read-md.h"
#include "gensupport.h"


/* Data structure for recording the patterns of insns that have CLOBBERs.
   We use this to output a function that adds these CLOBBERs to a
   previously-allocated PARALLEL expression.  */

struct clobber_pat
{
  struct clobber_ent *insns;
  rtvec pattern;
  int first_clobber;
  struct clobber_pat *next;
  int has_hard_reg;
  rtx_code code;
} *clobber_list;

/* Records one insn that uses the clobber list.  */

struct clobber_ent
{
  int code_number;		/* Counts only insns.  */
  struct clobber_ent *next;
};

static void output_peephole2_scratches	(rtx, FILE*);

/* True for <X>_optab if that optab isn't allowed to fail.  */
static bool nofail_optabs[NUM_OPTABS];

static void
print_code (RTX_CODE code, FILE *file)
{
  const char *p1;
  for (p1 = GET_RTX_NAME (code); *p1; p1++)
    fprintf (file, "%c", TOUPPER (*p1));
}

static void
gen_rtx_scratch (rtx x, enum rtx_code subroutine_type, FILE *file)
{
  if (subroutine_type == DEFINE_PEEPHOLE2)
    {
      fprintf (file, "operand%d", XINT (x, 0));
    }
  else
    {
      fprintf (file, "gen_rtx_SCRATCH (%smode)", GET_MODE_NAME (GET_MODE (x)));
    }
}

/* Print a C expression to construct an RTX just like X,
   substituting any operand references appearing within.  */

static void
gen_exp (rtx x, enum rtx_code subroutine_type, char *used, md_rtx_info *info,
	 FILE *file)
{
  RTX_CODE code;
  int i;
  int len;
  const char *fmt;
  const char *sep = "";

  if (x == 0)
    {
      fprintf (file, "NULL_RTX");
      return;
    }

  code = GET_CODE (x);

  switch (code)
    {
    case MATCH_OPERAND:
    case MATCH_DUP:
      if (used)
	{
	  if (used[XINT (x, 0)])
	    {
	      fprintf (file, "copy_rtx (operand%d)", XINT (x, 0));
	      return;
	    }
	  used[XINT (x, 0)] = 1;
	}
      fprintf (file, "operand%d", XINT (x, 0));
      return;

    case MATCH_OP_DUP:
      fprintf (file, "gen_rtx_fmt_");
      for (i = 0; i < XVECLEN (x, 1); i++)
	fprintf (file, "e");
      fprintf (file, " (GET_CODE (operand%d), ", XINT (x, 0));
      if (GET_MODE (x) == VOIDmode)
	fprintf (file, "GET_MODE (operand%d)", XINT (x, 0));
      else
	fprintf (file, "%smode", GET_MODE_NAME (GET_MODE (x)));
      for (i = 0; i < XVECLEN (x, 1); i++)
	{
	  fprintf (file, ",\n\t\t");
	  gen_exp (XVECEXP (x, 1, i), subroutine_type, used, info, file);
	}
      fprintf (file, ")");
      return;

    case MATCH_OPERATOR:
      fprintf (file, "gen_rtx_fmt_");
      for (i = 0; i < XVECLEN (x, 2); i++)
	fprintf (file, "e");
      fprintf (file, " (GET_CODE (operand%d)", XINT (x, 0));
      fprintf (file, ", %smode", GET_MODE_NAME (GET_MODE (x)));
      for (i = 0; i < XVECLEN (x, 2); i++)
	{
	  fprintf (file, ",\n\t\t");
	  gen_exp (XVECEXP (x, 2, i), subroutine_type, used, info, file);
	}
      fprintf (file, ")");
      return;

    case MATCH_PARALLEL:
    case MATCH_PAR_DUP:
      fprintf (file, "operand%d", XINT (x, 0));
      return;

    case MATCH_SCRATCH:
      gen_rtx_scratch (x, subroutine_type, file);
      return;

    case PC:
      fprintf (file, "pc_rtx");
      return;
    case RETURN:
      fprintf (file, "ret_rtx");
      return;
    case SIMPLE_RETURN:
      fprintf (file, "simple_return_rtx");
      return;
    case CLOBBER:
      if (REG_P (XEXP (x, 0)))
	{
	  fprintf (file, "gen_hard_reg_clobber (%smode, %i)",
		  GET_MODE_NAME (GET_MODE (XEXP (x, 0))),
		  REGNO (XEXP (x, 0)));
	  return;
	}
      break;

    case CONST_INT:
      if (INTVAL (x) == 0)
	fprintf (file, "const0_rtx");
      else if (INTVAL (x) == 1)
	fprintf (file, "const1_rtx");
      else if (INTVAL (x) == -1)
	fprintf (file, "constm1_rtx");
      else if (-MAX_SAVED_CONST_INT <= INTVAL (x)
	       && INTVAL (x) <= MAX_SAVED_CONST_INT)
	fprintf (file, "const_int_rtx[MAX_SAVED_CONST_INT + (%d)]",
		(int) INTVAL (x));
      else if (INTVAL (x) == STORE_FLAG_VALUE)
	fprintf (file, "const_true_rtx");
      else
	{
	  fprintf (file, "GEN_INT (");
	  fprintf (file, HOST_WIDE_INT_PRINT_DEC_C, INTVAL (x));
	  fprintf (file, ")");
	}
      return;

    case CONST_DOUBLE:
      /* Handle `const_double_zero' rtx.  */
      if (CONST_DOUBLE_REAL_VALUE (x)->cl == rvc_zero)
	{
	  fprintf (file, "CONST_DOUBLE_ATOF (\"0\", %smode)",
		  GET_MODE_NAME (GET_MODE (x)));
	  return;
	}
      /* Fall through.  */
    case CONST_FIXED:
    case CONST_WIDE_INT:
      /* These shouldn't be written in MD files.  Instead, the appropriate
	 routines in varasm.cc should be called.  */
      gcc_unreachable ();

    default:
      break;
    }

  fprintf (file, "gen_rtx_");
  print_code (code, file);
  fprintf (file, " (");
  if (!always_void_p (code))
    {
      fprintf (file, "%smode", GET_MODE_NAME (GET_MODE (x)));
      sep = ",\n\t";
    }

  fmt = GET_RTX_FORMAT (code);
  len = GET_RTX_LENGTH (code);
  for (i = 0; i < len; i++)
    {
      if (fmt[i] == '0')
	break;
      fputs (sep, file);
      switch (fmt[i])
	{
	case 'e': case 'u':
	  gen_exp (XEXP (x, i), subroutine_type, used, info, file);
	  break;

	case 'i':
	  fprintf (file, "%u", XINT (x, i));
	  break;

	case 'L':
	  fprintf (file, "%llu", (unsigned long long) XLOC (x, i));
	  break;

	case 'r':
	  fprintf (file, "%u", REGNO (x));
	  break;

	case 'p':
	  /* We don't have a way of parsing polynomial offsets yet,
	     and hopefully never will.  */
	  fprintf (file, "%d", SUBREG_BYTE (x).to_constant ());
	  break;

	case 's':
	  fprintf (file, "\"%s\"", XSTR (x, i));
	  break;

	case 'E':
	  {
	    int j;
	    fprintf (file, "gen_rtvec (%d", XVECLEN (x, i));
	    for (j = 0; j < XVECLEN (x, i); j++)
	      {
		fprintf (file, ",\n\t\t");
		gen_exp (XVECEXP (x, i, j), subroutine_type, used, info, file);
	      }
	    fprintf (file, ")");
	    break;
	  }

	default:
	  gcc_unreachable ();
	}
      sep = ",\n\t";
    }
  fprintf (file, ")");
}

/* Output code to emit the instruction patterns in VEC, with each element
   becoming a separate instruction.  USED is as for gen_exp.  */

static void
gen_emit_seq (rtvec vec, char *used, md_rtx_info *info, FILE *file)
{
  for (int i = 0, len = GET_NUM_ELEM (vec); i < len; ++i)
    {
      bool last_p = (i == len - 1);
      rtx next = RTVEC_ELT (vec, i);
      if (const char *name = get_emit_function (next))
	{
	  fprintf (file, "  %s (", name);
	  gen_exp (next, DEFINE_EXPAND, used, info, file);
	  fprintf (file, ");\n");
	  if (!last_p && needs_barrier_p (next))
	    fprintf (file, "  emit_barrier ();");
	}
      else
	{
	  fprintf (file, "  emit (");
	  gen_exp (next, DEFINE_EXPAND, used, info, file);
	  fprintf (file, ", %s);\n", last_p ? "false" : "true");
	}
    }
}

/* Emit the given C code to the output file.  The code is allowed to
   fail if CAN_FAIL_P.  NAME describes what we're generating,
   for use in error messages.  */

static void
emit_c_code (const char *code, bool can_fail_p, const char *name, FILE *file)
{
  if (can_fail_p)
    fprintf (file, "#define FAIL return (end_sequence (), _val)\n");
  else
    fprintf (file, "#define FAIL _Pragma (\"GCC error \\\"%s cannot FAIL\\\"\")"
	    " (void)0\n", name);
  fprintf (file, "#define DONE return (_val = get_insns (), "
	  "end_sequence (), _val)\n");

  rtx_reader_ptr->print_md_ptr_loc (code, file);
  fprintf (file, "%s\n", code);

  fprintf (file, "#undef DONE\n");
  fprintf (file, "#undef FAIL\n");
}

/* Generate the `gen_...' function for a DEFINE_INSN.  */

static void
gen_insn (md_rtx_info *info, FILE *file)
{
  struct pattern_stats stats;
  int i;

  /* See if the pattern for this insn ends with a group of CLOBBERs of (hard)
     registers or MATCH_SCRATCHes.  If so, store away the information for
     later.  */

  rtx insn = info->def;
  if (XVEC (insn, 1))
    {
      int has_hard_reg = 0;
      rtvec pattern = XVEC (insn, 1);

      /* Look though an explicit parallel. */
      if (GET_NUM_ELEM (pattern) == 1
	  && GET_CODE (RTVEC_ELT (pattern, 0)) == PARALLEL)
	pattern = XVEC (RTVEC_ELT (pattern, 0), 0);

      for (i = GET_NUM_ELEM (pattern) - 1; i > 0; i--)
	{
	  if (GET_CODE (RTVEC_ELT (pattern, i)) != CLOBBER)
	    break;

	  if (REG_P (XEXP (RTVEC_ELT (pattern, i), 0)))
	    has_hard_reg = 1;
	  else if (GET_CODE (XEXP (RTVEC_ELT (pattern, i), 0)) != MATCH_SCRATCH)
	    break;
	}

      if (i != GET_NUM_ELEM (pattern) - 1)
	{
	  struct clobber_pat *p;
	  struct clobber_ent *link = XNEW (struct clobber_ent);
	  int j;

	  link->code_number = info->index;

	  /* See if any previous CLOBBER_LIST entry is the same as this
	     one.  */

	  for (p = clobber_list; p; p = p->next)
	    {
	      if (p->first_clobber != i + 1
		  || GET_NUM_ELEM (p->pattern) != GET_NUM_ELEM (pattern))
		continue;

	      for (j = i + 1; j < GET_NUM_ELEM (pattern); j++)
		{
		  rtx old_rtx = XEXP (RTVEC_ELT (p->pattern, j), 0);
		  rtx new_rtx = XEXP (RTVEC_ELT (pattern, j), 0);

		  /* OLD and NEW_INSN are the same if both are to be a SCRATCH
		     of the same mode,
		     or if both are registers of the same mode and number.  */
		  if (! (GET_CODE (old_rtx) == GET_CODE (new_rtx)
			 && GET_MODE (old_rtx) == GET_MODE (new_rtx)
			 && ((GET_CODE (old_rtx) == MATCH_SCRATCH
			      && GET_CODE (new_rtx) == MATCH_SCRATCH)
			     || (REG_P (old_rtx) && REG_P (new_rtx)
				 && REGNO (old_rtx) == REGNO (new_rtx)))))
		    break;
		}

	      if (j == GET_NUM_ELEM (pattern))
		break;
	    }

	  if (p == 0)
	    {
	      p = XNEW (struct clobber_pat);

	      p->insns = 0;
	      p->pattern = pattern;
	      p->first_clobber = i + 1;
	      p->next = clobber_list;
	      p->has_hard_reg = has_hard_reg;
	      p->code = GET_CODE (insn);
	      clobber_list = p;
	    }

	  link->next = p->insns;
	  p->insns = link;
	}
    }

  /* Don't mention instructions whose names are the null string
     or begin with '*'.  They are in the machine description just
     to be recognized.  */
  if (XSTR (insn, 0)[0] == 0 || XSTR (insn, 0)[0] == '*')
    return;

  fprintf (file, "/* %s:%d */\n", info->loc.filename, info->loc.lineno);

  /* Find out how many operands this function has.  */
  get_pattern_stats (&stats, XVEC (insn, 1));
  if (stats.max_dup_opno > stats.max_opno)
    fatal_at (info->loc, "match_dup operand number has no match_operand");

  /* Output the function name and argument declarations.  */
  fprintf (file, "rtx\ngen_%s (", XSTR (insn, 0));
  if (stats.num_generator_args)
    for (i = 0; i < stats.num_generator_args; i++)
      if (i)
	fprintf (file, ",\n\trtx operand%d ATTRIBUTE_UNUSED", i);
      else
	fprintf (file, "rtx operand%d ATTRIBUTE_UNUSED", i);
  else
    fprintf (file, "void");
  fprintf (file, ")\n");
  fprintf (file, "{\n");

  /* Output code to construct and return the rtl for the instruction body.  */

  rtx pattern = add_implicit_parallel (XVEC (insn, 1));
  /* ??? This is the traditional behavior, but seems suspect.  */
  char *used = (XVECLEN (insn, 1) == 1
		? NULL
		: XCNEWVEC (char, stats.num_generator_args));
  fprintf (file, "  return ");
  gen_exp (pattern, DEFINE_INSN, used, info, file);
  fprintf (file, ";\n}\n\n");
  XDELETEVEC (used);
}

/* Generate the `gen_...' function for a DEFINE_EXPAND.  */

static void
gen_expand (md_rtx_info *info, FILE *file)
{
  struct pattern_stats stats;
  int i;
  char *used;

  rtx expand = info->def;
  if (strlen (XSTR (expand, 0)) == 0)
    fatal_at (info->loc, "define_expand lacks a name");
  if (XVEC (expand, 1) == 0)
    fatal_at (info->loc, "define_expand for %s lacks a pattern",
	      XSTR (expand, 0));

  /* Find out how many operands this function has.  */
  get_pattern_stats (&stats, XVEC (expand, 1));
  if (stats.min_scratch_opno != -1
      && stats.min_scratch_opno <= MAX (stats.max_opno, stats.max_dup_opno))
    fatal_at (info->loc, "define_expand for %s needs to have match_scratch "
			 "numbers above all other operands", XSTR (expand, 0));

  /* Output the function name and argument declarations.  */
  fprintf (file, "rtx\ngen_%s (", XSTR (expand, 0));
  if (stats.num_generator_args)
    for (i = 0; i < stats.num_generator_args; i++)
      if (i)
	fprintf (file, ",\n\trtx operand%d", i);
      else
	fprintf (file, "rtx operand%d", i);
  else
    fprintf (file, "void");
  fprintf (file, ")\n");
  fprintf (file, "{\n");

  /* If we don't have any C code to write, only one insn is being written,
     and no MATCH_DUPs are present, we can just return the desired insn
     like we do for a DEFINE_INSN.  This saves memory.  */
  if ((XSTR (expand, 3) == 0 || *XSTR (expand, 3) == '\0')
      && stats.max_opno >= stats.max_dup_opno
      && XVECLEN (expand, 1) == 1)
    {
      fprintf (file, "  return ");
      gen_exp (XVECEXP (expand, 1, 0), DEFINE_EXPAND, NULL, info, file);
      fprintf (file, ";\n}\n\n");
      return;
    }

  /* For each operand referred to only with MATCH_DUPs,
     make a local variable.  */
  for (i = stats.num_generator_args; i <= stats.max_dup_opno; i++)
    fprintf (file, "  rtx operand%d;\n", i);
  fprintf (file, "  rtx_insn *_val = 0;\n");
  fprintf (file, "  start_sequence ();\n");

  /* The fourth operand of DEFINE_EXPAND is some code to be executed
     before the actual construction.
     This code expects to refer to `operands'
     just as the output-code in a DEFINE_INSN does,
     but here `operands' is an automatic array.
     So copy the operand values there before executing it.  */
  if (XSTR (expand, 3) && *XSTR (expand, 3))
    {
      fprintf (file, "  {\n");
      if (stats.num_operand_vars > 0)
	fprintf (file, "    rtx operands[%d];\n", stats.num_operand_vars);

      /* Output code to copy the arguments into `operands'.  */
      for (i = 0; i < stats.num_generator_args; i++)
	fprintf (file, "    operands[%d] = operand%d;\n", i, i);

      /* Output the special code to be executed before the sequence
	 is generated.  */
      optab_pattern p;
      bool can_fail_p = true;
      if (find_optab (&p, XSTR (expand, 0)))
	{
	  gcc_assert (p.op < NUM_OPTABS);
	  if (nofail_optabs[p.op])
	    can_fail_p = false;
	}
      emit_c_code (XSTR (expand, 3), can_fail_p, XSTR (expand, 0), file);

      /* Output code to copy the arguments back out of `operands'
	 (unless we aren't going to use them at all).  */
      if (XVEC (expand, 1) != 0)
	{
	  for (i = 0; i <= MAX (stats.max_opno, stats.max_dup_opno); i++)
	    {
	      fprintf (file, "    operand%d = operands[%d];\n", i, i);
	      fprintf (file, "    (void) operand%d;\n", i);
	    }
	}
      fprintf (file, "  }\n");
    }

  used = XCNEWVEC (char, stats.num_operand_vars);
  gen_emit_seq (XVEC (expand, 1), used, info, file);
  XDELETEVEC (used);

  /* Call `get_insns' to extract the list of all the
     insns emitted within this gen_... function.  */

  fprintf (file, "  _val = get_insns ();\n");
  fprintf (file, "  end_sequence ();\n");
  fprintf (file, "  return _val;\n}\n\n");
}

/* Like gen_expand, but generates insns resulting from splitting SPLIT.  */

static void
gen_split (md_rtx_info *info, FILE *file)
{
  struct pattern_stats stats;
  int i;
  rtx split = info->def;
  const char *const name =
    ((GET_CODE (split) == DEFINE_PEEPHOLE2) ? "peephole2" : "split");
  const char *unused;
  char *used;

  if (XVEC (split, 0) == 0)
    fatal_at (info->loc, "%s lacks a pattern",
	      GET_RTX_NAME (GET_CODE (split)));
  else if (XVEC (split, 2) == 0)
    fatal_at (info->loc, "%s lacks a replacement pattern",
	      GET_RTX_NAME (GET_CODE (split)));

  /* Find out how many operands this function has.  */

  get_pattern_stats (&stats, XVEC (split, 2));
  unused = (stats.num_operand_vars == 0 ? " ATTRIBUTE_UNUSED" : "");
  used = XCNEWVEC (char, stats.num_operand_vars);

  /* Output the prototype, function name and argument declarations.  */
  if (GET_CODE (split) == DEFINE_PEEPHOLE2)
    {
      fprintf (file, "extern rtx_insn *gen_%s_%d (rtx_insn *, rtx *);\n",
	      name, info->index);
      fprintf (file, "rtx_insn *\ngen_%s_%d (rtx_insn *curr_insn ATTRIBUTE_UNUSED,"
	      " rtx *operands%s)\n",
	      name, info->index, unused);
    }
  else
    {
      fprintf (file, "extern rtx_insn *gen_split_%d (rtx_insn *, rtx *);\n",
	      info->index);
      fprintf (file, "rtx_insn *\ngen_split_%d "
	      "(rtx_insn *curr_insn ATTRIBUTE_UNUSED, rtx *operands%s)\n",
	      info->index, unused);
    }
  fprintf (file, "{\n");

  /* Declare all local variables.  */
  for (i = 0; i < stats.num_operand_vars; i++)
    fprintf (file, "  rtx operand%d;\n", i);
  fprintf (file, "  rtx_insn *_val = NULL;\n");

  if (GET_CODE (split) == DEFINE_PEEPHOLE2)
    output_peephole2_scratches (split, file);

  const char *fn = info->loc.filename;
  for (const char *p = fn; *p; p++)
    if (*p == '/')
      fn = p + 1;

  fprintf (file, "  if (dump_file)\n");
  fprintf (file, "    fprintf (dump_file, \"Splitting with gen_%s_%d (%s:%d)\\n\");\n",
	  name, info->index, fn, info->loc.lineno);

  fprintf (file, "  start_sequence ();\n");

  /* The fourth operand of DEFINE_SPLIT is some code to be executed
     before the actual construction.  */

  if (XSTR (split, 3))
    emit_c_code (XSTR (split, 3), true, name, file);

  /* Output code to copy the arguments back out of `operands'  */
  for (i = 0; i < stats.num_operand_vars; i++)
    {
      fprintf (file, "  operand%d = operands[%d];\n", i, i);
      fprintf (file, "  (void) operand%d;\n", i);
    }

  gen_emit_seq (XVEC (split, 2), used, info, file);

  /* Call `get_insns' to make a list of all the
     insns emitted within this gen_... function.  */

  fprintf (file, "  _val = get_insns ();\n");
  fprintf (file, "  end_sequence ();\n");
  fprintf (file, "  return _val;\n}\n\n");

  free (used);
}

/* Write a function, `add_clobbers', that is given a PARALLEL of sufficient
   size for the insn and an INSN_CODE, and inserts the required CLOBBERs at
   the end of the vector.  */

static void
output_add_clobbers (md_rtx_info *info, FILE *file)
{
  struct clobber_pat *clobber;
  struct clobber_ent *ent;
  int i;

  fprintf (file, "\n\nvoid\nadd_clobbers (rtx pattern ATTRIBUTE_UNUSED, int insn_code_number)\n");
  fprintf (file, "{\n");
  fprintf (file, "  switch (insn_code_number)\n");
  fprintf (file, "    {\n");

  for (clobber = clobber_list; clobber; clobber = clobber->next)
    {
      for (ent = clobber->insns; ent; ent = ent->next)
	fprintf (file, "    case %d:\n", ent->code_number);

      for (i = clobber->first_clobber; i < GET_NUM_ELEM (clobber->pattern); i++)
	{
	  fprintf (file, "      XVECEXP (pattern, 0, %d) = ", i);
	  gen_exp (RTVEC_ELT (clobber->pattern, i),
		   clobber->code, NULL, info, file);
	  fprintf (file, ";\n");
	}

      fprintf (file, "      break;\n\n");
    }

  fprintf (file, "    default:\n");
  fprintf (file, "      gcc_unreachable ();\n");
  fprintf (file, "    }\n");
  fprintf (file, "}\n");
}

/* Write a function, `added_clobbers_hard_reg_p' that is given an insn_code
   number that will have clobbers added (as indicated by `recog') and returns
   1 if those include a clobber of a hard reg or 0 if all of them just clobber
   SCRATCH.  */

static void
output_added_clobbers_hard_reg_p (FILE *file)
{
  struct clobber_pat *clobber;
  struct clobber_ent *ent;
  int clobber_p;
  bool used;

  fprintf (file, "\n\nbool\nadded_clobbers_hard_reg_p (int insn_code_number)\n");
  fprintf (file, "{\n");
  fprintf (file, "  switch (insn_code_number)\n");
  fprintf (file, "    {\n");

  for (clobber_p = 0; clobber_p <= 1; clobber_p++)
    {
      used = false;
      for (clobber = clobber_list; clobber; clobber = clobber->next)
	if (clobber->has_hard_reg == clobber_p)
	  for (ent = clobber->insns; ent; ent = ent->next)
	    {
	      fprintf (file, "    case %d:\n", ent->code_number);
	      used = true;
	    }

      if (used)
	fprintf (file, "      return %s;\n\n", clobber_p ? "true" : "false");
    }

  fprintf (file, "    default:\n");
  fprintf (file, "      gcc_unreachable ();\n");
  fprintf (file, "    }\n");
  fprintf (file, "}\n");
}

/* Generate code to invoke find_free_register () as needed for the
   scratch registers used by the peephole2 pattern in SPLIT.  */

static void
output_peephole2_scratches (rtx split, FILE *file)
{
  int i;
  int insn_nr = 0;
  bool first = true;

  for (i = 0; i < XVECLEN (split, 0); i++)
    {
      rtx elt = XVECEXP (split, 0, i);
      if (GET_CODE (elt) == MATCH_SCRATCH)
	{
	  int last_insn_nr = insn_nr;
	  int cur_insn_nr = insn_nr;
	  int j;
	  for (j = i + 1; j < XVECLEN (split, 0); j++)
	    if (GET_CODE (XVECEXP (split, 0, j)) == MATCH_DUP)
	      {
		if (XINT (XVECEXP (split, 0, j), 0) == XINT (elt, 0))
		  last_insn_nr = cur_insn_nr;
	      }
	    else if (GET_CODE (XVECEXP (split, 0, j)) != MATCH_SCRATCH)
	      cur_insn_nr++;

	  if (first)
	    {
	      fprintf (file, "  HARD_REG_SET _regs_allocated;\n");
	      fprintf (file, "  CLEAR_HARD_REG_SET (_regs_allocated);\n");
	      first = false;
	    }

	  fprintf (file, "  if ((operands[%d] = peep2_find_free_register (%d, %d, \"%s\", %smode, &_regs_allocated)) == NULL_RTX)\n\
    return NULL;\n",
		  XINT (elt, 0),
		  insn_nr, last_insn_nr,
		  XSTR (elt, 1),
		  GET_MODE_NAME (GET_MODE (elt)));

	}
      else if (GET_CODE (elt) != MATCH_DUP)
	insn_nr++;
    }
}

/* Print "arg<N>" parameter declarations for each argument N of ONAME.  */

static void
print_overload_arguments (overloaded_name *oname, FILE *file)
{
  for (unsigned int i = 0; i < oname->arg_types.length (); ++i)
    fprintf (file, "%s%s arg%d", i == 0 ? "" : ", ", oname->arg_types[i], i);
}

/* Print code to test whether INSTANCE should be chosen, given that
   argument N of the overload is available as "arg<N>".  */

static void
print_overload_test (overloaded_instance *instance, FILE *file)
{
  for (unsigned int i = 0; i < instance->arg_values.length (); ++i)
    fprintf (file, "%sarg%d == %s", i == 0 ? "  if (" : "\n      && ",
	    i, instance->arg_values[i]);
  fprintf (file, ")\n");
}

/* Emit a maybe_code_for_* function for ONAME.  */

static void
handle_overloaded_code_for (overloaded_name *oname, FILE *file)
{
  /* Print the function prototype.  */
  fprintf (file, "\ninsn_code\nmaybe_code_for_%s (", oname->name);
  print_overload_arguments (oname, file);
  fprintf (file, ")\n{\n");

  /* Use a sequence of "if" statements for each instance.  */
  for (overloaded_instance *instance = oname->first_instance;
       instance; instance = instance->next)
    {
      print_overload_test (instance, file);
      fprintf (file, "    return CODE_FOR_%s;\n", instance->name);
    }

  /* Return null if no match was found.  */
  fprintf (file, "  return CODE_FOR_nothing;\n}\n");
}

/* Emit a maybe_gen_* function for ONAME.  */

static void
handle_overloaded_gen (overloaded_name *oname, FILE *file)
{
  unsigned HOST_WIDE_INT seen = 0;
  /* All patterns must have the same number of operands.  */
  for (overloaded_instance *instance = oname->first_instance->next;
       instance; instance = instance->next)
    {
      pattern_stats stats;
      get_pattern_stats (&stats, XVEC (instance->insn, 1));
      unsigned HOST_WIDE_INT mask
	= HOST_WIDE_INT_1U << stats.num_generator_args;
      if (seen & mask)
	continue;

      seen |= mask;

      /* Print the function prototype.  */
      fprintf (file, "\nrtx\nmaybe_gen_%s (", oname->name);
      print_overload_arguments (oname, file);
      for (int i = 0; i < stats.num_generator_args; ++i)
	fprintf (file, ", rtx x%d", i);
      fprintf (file, ")\n{\n");

      /* Use maybe_code_for_*, instead of duplicating the selection
	 logic here.  */
      fprintf (file, "  insn_code code = maybe_code_for_%s (", oname->name);
      for (unsigned int i = 0; i < oname->arg_types.length (); ++i)
	fprintf (file, "%sarg%d", i == 0 ? "" : ", ", i);
      fprintf (file, ");\n"
	      "  if (code != CODE_FOR_nothing)\n"
	      "    {\n"
	      "      gcc_assert (insn_data[code].n_generator_args == %d);\n"
	      "      return GEN_FCN (code) (", stats.num_generator_args);
      for (int i = 0; i < stats.num_generator_args; ++i)
	fprintf (file, "%sx%d", i == 0 ? "" : ", ", i);
      fprintf (file, ");\n"
	      "    }\n"
	      "  else\n"
	      "    return NULL_RTX;\n"
	      "}\n");
    }
}

void
print_header (FILE *file)
{
  fprintf (file, "/* Generated automatically by the program `genemit'\n\
from the machine description file `md'.  */\n\n");

  fprintf (file, "#define IN_TARGET_CODE 1\n");
  fprintf (file, "#include \"config.h\"\n");
  fprintf (file, "#include \"system.h\"\n");
  fprintf (file, "#include \"coretypes.h\"\n");
  fprintf (file, "#include \"backend.h\"\n");
  fprintf (file, "#include \"predict.h\"\n");
  fprintf (file, "#include \"tree.h\"\n");
  fprintf (file, "#include \"rtl.h\"\n");
  fprintf (file, "#include \"alias.h\"\n");
  fprintf (file, "#include \"varasm.h\"\n");
  fprintf (file, "#include \"stor-layout.h\"\n");
  fprintf (file, "#include \"calls.h\"\n");
  fprintf (file, "#include \"memmodel.h\"\n");
  fprintf (file, "#include \"tm_p.h\"\n");
  fprintf (file, "#include \"flags.h\"\n");
  fprintf (file, "#include \"insn-config.h\"\n");
  fprintf (file, "#include \"expmed.h\"\n");
  fprintf (file, "#include \"dojump.h\"\n");
  fprintf (file, "#include \"explow.h\"\n");
  fprintf (file, "#include \"emit-rtl.h\"\n");
  fprintf (file, "#include \"stmt.h\"\n");
  fprintf (file, "#include \"expr.h\"\n");
  fprintf (file, "#include \"insn-codes.h\"\n");
  fprintf (file, "#include \"optabs.h\"\n");
  fprintf (file, "#include \"dfp.h\"\n");
  fprintf (file, "#include \"output.h\"\n");
  fprintf (file, "#include \"recog.h\"\n");
  fprintf (file, "#include \"df.h\"\n");
  fprintf (file, "#include \"resource.h\"\n");
  fprintf (file, "#include \"reload.h\"\n");
  fprintf (file, "#include \"diagnostic-core.h\"\n");
  fprintf (file, "#include \"regs.h\"\n");
  fprintf (file, "#include \"tm-constrs.h\"\n");
  fprintf (file, "#include \"ggc.h\"\n");
  fprintf (file, "#include \"target.h\"\n\n");
}

auto_vec<FILE *, 10> output_files;

static bool
handle_arg (const char *arg)
{
  if (arg[1] == 'O')
    {
      FILE *file = fopen (&arg[2], "w");
      output_files.safe_push (file);
      return true;
    }
  return false;
}

int
main (int argc, const char **argv)
{
  progname = "genemit";

  if (!init_rtx_reader_args_cb (argc, argv, handle_arg))
    return (FATAL_EXIT_CODE);

#define DEF_INTERNAL_OPTAB_FN(NAME, FLAGS, OPTAB, TYPE) \
  nofail_optabs[OPTAB##_optab] = true;
#include "internal-fn.def"

  /* Assign sequential codes to all entries in the machine description
     in parallel with the tables in insn-output.cc.  */

  md_rtx_info info;

  if (output_files.is_empty ())
    output_files.safe_push (stdout);

  for (auto f : output_files)
    print_header (f);

  FILE *file = NULL;
  unsigned file_idx;

  /* Read the machine description.  */
  while (read_md_rtx (&info))
    {
      file = choose_output (output_files, file_idx);

      switch (GET_CODE (info.def))
	{
	case DEFINE_INSN:
	  gen_insn (&info, file);
	  break;

	case DEFINE_EXPAND:
	  fprintf (file, "/* %s:%d */\n", info.loc.filename, info.loc.lineno);
	  gen_expand (&info, file);
	  break;

	case DEFINE_SPLIT:
	  fprintf (file, "/* %s:%d */\n", info.loc.filename, info.loc.lineno);
	  gen_split (&info, file);
	  break;

	case DEFINE_PEEPHOLE2:
	  fprintf (file, "/* %s:%d */\n", info.loc.filename, info.loc.lineno);
	  gen_split (&info, file);
	  break;

	default:
	  break;
	}
    }

  file = choose_output (output_files, file_idx);

  /* Write out the routines to add CLOBBERs to a pattern and say whether they
     clobber a hard reg.  */
  output_add_clobbers (&info, file);
  output_added_clobbers_hard_reg_p (file);

  for (overloaded_name *oname = rtx_reader_ptr->get_overloads ();
       oname; oname = oname->next)
    {
      handle_overloaded_code_for (oname, file);
      handle_overloaded_gen (oname, file);
    }

  int ret = SUCCESS_EXIT_CODE;
  for (FILE *f : output_files)
    if (fclose (f) != 0)
      ret = FATAL_EXIT_CODE;

  return ret;
}
