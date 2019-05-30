/* Utility routines for data type conversion for GCC.
   Copyright (C) 1987-2019 Free Software Foundation, Inc.

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


/* These routines are somewhat language-independent utility function
   intended to be called by the language-specific convert () functions.  */

#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "target.h"
#include "tree.h"
#include "diagnostic-core.h"
#include "fold-const.h"
#include "stor-layout.h"
#include "convert.h"
#include "langhooks.h"
#include "builtins.h"
#include "ubsan.h"
#include "stringpool.h"
#include "attribs.h"
#include "asan.h"
#include "selftest.h"

#define maybe_fold_build1_loc(FOLD_P, LOC, CODE, TYPE, EXPR) \
  ((FOLD_P) ? fold_build1_loc (LOC, CODE, TYPE, EXPR)	     \
   : build1_loc (LOC, CODE, TYPE, EXPR))
#define maybe_fold_build2_loc(FOLD_P, LOC, CODE, TYPE, EXPR1, EXPR2) \
  ((FOLD_P) ? fold_build2_loc (LOC, CODE, TYPE, EXPR1, EXPR2)	     \
   : build2_loc (LOC, CODE, TYPE, EXPR1, EXPR2))

/* Convert EXPR to some pointer or reference type TYPE.
   EXPR must be pointer, reference, integer, enumeral, or literal zero;
   in other cases error is called.  If FOLD_P is true, try to fold the
   expression.  */

static tree
convert_to_pointer_1 (tree type, tree expr, bool fold_p)
{
  location_t loc = EXPR_LOCATION (expr);
  if (TREE_TYPE (expr) == type)
    return expr;

  switch (TREE_CODE (TREE_TYPE (expr)))
    {
    case POINTER_TYPE:
    case REFERENCE_TYPE:
      {
        /* If the pointers point to different address spaces, conversion needs
	   to be done via a ADDR_SPACE_CONVERT_EXPR instead of a NOP_EXPR.  */
	addr_space_t to_as = TYPE_ADDR_SPACE (TREE_TYPE (type));
	addr_space_t from_as = TYPE_ADDR_SPACE (TREE_TYPE (TREE_TYPE (expr)));

	if (to_as == from_as)
	  return maybe_fold_build1_loc (fold_p, loc, NOP_EXPR, type, expr);
	else
	  return maybe_fold_build1_loc (fold_p, loc, ADDR_SPACE_CONVERT_EXPR,
					type, expr);
      }

    case INTEGER_TYPE:
    case ENUMERAL_TYPE:
    case BOOLEAN_TYPE:
      {
	/* If the input precision differs from the target pointer type
	   precision, first convert the input expression to an integer type of
	   the target precision.  Some targets, e.g. VMS, need several pointer
	   sizes to coexist so the latter isn't necessarily POINTER_SIZE.  */
	unsigned int pprec = TYPE_PRECISION (type);
	unsigned int eprec = TYPE_PRECISION (TREE_TYPE (expr));

	if (eprec != pprec)
	  expr
	    = maybe_fold_build1_loc (fold_p, loc, NOP_EXPR,
				     lang_hooks.types.type_for_size (pprec, 0),
				     expr);
      }
      return maybe_fold_build1_loc (fold_p, loc, CONVERT_EXPR, type, expr);

    default:
      error ("cannot convert to a pointer type");
      return convert_to_pointer_1 (type, integer_zero_node, fold_p);
    }
}

/* Subroutine of the various convert_to_*_maybe_fold routines.

   If a location wrapper has been folded to a constant (presumably of
   a different type), re-wrap the new constant with a location wrapper.  */

tree
preserve_any_location_wrapper (tree result, tree orig_expr)
{
  if (CONSTANT_CLASS_P (result) && location_wrapper_p (orig_expr))
    {
      if (result == TREE_OPERAND (orig_expr, 0))
	return orig_expr;
      else
	return maybe_wrap_with_location (result, EXPR_LOCATION (orig_expr));
    }

  return result;
}

/* A wrapper around convert_to_pointer_1 that always folds the
   expression.  */

tree
convert_to_pointer (tree type, tree expr)
{
  return convert_to_pointer_1 (type, expr, true);
}

/* A wrapper around convert_to_pointer_1 that only folds the
   expression if DOFOLD, or if it is CONSTANT_CLASS_OR_WRAPPER_P.  */

tree
convert_to_pointer_maybe_fold (tree type, tree expr, bool dofold)
{
  tree result
    = convert_to_pointer_1 (type, expr,
			    dofold || CONSTANT_CLASS_OR_WRAPPER_P (expr));
  return preserve_any_location_wrapper (result, expr);
}

/* Convert EXPR to some floating-point type TYPE.

   EXPR must be float, fixed-point, integer, or enumeral;
   in other cases error is called.  If FOLD_P is true, try to fold
   the expression.  */

static tree
convert_to_real_1 (tree type, tree expr, bool fold_p)
{
  enum built_in_function fcode = builtin_mathfn_code (expr);
  tree itype = TREE_TYPE (expr);
  location_t loc = EXPR_LOCATION (expr);

  if (TREE_CODE (expr) == COMPOUND_EXPR)
    {
      tree t = convert_to_real_1 (type, TREE_OPERAND (expr, 1), fold_p);
      if (t == TREE_OPERAND (expr, 1))
	return expr;
      return build2_loc (EXPR_LOCATION (expr), COMPOUND_EXPR, TREE_TYPE (t),
			 TREE_OPERAND (expr, 0), t);
    }    

  /* Disable until we figure out how to decide whether the functions are
     present in runtime.  */
  /* Convert (float)sqrt((double)x) where x is float into sqrtf(x) */
  if (optimize
      && (TYPE_MODE (type) == TYPE_MODE (double_type_node)
          || TYPE_MODE (type) == TYPE_MODE (float_type_node)))
    {
      switch (fcode)
        {
#define CASE_MATHFN(FN) case BUILT_IN_##FN: case BUILT_IN_##FN##L:
	  CASE_MATHFN (COSH)
	  CASE_MATHFN (EXP)
	  CASE_MATHFN (EXP10)
	  CASE_MATHFN (EXP2)
 	  CASE_MATHFN (EXPM1)
	  CASE_MATHFN (GAMMA)
	  CASE_MATHFN (J0)
	  CASE_MATHFN (J1)
	  CASE_MATHFN (LGAMMA)
	  CASE_MATHFN (POW10)
	  CASE_MATHFN (SINH)
	  CASE_MATHFN (TGAMMA)
	  CASE_MATHFN (Y0)
	  CASE_MATHFN (Y1)
	    /* The above functions may set errno differently with float
	       input or output so this transformation is not safe with
	       -fmath-errno.  */
	    if (flag_errno_math)
	      break;
	    gcc_fallthrough ();
	  CASE_MATHFN (ACOS)
	  CASE_MATHFN (ACOSH)
	  CASE_MATHFN (ASIN)
 	  CASE_MATHFN (ASINH)
 	  CASE_MATHFN (ATAN)
	  CASE_MATHFN (ATANH)
 	  CASE_MATHFN (CBRT)
 	  CASE_MATHFN (COS)
 	  CASE_MATHFN (ERF)
 	  CASE_MATHFN (ERFC)
	  CASE_MATHFN (LOG)
	  CASE_MATHFN (LOG10)
	  CASE_MATHFN (LOG2)
 	  CASE_MATHFN (LOG1P)
 	  CASE_MATHFN (SIN)
 	  CASE_MATHFN (TAN)
 	  CASE_MATHFN (TANH)
	    /* The above functions are not safe to do this conversion.  */
	    if (!flag_unsafe_math_optimizations)
	      break;
	    gcc_fallthrough ();
	  CASE_MATHFN (SQRT)
	  CASE_MATHFN (FABS)
	  CASE_MATHFN (LOGB)
#undef CASE_MATHFN
	    if (call_expr_nargs (expr) != 1
		|| !SCALAR_FLOAT_TYPE_P (TREE_TYPE (CALL_EXPR_ARG (expr, 0))))
	      break;
	    {
	      tree arg0 = strip_float_extensions (CALL_EXPR_ARG (expr, 0));
	      tree newtype = type;

	      /* We have (outertype)sqrt((innertype)x).  Choose the wider mode
		 from the both as the safe type for operation.  */
	      if (TYPE_PRECISION (TREE_TYPE (arg0)) > TYPE_PRECISION (type))
		newtype = TREE_TYPE (arg0);

	      /* We consider to convert

		     (T1) sqrtT2 ((T2) exprT3)
		 to
		     (T1) sqrtT4 ((T4) exprT3)

		  , where T1 is TYPE, T2 is ITYPE, T3 is TREE_TYPE (ARG0),
		 and T4 is NEWTYPE.  All those types are of floating point types.
		 T4 (NEWTYPE) should be narrower than T2 (ITYPE). This conversion
		 is safe only if P1 >= P2*2+2, where P1 and P2 are precisions of
		 T2 and T4.  See the following URL for a reference:
		 http://stackoverflow.com/questions/9235456/determining-
                 floating-point-square-root
		 */
	      if ((fcode == BUILT_IN_SQRT || fcode == BUILT_IN_SQRTL)
		  && !flag_unsafe_math_optimizations)
		{
		  /* The following conversion is unsafe even the precision condition
		     below is satisfied:

		     (float) sqrtl ((long double) double_val) -> (float) sqrt (double_val)
		    */
		  if (TYPE_MODE (type) != TYPE_MODE (newtype))
		    break;

		  int p1 = REAL_MODE_FORMAT (TYPE_MODE (itype))->p;
		  int p2 = REAL_MODE_FORMAT (TYPE_MODE (newtype))->p;
		  if (p1 < p2 * 2 + 2)
		    break;
		}

	      /* Be careful about integer to fp conversions.
		 These may overflow still.  */
	      if (FLOAT_TYPE_P (TREE_TYPE (arg0))
		  && TYPE_PRECISION (newtype) < TYPE_PRECISION (itype)
		  && (TYPE_MODE (newtype) == TYPE_MODE (double_type_node)
		      || TYPE_MODE (newtype) == TYPE_MODE (float_type_node)))
		{
		  tree fn = mathfn_built_in (newtype, fcode);
		  if (fn)
		    {
		      tree arg = convert_to_real_1 (newtype, arg0, fold_p);
		      expr = build_call_expr (fn, 1, arg);
		      if (newtype == type)
			return expr;
		    }
		}
	    }
	default:
	  break;
	}
    }

  /* Propagate the cast into the operation.  */
  if (itype != type && FLOAT_TYPE_P (type))
    switch (TREE_CODE (expr))
      {
	/* Convert (float)-x into -(float)x.  This is safe for
	   round-to-nearest rounding mode when the inner type is float.  */
	case ABS_EXPR:
	case NEGATE_EXPR:
	  if (!flag_rounding_math
	      && FLOAT_TYPE_P (itype)
	      && TYPE_PRECISION (type) < TYPE_PRECISION (itype))
	    {
	      tree arg = convert_to_real_1 (type, TREE_OPERAND (expr, 0),
					    fold_p);
	      return build1 (TREE_CODE (expr), type, arg);
	    }
	  break;
	/* Convert (outertype)((innertype0)a+(innertype1)b)
	   into ((newtype)a+(newtype)b) where newtype
	   is the widest mode from all of these.  */
	case PLUS_EXPR:
	case MINUS_EXPR:
	case MULT_EXPR:
	case RDIV_EXPR:
	   {
	     tree arg0 = strip_float_extensions (TREE_OPERAND (expr, 0));
	     tree arg1 = strip_float_extensions (TREE_OPERAND (expr, 1));

	     if (FLOAT_TYPE_P (TREE_TYPE (arg0))
		 && FLOAT_TYPE_P (TREE_TYPE (arg1))
		 && DECIMAL_FLOAT_TYPE_P (itype) == DECIMAL_FLOAT_TYPE_P (type))
	       {
		  tree newtype = type;

		  if (TYPE_MODE (TREE_TYPE (arg0)) == SDmode
		      || TYPE_MODE (TREE_TYPE (arg1)) == SDmode
		      || TYPE_MODE (type) == SDmode)
		    newtype = dfloat32_type_node;
		  if (TYPE_MODE (TREE_TYPE (arg0)) == DDmode
		      || TYPE_MODE (TREE_TYPE (arg1)) == DDmode
		      || TYPE_MODE (type) == DDmode)
		    newtype = dfloat64_type_node;
		  if (TYPE_MODE (TREE_TYPE (arg0)) == TDmode
		      || TYPE_MODE (TREE_TYPE (arg1)) == TDmode
		      || TYPE_MODE (type) == TDmode)
                    newtype = dfloat128_type_node;
		  if (newtype == dfloat32_type_node
		      || newtype == dfloat64_type_node
		      || newtype == dfloat128_type_node)
		    {
		      expr = build2 (TREE_CODE (expr), newtype,
				     convert_to_real_1 (newtype, arg0,
							fold_p),
				     convert_to_real_1 (newtype, arg1,
							fold_p));
		      if (newtype == type)
			return expr;
		      break;
		    }

		  if (TYPE_PRECISION (TREE_TYPE (arg0)) > TYPE_PRECISION (newtype))
		    newtype = TREE_TYPE (arg0);
		  if (TYPE_PRECISION (TREE_TYPE (arg1)) > TYPE_PRECISION (newtype))
		    newtype = TREE_TYPE (arg1);
		  /* Sometimes this transformation is safe (cannot
		     change results through affecting double rounding
		     cases) and sometimes it is not.  If NEWTYPE is
		     wider than TYPE, e.g. (float)((long double)double
		     + (long double)double) converted to
		     (float)(double + double), the transformation is
		     unsafe regardless of the details of the types
		     involved; double rounding can arise if the result
		     of NEWTYPE arithmetic is a NEWTYPE value half way
		     between two representable TYPE values but the
		     exact value is sufficiently different (in the
		     right direction) for this difference to be
		     visible in ITYPE arithmetic.  If NEWTYPE is the
		     same as TYPE, however, the transformation may be
		     safe depending on the types involved: it is safe
		     if the ITYPE has strictly more than twice as many
		     mantissa bits as TYPE, can represent infinities
		     and NaNs if the TYPE can, and has sufficient
		     exponent range for the product or ratio of two
		     values representable in the TYPE to be within the
		     range of normal values of ITYPE.  */
		  if (TYPE_PRECISION (newtype) < TYPE_PRECISION (itype)
		      && (flag_unsafe_math_optimizations
			  || (TYPE_PRECISION (newtype) == TYPE_PRECISION (type)
			      && real_can_shorten_arithmetic (TYPE_MODE (itype),
							      TYPE_MODE (type))
			      && !excess_precision_type (newtype))))
		    {
		      expr = build2 (TREE_CODE (expr), newtype,
				     convert_to_real_1 (newtype, arg0,
							fold_p),
				     convert_to_real_1 (newtype, arg1,
							fold_p));
		      if (newtype == type)
			return expr;
		    }
	       }
	   }
	  break;
	default:
	  break;
      }

  switch (TREE_CODE (TREE_TYPE (expr)))
    {
    case REAL_TYPE:
      /* Ignore the conversion if we don't need to store intermediate
	 results and neither type is a decimal float.  */
      return build1_loc (loc,
			 (flag_float_store
			  || DECIMAL_FLOAT_TYPE_P (type)
			  || DECIMAL_FLOAT_TYPE_P (itype))
			 ? CONVERT_EXPR : NOP_EXPR, type, expr);

    case INTEGER_TYPE:
    case ENUMERAL_TYPE:
    case BOOLEAN_TYPE:
      return build1 (FLOAT_EXPR, type, expr);

    case FIXED_POINT_TYPE:
      return build1 (FIXED_CONVERT_EXPR, type, expr);

    case COMPLEX_TYPE:
      return convert (type,
		      maybe_fold_build1_loc (fold_p, loc, REALPART_EXPR,
					     TREE_TYPE (TREE_TYPE (expr)),
					     expr));

    case POINTER_TYPE:
    case REFERENCE_TYPE:
      error ("pointer value used where a floating point was expected");
      return convert_to_real_1 (type, integer_zero_node, fold_p);

    default:
      error ("aggregate value used where a floating point was expected");
      return convert_to_real_1 (type, integer_zero_node, fold_p);
    }
}

/* A wrapper around convert_to_real_1 that always folds the
   expression.  */

tree
convert_to_real (tree type, tree expr)
{
  return convert_to_real_1 (type, expr, true);
}

/* A wrapper around convert_to_real_1 that only folds the
   expression if DOFOLD, or if it is CONSTANT_CLASS_OR_WRAPPER_P.  */

tree
convert_to_real_maybe_fold (tree type, tree expr, bool dofold)
{
  tree result
    = convert_to_real_1 (type, expr,
			 dofold || CONSTANT_CLASS_OR_WRAPPER_P (expr));
  return preserve_any_location_wrapper (result, expr);
}

/* Try to narrow EX_FORM ARG0 ARG1 in narrowed arg types producing a
   result in TYPE.  */

static tree
do_narrow (location_t loc,
	   enum tree_code ex_form, tree type, tree arg0, tree arg1,
	   tree expr, unsigned inprec, unsigned outprec, bool dofold)
{
  /* Do the arithmetic in type TYPEX,
     then convert result to TYPE.  */
  tree typex = type;

  /* Can't do arithmetic in enumeral types
     so use an integer type that will hold the values.  */
  if (TREE_CODE (typex) == ENUMERAL_TYPE)
    typex = lang_hooks.types.type_for_size (TYPE_PRECISION (typex),
					    TYPE_UNSIGNED (typex));

  /* The type demotion below might cause doing unsigned arithmetic
     instead of signed, and thus hide overflow bugs.  */
  if ((ex_form == PLUS_EXPR || ex_form == MINUS_EXPR)
      && !TYPE_UNSIGNED (typex)
      && sanitize_flags_p (SANITIZE_SI_OVERFLOW))
    return NULL_TREE;

  /* But now perhaps TYPEX is as wide as INPREC.
     In that case, do nothing special here.
     (Otherwise would recurse infinitely in convert.  */
  if (TYPE_PRECISION (typex) != inprec)
    {
      /* Don't do unsigned arithmetic where signed was wanted,
	 or vice versa.
	 Exception: if both of the original operands were
	 unsigned then we can safely do the work as unsigned.
	 Exception: shift operations take their type solely
	 from the first argument.
	 Exception: the LSHIFT_EXPR case above requires that
	 we perform this operation unsigned lest we produce
	 signed-overflow undefinedness.
	 And we may need to do it as unsigned
	 if we truncate to the original size.  */
      if (TYPE_UNSIGNED (TREE_TYPE (expr))
	  || (TYPE_UNSIGNED (TREE_TYPE (arg0))
	      && (TYPE_UNSIGNED (TREE_TYPE (arg1))
		  || ex_form == LSHIFT_EXPR
		  || ex_form == RSHIFT_EXPR
		  || ex_form == LROTATE_EXPR
		  || ex_form == RROTATE_EXPR))
	  || ex_form == LSHIFT_EXPR
	  /* If we have !flag_wrapv, and either ARG0 or
	     ARG1 is of a signed type, we have to do
	     PLUS_EXPR, MINUS_EXPR or MULT_EXPR in an unsigned
	     type in case the operation in outprec precision
	     could overflow.  Otherwise, we would introduce
	     signed-overflow undefinedness.  */
	  || ((!(INTEGRAL_TYPE_P (TREE_TYPE (arg0))
		 && TYPE_OVERFLOW_WRAPS (TREE_TYPE (arg0)))
	       || !(INTEGRAL_TYPE_P (TREE_TYPE (arg1))
		    && TYPE_OVERFLOW_WRAPS (TREE_TYPE (arg1))))
	      && ((TYPE_PRECISION (TREE_TYPE (arg0)) * 2u
		   > outprec)
		  || (TYPE_PRECISION (TREE_TYPE (arg1)) * 2u
		      > outprec))
	      && (ex_form == PLUS_EXPR
		  || ex_form == MINUS_EXPR
		  || ex_form == MULT_EXPR)))
	{
	  if (!TYPE_UNSIGNED (typex))
	    typex = unsigned_type_for (typex);
	}
      else
	{
	  if (TYPE_UNSIGNED (typex))
	    typex = signed_type_for (typex);
	}
      /* We should do away with all this once we have a proper
	 type promotion/demotion pass, see PR45397.  */
      expr = maybe_fold_build2_loc (dofold, loc, ex_form, typex,
				    convert (typex, arg0),
				    convert (typex, arg1));
      return convert (type, expr);
    }
  
  return NULL_TREE;
}

/* Convert EXPR to some integer (or enum) type TYPE.

   EXPR must be pointer, integer, discrete (enum, char, or bool), float,
   fixed-point or vector; in other cases error is called.

   If DOFOLD is TRUE, we try to simplify newly-created patterns by folding.

   The result of this is always supposed to be a newly created tree node
   not in use in any existing structure.  */

static tree
convert_to_integer_1 (tree type, tree expr, bool dofold)
{
  enum tree_code ex_form = TREE_CODE (expr);
  tree intype = TREE_TYPE (expr);
  unsigned int inprec = element_precision (intype);
  unsigned int outprec = element_precision (type);
  location_t loc = EXPR_LOCATION (expr);

  /* An INTEGER_TYPE cannot be incomplete, but an ENUMERAL_TYPE can
     be.  Consider `enum E = { a, b = (enum E) 3 };'.  */
  if (!COMPLETE_TYPE_P (type))
    {
      error ("conversion to incomplete type");
      return error_mark_node;
    }

  if (ex_form == COMPOUND_EXPR)
    {
      tree t = convert_to_integer_1 (type, TREE_OPERAND (expr, 1), dofold);
      if (t == TREE_OPERAND (expr, 1))
	return expr;
      return build2_loc (EXPR_LOCATION (expr), COMPOUND_EXPR, TREE_TYPE (t),
			 TREE_OPERAND (expr, 0), t);
    }    

  /* Convert e.g. (long)round(d) -> lround(d).  */
  /* If we're converting to char, we may encounter differing behavior
     between converting from double->char vs double->long->char.
     We're in "undefined" territory but we prefer to be conservative,
     so only proceed in "unsafe" math mode.  */
  if (optimize
      && (flag_unsafe_math_optimizations
	  || (long_integer_type_node
	      && outprec >= TYPE_PRECISION (long_integer_type_node))))
    {
      tree s_expr = strip_float_extensions (expr);
      tree s_intype = TREE_TYPE (s_expr);
      const enum built_in_function fcode = builtin_mathfn_code (s_expr);
      tree fn = 0;

      switch (fcode)
        {
	CASE_FLT_FN (BUILT_IN_CEIL):
	CASE_FLT_FN_FLOATN_NX (BUILT_IN_CEIL):
	  /* Only convert in ISO C99 mode.  */
	  if (!targetm.libc_has_function (function_c99_misc))
	    break;
	  if (outprec < TYPE_PRECISION (integer_type_node)
	      || (outprec == TYPE_PRECISION (integer_type_node)
		  && !TYPE_UNSIGNED (type)))
	    fn = mathfn_built_in (s_intype, BUILT_IN_ICEIL);
	  else if (outprec == TYPE_PRECISION (long_integer_type_node)
		   && !TYPE_UNSIGNED (type))
	    fn = mathfn_built_in (s_intype, BUILT_IN_LCEIL);
	  else if (outprec == TYPE_PRECISION (long_long_integer_type_node)
		   && !TYPE_UNSIGNED (type))
	    fn = mathfn_built_in (s_intype, BUILT_IN_LLCEIL);
	  break;

	CASE_FLT_FN (BUILT_IN_FLOOR):
	CASE_FLT_FN_FLOATN_NX (BUILT_IN_FLOOR):
	  /* Only convert in ISO C99 mode.  */
	  if (!targetm.libc_has_function (function_c99_misc))
	    break;
	  if (outprec < TYPE_PRECISION (integer_type_node)
	      || (outprec == TYPE_PRECISION (integer_type_node)
		  && !TYPE_UNSIGNED (type)))
	    fn = mathfn_built_in (s_intype, BUILT_IN_IFLOOR);
	  else if (outprec == TYPE_PRECISION (long_integer_type_node)
		   && !TYPE_UNSIGNED (type))
	    fn = mathfn_built_in (s_intype, BUILT_IN_LFLOOR);
	  else if (outprec == TYPE_PRECISION (long_long_integer_type_node)
		   && !TYPE_UNSIGNED (type))
	    fn = mathfn_built_in (s_intype, BUILT_IN_LLFLOOR);
	  break;

	CASE_FLT_FN (BUILT_IN_ROUND):
	CASE_FLT_FN_FLOATN_NX (BUILT_IN_ROUND):
	  /* Only convert in ISO C99 mode and with -fno-math-errno.  */
	  if (!targetm.libc_has_function (function_c99_misc)
	      || flag_errno_math)
	    break;
	  if (outprec < TYPE_PRECISION (integer_type_node)
	      || (outprec == TYPE_PRECISION (integer_type_node)
		  && !TYPE_UNSIGNED (type)))
	    fn = mathfn_built_in (s_intype, BUILT_IN_IROUND);
	  else if (outprec == TYPE_PRECISION (long_integer_type_node)
		   && !TYPE_UNSIGNED (type))
	    fn = mathfn_built_in (s_intype, BUILT_IN_LROUND);
	  else if (outprec == TYPE_PRECISION (long_long_integer_type_node)
		   && !TYPE_UNSIGNED (type))
	    fn = mathfn_built_in (s_intype, BUILT_IN_LLROUND);
	  break;

	CASE_FLT_FN (BUILT_IN_NEARBYINT):
	CASE_FLT_FN_FLOATN_NX (BUILT_IN_NEARBYINT):
	  /* Only convert nearbyint* if we can ignore math exceptions.  */
	  if (flag_trapping_math)
	    break;
	  gcc_fallthrough ();
	CASE_FLT_FN (BUILT_IN_RINT):
	CASE_FLT_FN_FLOATN_NX (BUILT_IN_RINT):
	  /* Only convert in ISO C99 mode and with -fno-math-errno.  */
	  if (!targetm.libc_has_function (function_c99_misc)
	      || flag_errno_math)
	    break;
	  if (outprec < TYPE_PRECISION (integer_type_node)
	      || (outprec == TYPE_PRECISION (integer_type_node)
		  && !TYPE_UNSIGNED (type)))
	    fn = mathfn_built_in (s_intype, BUILT_IN_IRINT);
	  else if (outprec == TYPE_PRECISION (long_integer_type_node)
		   && !TYPE_UNSIGNED (type))
	    fn = mathfn_built_in (s_intype, BUILT_IN_LRINT);
	  else if (outprec == TYPE_PRECISION (long_long_integer_type_node)
		   && !TYPE_UNSIGNED (type))
	    fn = mathfn_built_in (s_intype, BUILT_IN_LLRINT);
	  break;

	CASE_FLT_FN (BUILT_IN_TRUNC):
	CASE_FLT_FN_FLOATN_NX (BUILT_IN_TRUNC):
	  if (call_expr_nargs (s_expr) != 1
	      || !SCALAR_FLOAT_TYPE_P (TREE_TYPE (CALL_EXPR_ARG (s_expr, 0))))
	    break;
	  return convert_to_integer_1 (type, CALL_EXPR_ARG (s_expr, 0),
				       dofold);

	default:
	  break;
	}

      if (fn
	  && call_expr_nargs (s_expr) == 1
	  && SCALAR_FLOAT_TYPE_P (TREE_TYPE (CALL_EXPR_ARG (s_expr, 0))))
	{
	  tree newexpr = build_call_expr (fn, 1, CALL_EXPR_ARG (s_expr, 0));
	  return convert_to_integer_1 (type, newexpr, dofold);
	}
    }

  /* Convert (int)logb(d) -> ilogb(d).  */
  if (optimize
      && flag_unsafe_math_optimizations
      && !flag_trapping_math && !flag_errno_math && flag_finite_math_only
      && integer_type_node
      && (outprec > TYPE_PRECISION (integer_type_node)
	  || (outprec == TYPE_PRECISION (integer_type_node)
	      && !TYPE_UNSIGNED (type))))
    {
      tree s_expr = strip_float_extensions (expr);
      tree s_intype = TREE_TYPE (s_expr);
      const enum built_in_function fcode = builtin_mathfn_code (s_expr);
      tree fn = 0;

      switch (fcode)
	{
	CASE_FLT_FN (BUILT_IN_LOGB):
	  fn = mathfn_built_in (s_intype, BUILT_IN_ILOGB);
	  break;

	default:
	  break;
	}

      if (fn
	  && call_expr_nargs (s_expr) == 1
	  && SCALAR_FLOAT_TYPE_P (TREE_TYPE (CALL_EXPR_ARG (s_expr, 0))))
        {
	  tree newexpr = build_call_expr (fn, 1, CALL_EXPR_ARG (s_expr, 0));
	  return convert_to_integer_1 (type, newexpr, dofold);
	}
    }

  switch (TREE_CODE (intype))
    {
    case POINTER_TYPE:
    case REFERENCE_TYPE:
      if (integer_zerop (expr)
	  && !TREE_OVERFLOW (tree_strip_any_location_wrapper (expr)))
	return build_int_cst (type, 0);

      /* Convert to an unsigned integer of the correct width first, and from
	 there widen/truncate to the required type.  Some targets support the
	 coexistence of multiple valid pointer sizes, so fetch the one we need
	 from the type.  */
      if (!dofold)
	return build1 (CONVERT_EXPR, type, expr);
      expr = fold_build1 (CONVERT_EXPR,
			  lang_hooks.types.type_for_size
			    (TYPE_PRECISION (intype), 0),
			  expr);
      return fold_convert (type, expr);

    case INTEGER_TYPE:
    case ENUMERAL_TYPE:
    case BOOLEAN_TYPE:
    case OFFSET_TYPE:
      /* If this is a logical operation, which just returns 0 or 1, we can
	 change the type of the expression.  */

      if (TREE_CODE_CLASS (ex_form) == tcc_comparison)
	{
	  expr = copy_node (expr);
	  TREE_TYPE (expr) = type;
	  return expr;
	}

      /* If we are widening the type, put in an explicit conversion.
	 Similarly if we are not changing the width.  After this, we know
	 we are truncating EXPR.  */

      else if (outprec >= inprec)
	{
	  enum tree_code code;

	  /* If the precision of the EXPR's type is K bits and the
	     destination mode has more bits, and the sign is changing,
	     it is not safe to use a NOP_EXPR.  For example, suppose
	     that EXPR's type is a 3-bit unsigned integer type, the
	     TYPE is a 3-bit signed integer type, and the machine mode
	     for the types is 8-bit QImode.  In that case, the
	     conversion necessitates an explicit sign-extension.  In
	     the signed-to-unsigned case the high-order bits have to
	     be cleared.  */
	  if (TYPE_UNSIGNED (type) != TYPE_UNSIGNED (TREE_TYPE (expr))
	      && !type_has_mode_precision_p (TREE_TYPE (expr)))
	    code = CONVERT_EXPR;
	  else
	    code = NOP_EXPR;

	  return maybe_fold_build1_loc (dofold, loc, code, type, expr);
	}

      /* If TYPE is an enumeral type or a type with a precision less
	 than the number of bits in its mode, do the conversion to the
	 type corresponding to its mode, then do a nop conversion
	 to TYPE.  */
      else if (TREE_CODE (type) == ENUMERAL_TYPE
	       || maybe_ne (outprec, GET_MODE_PRECISION (TYPE_MODE (type))))
	{
	  expr
	    = convert_to_integer_1 (lang_hooks.types.type_for_mode
				    (TYPE_MODE (type), TYPE_UNSIGNED (type)),
				    expr, dofold);
	  return maybe_fold_build1_loc (dofold, loc, NOP_EXPR, type, expr);
	}

      /* Here detect when we can distribute the truncation down past some
	 arithmetic.  For example, if adding two longs and converting to an
	 int, we can equally well convert both to ints and then add.
	 For the operations handled here, such truncation distribution
	 is always safe.
	 It is desirable in these cases:
	 1) when truncating down to full-word from a larger size
	 2) when truncating takes no work.
	 3) when at least one operand of the arithmetic has been extended
	 (as by C's default conversions).  In this case we need two conversions
	 if we do the arithmetic as already requested, so we might as well
	 truncate both and then combine.  Perhaps that way we need only one.

	 Note that in general we cannot do the arithmetic in a type
	 shorter than the desired result of conversion, even if the operands
	 are both extended from a shorter type, because they might overflow
	 if combined in that type.  The exceptions to this--the times when
	 two narrow values can be combined in their narrow type even to
	 make a wider result--are handled by "shorten" in build_binary_op.  */

      if (dofold)
	switch (ex_form)
	  {
	  case RSHIFT_EXPR:
	    /* We can pass truncation down through right shifting
	       when the shift count is a nonpositive constant.  */
	    if (TREE_CODE (TREE_OPERAND (expr, 1)) == INTEGER_CST
		&& tree_int_cst_sgn (TREE_OPERAND (expr, 1)) <= 0)
	      goto trunc1;
	    break;

	  case LSHIFT_EXPR:
	    /* We can pass truncation down through left shifting
	       when the shift count is a nonnegative constant and
	       the target type is unsigned.  */
	    if (TREE_CODE (TREE_OPERAND (expr, 1)) == INTEGER_CST
		&& tree_int_cst_sgn (TREE_OPERAND (expr, 1)) >= 0
		&& TYPE_UNSIGNED (type)
		&& TREE_CODE (TYPE_SIZE (type)) == INTEGER_CST)
	      {
		/* If shift count is less than the width of the truncated type,
		   really shift.  */
		if (tree_int_cst_lt (TREE_OPERAND (expr, 1), TYPE_SIZE (type)))
		  /* In this case, shifting is like multiplication.  */
		  goto trunc1;
		else
		  {
		    /* If it is >= that width, result is zero.
		       Handling this with trunc1 would give the wrong result:
		       (int) ((long long) a << 32) is well defined (as 0)
		       but (int) a << 32 is undefined and would get a
		       warning.  */

		    tree t = build_int_cst (type, 0);

		    /* If the original expression had side-effects, we must
		       preserve it.  */
		    if (TREE_SIDE_EFFECTS (expr))
		      return build2 (COMPOUND_EXPR, type, expr, t);
		    else
		      return t;
		  }
	      }
	    break;

	  case TRUNC_DIV_EXPR:
	    {
	      tree arg0 = get_unwidened (TREE_OPERAND (expr, 0), NULL_TREE);
	      tree arg1 = get_unwidened (TREE_OPERAND (expr, 1), NULL_TREE);

	      /* Don't distribute unless the output precision is at least as
		 big as the actual inputs and it has the same signedness.  */
	      if (outprec >= TYPE_PRECISION (TREE_TYPE (arg0))
		  && outprec >= TYPE_PRECISION (TREE_TYPE (arg1))
		  /* If signedness of arg0 and arg1 don't match,
		     we can't necessarily find a type to compare them in.  */
		  && (TYPE_UNSIGNED (TREE_TYPE (arg0))
		      == TYPE_UNSIGNED (TREE_TYPE (arg1)))
		  /* Do not change the sign of the division.  */
		  && (TYPE_UNSIGNED (TREE_TYPE (expr))
		      == TYPE_UNSIGNED (TREE_TYPE (arg0)))
		  /* Either require unsigned division or a division by
		     a constant that is not -1.  */
		  && (TYPE_UNSIGNED (TREE_TYPE (arg0))
		      || (TREE_CODE (arg1) == INTEGER_CST
			  && !integer_all_onesp (arg1))))
		{
		  tree tem = do_narrow (loc, ex_form, type, arg0, arg1,
					expr, inprec, outprec, dofold);
		  if (tem)
		    return tem;
		}
	      break;
	    }

	  case MAX_EXPR:
	  case MIN_EXPR:
	  case MULT_EXPR:
	    {
	      tree arg0 = get_unwidened (TREE_OPERAND (expr, 0), type);
	      tree arg1 = get_unwidened (TREE_OPERAND (expr, 1), type);

	      /* Don't distribute unless the output precision is at least as
		 big as the actual inputs.  Otherwise, the comparison of the
		 truncated values will be wrong.  */
	      if (outprec >= TYPE_PRECISION (TREE_TYPE (arg0))
		  && outprec >= TYPE_PRECISION (TREE_TYPE (arg1))
		  /* If signedness of arg0 and arg1 don't match,
		     we can't necessarily find a type to compare them in.  */
		  && (TYPE_UNSIGNED (TREE_TYPE (arg0))
		      == TYPE_UNSIGNED (TREE_TYPE (arg1))))
		goto trunc1;
	      break;
	    }

	  case PLUS_EXPR:
	  case MINUS_EXPR:
	  case BIT_AND_EXPR:
	  case BIT_IOR_EXPR:
	  case BIT_XOR_EXPR:
	  trunc1:
	    {
	      tree arg0 = get_unwidened (TREE_OPERAND (expr, 0), type);
	      tree arg1 = get_unwidened (TREE_OPERAND (expr, 1), type);

	      /* Do not try to narrow operands of pointer subtraction;
		 that will interfere with other folding.  */
	      if (ex_form == MINUS_EXPR
		  && CONVERT_EXPR_P (arg0)
		  && CONVERT_EXPR_P (arg1)
		  && POINTER_TYPE_P (TREE_TYPE (TREE_OPERAND (arg0, 0)))
		  && POINTER_TYPE_P (TREE_TYPE (TREE_OPERAND (arg1, 0))))
		break;

	      if (outprec >= BITS_PER_WORD
		  || targetm.truly_noop_truncation (outprec, inprec)
		  || inprec > TYPE_PRECISION (TREE_TYPE (arg0))
		  || inprec > TYPE_PRECISION (TREE_TYPE (arg1)))
		{
		  tree tem = do_narrow (loc, ex_form, type, arg0, arg1,
					expr, inprec, outprec, dofold);
		  if (tem)
		    return tem;
		}
	    }
	    break;

	  case NEGATE_EXPR:
	    /* Using unsigned arithmetic for signed types may hide overflow
	       bugs.  */
	    if (!TYPE_UNSIGNED (TREE_TYPE (TREE_OPERAND (expr, 0)))
		&& sanitize_flags_p (SANITIZE_SI_OVERFLOW))
	      break;
	    /* Fall through.  */
	  case BIT_NOT_EXPR:
	    /* This is not correct for ABS_EXPR,
	       since we must test the sign before truncation.  */
	    {
	      /* Do the arithmetic in type TYPEX,
		 then convert result to TYPE.  */
	      tree typex = type;

	      /* Can't do arithmetic in enumeral types
		 so use an integer type that will hold the values.  */
	      if (TREE_CODE (typex) == ENUMERAL_TYPE)
		typex
		  = lang_hooks.types.type_for_size (TYPE_PRECISION (typex),
						    TYPE_UNSIGNED (typex));

	      if (!TYPE_UNSIGNED (typex))
		typex = unsigned_type_for (typex);
	      return convert (type,
			      fold_build1 (ex_form, typex,
					   convert (typex,
						    TREE_OPERAND (expr, 0))));
	    }

	  CASE_CONVERT:
	    {
	      tree argtype = TREE_TYPE (TREE_OPERAND (expr, 0));
	      /* Don't introduce a "can't convert between vector values
		 of different size" error.  */
	      if (TREE_CODE (argtype) == VECTOR_TYPE
		  && maybe_ne (GET_MODE_SIZE (TYPE_MODE (argtype)),
			       GET_MODE_SIZE (TYPE_MODE (type))))
		break;
	    }
	    /* If truncating after truncating, might as well do all at once.
	       If truncating after extending, we may get rid of wasted work.  */
	    return convert (type, get_unwidened (TREE_OPERAND (expr, 0), type));

	  case COND_EXPR:
	    /* It is sometimes worthwhile to push the narrowing down through
	       the conditional and never loses.  A COND_EXPR may have a throw
	       as one operand, which then has void type.  Just leave void
	       operands as they are.  */
	    return
	      fold_build3 (COND_EXPR, type, TREE_OPERAND (expr, 0),
			   VOID_TYPE_P (TREE_TYPE (TREE_OPERAND (expr, 1)))
			   ? TREE_OPERAND (expr, 1)
			   : convert (type, TREE_OPERAND (expr, 1)),
			   VOID_TYPE_P (TREE_TYPE (TREE_OPERAND (expr, 2)))
			   ? TREE_OPERAND (expr, 2)
			   : convert (type, TREE_OPERAND (expr, 2)));

	  default:
	    break;
	  }

      /* When parsing long initializers, we might end up with a lot of casts.
	 Shortcut this.  */
      if (TREE_CODE (tree_strip_any_location_wrapper (expr)) == INTEGER_CST)
	return fold_convert (type, expr);
      return build1 (CONVERT_EXPR, type, expr);

    case REAL_TYPE:
      if (sanitize_flags_p (SANITIZE_FLOAT_CAST)
	  && current_function_decl != NULL_TREE)
	{
	  expr = save_expr (expr);
	  tree check = ubsan_instrument_float_cast (loc, type, expr);
	  expr = build1 (FIX_TRUNC_EXPR, type, expr);
	  if (check == NULL_TREE)
	    return expr;
	  return maybe_fold_build2_loc (dofold, loc, COMPOUND_EXPR,
					TREE_TYPE (expr), check, expr);
	}
      else
	return build1 (FIX_TRUNC_EXPR, type, expr);

    case FIXED_POINT_TYPE:
      return build1 (FIXED_CONVERT_EXPR, type, expr);

    case COMPLEX_TYPE:
      expr = maybe_fold_build1_loc (dofold, loc, REALPART_EXPR,
				    TREE_TYPE (TREE_TYPE (expr)), expr);
      return convert (type, expr);

    case VECTOR_TYPE:
      if (!tree_int_cst_equal (TYPE_SIZE (type), TYPE_SIZE (TREE_TYPE (expr))))
	{
	  error ("cannot convert a vector of type %qT"
		 " to type %qT which has different size",
		 TREE_TYPE (expr), type);
	  return error_mark_node;
	}
      return build1 (VIEW_CONVERT_EXPR, type, expr);

    default:
      error ("aggregate value used where an integer was expected");
      return convert (type, integer_zero_node);
    }
}

/* Convert EXPR to some integer (or enum) type TYPE.

   EXPR must be pointer, integer, discrete (enum, char, or bool), float,
   fixed-point or vector; in other cases error is called.

   The result of this is always supposed to be a newly created tree node
   not in use in any existing structure.  */

tree
convert_to_integer (tree type, tree expr)
{
  return convert_to_integer_1 (type, expr, true);
}

/* A wrapper around convert_to_complex_1 that only folds the
   expression if DOFOLD, or if it is CONSTANT_CLASS_OR_WRAPPER_P.  */

tree
convert_to_integer_maybe_fold (tree type, tree expr, bool dofold)
{
  tree result
    = convert_to_integer_1 (type, expr,
			    dofold || CONSTANT_CLASS_OR_WRAPPER_P (expr));
  return preserve_any_location_wrapper (result, expr);
}

/* Convert EXPR to the complex type TYPE in the usual ways.  If FOLD_P is
   true, try to fold the expression.  */

static tree
convert_to_complex_1 (tree type, tree expr, bool fold_p)
{
  location_t loc = EXPR_LOCATION (expr);
  tree subtype = TREE_TYPE (type);

  switch (TREE_CODE (TREE_TYPE (expr)))
    {
    case REAL_TYPE:
    case FIXED_POINT_TYPE:
    case INTEGER_TYPE:
    case ENUMERAL_TYPE:
    case BOOLEAN_TYPE:
      return build2 (COMPLEX_EXPR, type, convert (subtype, expr),
		     convert (subtype, integer_zero_node));

    case COMPLEX_TYPE:
      {
	tree elt_type = TREE_TYPE (TREE_TYPE (expr));

	if (TYPE_MAIN_VARIANT (elt_type) == TYPE_MAIN_VARIANT (subtype))
	  return expr;
	else if (TREE_CODE (expr) == COMPOUND_EXPR)
	  {
	    tree t = convert_to_complex_1 (type, TREE_OPERAND (expr, 1),
					   fold_p);
	    if (t == TREE_OPERAND (expr, 1))
	      return expr;
	    return build2_loc (EXPR_LOCATION (expr), COMPOUND_EXPR,
			       TREE_TYPE (t), TREE_OPERAND (expr, 0), t);
	  }
	else if (TREE_CODE (expr) == COMPLEX_EXPR)
	  return maybe_fold_build2_loc (fold_p, loc, COMPLEX_EXPR, type,
					convert (subtype,
						 TREE_OPERAND (expr, 0)),
					convert (subtype,
						 TREE_OPERAND (expr, 1)));
	else
	  {
	    expr = save_expr (expr);
	    tree realp = maybe_fold_build1_loc (fold_p, loc, REALPART_EXPR,
						TREE_TYPE (TREE_TYPE (expr)),
						expr);
	    tree imagp = maybe_fold_build1_loc (fold_p, loc, IMAGPART_EXPR,
						TREE_TYPE (TREE_TYPE (expr)),
						expr);
	    return maybe_fold_build2_loc (fold_p, loc, COMPLEX_EXPR, type,
					  convert (subtype, realp),
					  convert (subtype, imagp));
	  }
      }

    case POINTER_TYPE:
    case REFERENCE_TYPE:
      error ("pointer value used where a complex was expected");
      return convert_to_complex_1 (type, integer_zero_node, fold_p);

    default:
      error ("aggregate value used where a complex was expected");
      return convert_to_complex_1 (type, integer_zero_node, fold_p);
    }
}

/* A wrapper around convert_to_complex_1 that always folds the
   expression.  */

tree
convert_to_complex (tree type, tree expr)
{
  return convert_to_complex_1 (type, expr, true);
}

/* A wrapper around convert_to_complex_1 that only folds the
   expression if DOFOLD, or if it is CONSTANT_CLASS_OR_WRAPPER_P.  */

tree
convert_to_complex_maybe_fold (tree type, tree expr, bool dofold)
{
  tree result
    = convert_to_complex_1 (type, expr,
			    dofold || CONSTANT_CLASS_OR_WRAPPER_P (expr));
  return preserve_any_location_wrapper (result, expr);
}

/* Convert EXPR to the vector type TYPE in the usual ways.  */

tree
convert_to_vector (tree type, tree expr)
{
  switch (TREE_CODE (TREE_TYPE (expr)))
    {
    case INTEGER_TYPE:
    case VECTOR_TYPE:
      if (!tree_int_cst_equal (TYPE_SIZE (type), TYPE_SIZE (TREE_TYPE (expr))))
	{
	  error ("cannot convert a value of type %qT"
		 " to vector type %qT which has different size",
		 TREE_TYPE (expr), type);
	  return error_mark_node;
	}
      return build1 (VIEW_CONVERT_EXPR, type, expr);

    default:
      error ("cannot convert value to a vector");
      return error_mark_node;
    }
}

/* Convert EXPR to some fixed-point type TYPE.

   EXPR must be fixed-point, float, integer, or enumeral;
   in other cases error is called.  */

tree
convert_to_fixed (tree type, tree expr)
{
  if (integer_zerop (expr))
    {
      tree fixed_zero_node = build_fixed (type, FCONST0 (TYPE_MODE (type)));
      return fixed_zero_node;
    }
  else if (integer_onep (expr) && ALL_SCALAR_ACCUM_MODE_P (TYPE_MODE (type)))
    {
      tree fixed_one_node = build_fixed (type, FCONST1 (TYPE_MODE (type)));
      return fixed_one_node;
    }

  switch (TREE_CODE (TREE_TYPE (expr)))
    {
    case FIXED_POINT_TYPE:
    case INTEGER_TYPE:
    case ENUMERAL_TYPE:
    case BOOLEAN_TYPE:
    case REAL_TYPE:
      return build1 (FIXED_CONVERT_EXPR, type, expr);

    case COMPLEX_TYPE:
      return convert (type,
		      fold_build1 (REALPART_EXPR,
				   TREE_TYPE (TREE_TYPE (expr)), expr));

    default:
      error ("aggregate value used where a fixed-point was expected");
      return error_mark_node;
    }
}

#if CHECKING_P

namespace selftest {

/* Selftests for conversions.  */

static void
test_convert_to_integer_maybe_fold (tree orig_type, tree new_type)
{
  /* Calling convert_to_integer_maybe_fold on an INTEGER_CST.  */

  tree orig_cst = build_int_cst (orig_type, 42);

  /* Verify that convert_to_integer_maybe_fold on a constant returns a new
     constant of the new type, unless the types are the same, in which
     case verify it's a no-op.  */
  {
    tree result = convert_to_integer_maybe_fold (new_type,
						 orig_cst, false);
    if (orig_type != new_type)
      {
	ASSERT_EQ (TREE_TYPE (result), new_type);
	ASSERT_EQ (TREE_CODE (result), INTEGER_CST);
      }
    else
      ASSERT_EQ (result, orig_cst);
  }

  /* Calling convert_to_integer_maybe_fold on a location wrapper around
     an INTEGER_CST.

     Verify that convert_to_integer_maybe_fold on a location wrapper
     around a constant returns a new location wrapper around an equivalent
     constant, both of the new type, unless the types are the same,
     in which case the original wrapper should be returned.   */
  {
    const location_t loc = BUILTINS_LOCATION;
    tree wrapped_orig_cst = maybe_wrap_with_location (orig_cst, loc);
    tree result
      = convert_to_integer_maybe_fold (new_type, wrapped_orig_cst, false);
    ASSERT_EQ (TREE_TYPE (result), new_type);
    ASSERT_EQ (EXPR_LOCATION (result), loc);
    ASSERT_TRUE (location_wrapper_p (result));
    ASSERT_EQ (TREE_TYPE (TREE_OPERAND (result, 0)), new_type);
    ASSERT_EQ (TREE_CODE (TREE_OPERAND (result, 0)), INTEGER_CST);

    if (orig_type == new_type)
      ASSERT_EQ (result, wrapped_orig_cst);
  }
}

/* Verify that convert_to_integer_maybe_fold preserves locations.  */

static void
test_convert_to_integer_maybe_fold ()
{
  /* char -> long.  */
  test_convert_to_integer_maybe_fold (char_type_node, long_integer_type_node);

  /* char -> char.  */
  test_convert_to_integer_maybe_fold (char_type_node, char_type_node);

  /* long -> char.  */
  test_convert_to_integer_maybe_fold (char_type_node, long_integer_type_node);

  /* long -> long.  */
  test_convert_to_integer_maybe_fold (long_integer_type_node,
				      long_integer_type_node);
}

/* Run all of the selftests within this file.  */

void
convert_c_tests ()
{
  test_convert_to_integer_maybe_fold ();
}

} // namespace selftest

#endif /* CHECKING_P */
