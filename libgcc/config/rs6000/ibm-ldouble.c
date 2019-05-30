/* 128-bit long double support routines for Darwin.
   Copyright (C) 1993-2019 Free Software Foundation, Inc.

This file is part of GCC.

GCC is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free
Software Foundation; either version 3, or (at your option) any later
version.

GCC is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or
FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
for more details.

Under Section 7 of GPL version 3, you are granted additional
permissions described in the GCC Runtime Library Exception, version
3.1, as published by the Free Software Foundation.

You should have received a copy of the GNU General Public License and
a copy of the GCC Runtime Library Exception along with this program;
see the files COPYING3 and COPYING.RUNTIME respectively.  If not, see
<http://www.gnu.org/licenses/>.  */


/* Implementations of floating-point long double basic arithmetic
   functions called by the IBM C compiler when generating code for
   PowerPC platforms.  In particular, the following functions are
   implemented: __gcc_qadd, __gcc_qsub, __gcc_qmul, and __gcc_qdiv.
   Double-double algorithms are based on the paper "Doubled-Precision
   IEEE Standard 754 Floating-Point Arithmetic" by W. Kahan, February 26,
   1987.  An alternative published reference is "Software for
   Doubled-Precision Floating-Point Computations", by Seppo Linnainmaa,
   ACM TOMS vol 7 no 3, September 1981, pages 272-283.  */

/* Each long double is made up of two IEEE doubles.  The value of the
   long double is the sum of the values of the two parts.  The most
   significant part is required to be the value of the long double
   rounded to the nearest double, as specified by IEEE.  For Inf
   values, the least significant part is required to be one of +0.0 or
   -0.0.  No other requirements are made; so, for example, 1.0 may be
   represented as (1.0, +0.0) or (1.0, -0.0), and the low part of a
   NaN is don't-care.

   This code currently assumes the most significant double is in
   the lower numbered register or lower addressed memory.  */

#if (defined (__MACH__) || defined (__powerpc__) || defined (_AIX)) \
  && !defined (__rtems__) \
  && (defined (__LONG_DOUBLE_128__) || defined (__FLOAT128_TYPE__))

#define fabs(x) __builtin_fabs(x)
#define isless(x, y) __builtin_isless (x, y)
#define inf() __builtin_inf()

#define unlikely(x) __builtin_expect ((x), 0)

#define nonfinite(a) unlikely (! isless (fabs (a), inf ()))

/* If we have __float128/_Float128, use __ibm128 instead of long double.  On
   other systems, use long double, because __ibm128 might not have been
   created.  */
#ifdef __FLOAT128__
#define IBM128_TYPE __ibm128
#else
#define IBM128_TYPE long double
#endif

/* Define ALIASNAME as a strong alias for NAME.  */
# define strong_alias(name, aliasname) _strong_alias(name, aliasname)
# define _strong_alias(name, aliasname) \
  extern __typeof (name) aliasname __attribute__ ((alias (#name)));

/* All these routines actually take two long doubles as parameters,
   but GCC currently generates poor code when a union is used to turn
   a long double into a pair of doubles.  */

IBM128_TYPE __gcc_qadd (double, double, double, double);
IBM128_TYPE __gcc_qsub (double, double, double, double);
IBM128_TYPE __gcc_qmul (double, double, double, double);
IBM128_TYPE __gcc_qdiv (double, double, double, double);

#if defined __ELF__ && defined SHARED \
    && (defined __powerpc64__ || !(defined __linux__ || defined __gnu_hurd__))
/* Provide definitions of the old symbol names to satisfy apps and
   shared libs built against an older libgcc.  To access the _xlq
   symbols an explicit version reference is needed, so these won't
   satisfy an unadorned reference like _xlqadd.  If dot symbols are
   not needed, the assembler will remove the aliases from the symbol
   table.  */
__asm__ (".symver __gcc_qadd,_xlqadd@GCC_3.4\n\t"
	 ".symver __gcc_qsub,_xlqsub@GCC_3.4\n\t"
	 ".symver __gcc_qmul,_xlqmul@GCC_3.4\n\t"
	 ".symver __gcc_qdiv,_xlqdiv@GCC_3.4\n\t"
	 ".symver .__gcc_qadd,._xlqadd@GCC_3.4\n\t"
	 ".symver .__gcc_qsub,._xlqsub@GCC_3.4\n\t"
	 ".symver .__gcc_qmul,._xlqmul@GCC_3.4\n\t"
	 ".symver .__gcc_qdiv,._xlqdiv@GCC_3.4");
#endif

/* Combine two 'double' values into one 'IBM128_TYPE' and return the result.  */
static inline IBM128_TYPE
pack_ldouble (double dh, double dl)
{
#if defined (__LONG_DOUBLE_128__) && defined (__LONG_DOUBLE_IBM128__)	\
    && !(defined (_SOFT_FLOAT) || defined (__NO_FPRS__))
  return __builtin_pack_longdouble (dh, dl);
#else
  union
  {
    IBM128_TYPE ldval;
    double dval[2];
  } x;
  x.dval[0] = dh;
  x.dval[1] = dl;
  return x.ldval;
#endif
}

/* Add two 'IBM128_TYPE' values and return the result.	*/
IBM128_TYPE
__gcc_qadd (double a, double aa, double c, double cc)
{
  double xh, xl, z, q, zz;

  z = a + c;

  if (nonfinite (z))
    {
      if (fabs (z) != inf())
	return z;
      z = cc + aa + c + a;
      if (nonfinite (z))
	return z;
      xh = z;  /* Will always be DBL_MAX.  */
      zz = aa + cc;
      if (fabs(a) > fabs(c))
	xl = a - z + c + zz;
      else
	xl = c - z + a + zz;
    }
  else
    {
      q = a - z;
      zz = q + c + (a - (q + z)) + aa + cc;

      /* Keep -0 result.  */
      if (zz == 0.0)
	return z;

      xh = z + zz;
      if (nonfinite (xh))
	return xh;

      xl = z - xh + zz;
    }
  return pack_ldouble (xh, xl);
}

IBM128_TYPE
__gcc_qsub (double a, double b, double c, double d)
{
  return __gcc_qadd (a, b, -c, -d);
}

#ifdef __NO_FPRS__
static double fmsub (double, double, double);
#endif

IBM128_TYPE
__gcc_qmul (double a, double b, double c, double d)
{
  double xh, xl, t, tau, u, v, w;
  
  t = a * c;			/* Highest order double term.  */

  if (unlikely (t == 0)		/* Preserve -0.  */
      || nonfinite (t))
    return t;

  /* Sum terms of two highest orders. */
  
  /* Use fused multiply-add to get low part of a * c.  */
#ifndef __NO_FPRS__
  asm ("fmsub %0,%1,%2,%3" : "=f"(tau) : "f"(a), "f"(c), "f"(t));
#else
  tau = fmsub (a, c, t);
#endif
  v = a*d;
  w = b*c;
  tau += v + w;	    /* Add in other second-order terms.	 */
  u = t + tau;

  /* Construct IBM128_TYPE result.  */
  if (nonfinite (u))
    return u;
  xh = u;
  xl = (t - u) + tau;
  return pack_ldouble (xh, xl);
}

IBM128_TYPE
__gcc_qdiv (double a, double b, double c, double d)
{
  double xh, xl, s, sigma, t, tau, u, v, w;
  
  t = a / c;                    /* highest order double term */
  
  if (unlikely (t == 0)		/* Preserve -0.  */
      || nonfinite (t))
    return t;

  /* Finite nonzero result requires corrections to the highest order
     term.  These corrections require the low part of c * t to be
     exactly represented in double.  */
  if (fabs (a) <= 0x1p-969)
    {
      a *= 0x1p106;
      b *= 0x1p106;
      c *= 0x1p106;
      d *= 0x1p106;
    }

  s = c * t;                    /* (s,sigma) = c*t exactly.  */
  w = -(-b + d * t);	/* Written to get fnmsub for speed, but not
			   numerically necessary.  */
  
  /* Use fused multiply-add to get low part of c * t.	 */
#ifndef __NO_FPRS__
  asm ("fmsub %0,%1,%2,%3" : "=f"(sigma) : "f"(c), "f"(t), "f"(s));
#else
  sigma = fmsub (c, t, s);
#endif
  v = a - s;
  
  tau = ((v-sigma)+w)/c;   /* Correction to t.  */
  u = t + tau;

  /* Construct IBM128_TYPE result.  */
  if (nonfinite (u))
    return u;
  xh = u;
  xl = (t - u) + tau;
  return pack_ldouble (xh, xl);
}

#if defined (_SOFT_DOUBLE) && defined (__LONG_DOUBLE_128__)

IBM128_TYPE __gcc_qneg (double, double);
int __gcc_qeq (double, double, double, double);
int __gcc_qne (double, double, double, double);
int __gcc_qge (double, double, double, double);
int __gcc_qle (double, double, double, double);
IBM128_TYPE __gcc_stoq (float);
IBM128_TYPE __gcc_dtoq (double);
float __gcc_qtos (double, double);
double __gcc_qtod (double, double);
int __gcc_qtoi (double, double);
unsigned int __gcc_qtou (double, double);
IBM128_TYPE __gcc_itoq (int);
IBM128_TYPE __gcc_utoq (unsigned int);

extern int __eqdf2 (double, double);
extern int __ledf2 (double, double);
extern int __gedf2 (double, double);

/* Negate 'IBM128_TYPE' value and return the result.	*/
IBM128_TYPE
__gcc_qneg (double a, double aa)
{
  return pack_ldouble (-a, -aa);
}

/* Compare two 'IBM128_TYPE' values for equality.  */
int
__gcc_qeq (double a, double aa, double c, double cc)
{
  if (__eqdf2 (a, c) == 0)
    return __eqdf2 (aa, cc);
  return 1;
}

strong_alias (__gcc_qeq, __gcc_qne);

/* Compare two 'IBM128_TYPE' values for less than or equal.  */
int
__gcc_qle (double a, double aa, double c, double cc)
{
  if (__eqdf2 (a, c) == 0)
    return __ledf2 (aa, cc);
  return __ledf2 (a, c);
}

strong_alias (__gcc_qle, __gcc_qlt);

/* Compare two 'IBM128_TYPE' values for greater than or equal.  */
int
__gcc_qge (double a, double aa, double c, double cc)
{
  if (__eqdf2 (a, c) == 0)
    return __gedf2 (aa, cc);
  return __gedf2 (a, c);
}

strong_alias (__gcc_qge, __gcc_qgt);

/* Convert single to IBM128_TYPE.  */
IBM128_TYPE
__gcc_stoq (float a)
{
  return pack_ldouble ((double) a, 0.0);
}

/* Convert double to IBM128_TYPE.  */
IBM128_TYPE
__gcc_dtoq (double a)
{
  return pack_ldouble (a, 0.0);
}

/* Convert IBM128_TYPE to single.  */
float
__gcc_qtos (double a, double aa __attribute__ ((__unused__)))
{
  return (float) a;
}

/* Convert IBM128_TYPE to double.  */
double
__gcc_qtod (double a, double aa __attribute__ ((__unused__)))
{
  return a;
}

/* Convert IBM128_TYPE to int.  */
int
__gcc_qtoi (double a, double aa)
{
  double z = a + aa;
  return (int) z;
}

/* Convert IBM128_TYPE to unsigned int.  */
unsigned int
__gcc_qtou (double a, double aa)
{
  double z = a + aa;
  return (unsigned int) z;
}

/* Convert int to IBM128_TYPE.  */
IBM128_TYPE
__gcc_itoq (int a)
{
  return __gcc_dtoq ((double) a);
}

/* Convert unsigned int to IBM128_TYPE.  */
IBM128_TYPE
__gcc_utoq (unsigned int a)
{
  return __gcc_dtoq ((double) a);
}

#endif

#ifdef __NO_FPRS__

int __gcc_qunord (double, double, double, double);

extern int __eqdf2 (double, double);
extern int __unorddf2 (double, double);

/* Compare two 'IBM128_TYPE' values for unordered.  */
int
__gcc_qunord (double a, double aa, double c, double cc)
{
  if (__eqdf2 (a, c) == 0)
    return __unorddf2 (aa, cc);
  return __unorddf2 (a, c);
}

#include "soft-fp/soft-fp.h"
#include "soft-fp/double.h"
#include "soft-fp/quad.h"

/* Compute floating point multiply-subtract with higher (quad) precision.  */
static double
fmsub (double a, double b, double c)
{
    FP_DECL_EX;
    FP_DECL_D(A);
    FP_DECL_D(B);
    FP_DECL_D(C);
    FP_DECL_Q(X);
    FP_DECL_Q(Y);
    FP_DECL_Q(Z);
    FP_DECL_Q(U);
    FP_DECL_Q(V);
    FP_DECL_D(R);
    double r;
    IBM128_TYPE u, x, y, z;

    FP_INIT_ROUNDMODE;
    FP_UNPACK_RAW_D (A, a);
    FP_UNPACK_RAW_D (B, b);
    FP_UNPACK_RAW_D (C, c);

    /* Extend double to quad.  */
#if _FP_W_TYPE_SIZE < 64
    FP_EXTEND(Q,D,4,2,X,A);
    FP_EXTEND(Q,D,4,2,Y,B);
    FP_EXTEND(Q,D,4,2,Z,C);
#else
    FP_EXTEND(Q,D,2,1,X,A);
    FP_EXTEND(Q,D,2,1,Y,B);
    FP_EXTEND(Q,D,2,1,Z,C);
#endif
    FP_PACK_RAW_Q(x,X);
    FP_PACK_RAW_Q(y,Y);
    FP_PACK_RAW_Q(z,Z);
    FP_HANDLE_EXCEPTIONS;

    /* Multiply.  */
    FP_INIT_ROUNDMODE;
    FP_UNPACK_Q(X,x);
    FP_UNPACK_Q(Y,y);
    FP_MUL_Q(U,X,Y);
    FP_PACK_Q(u,U);
    FP_HANDLE_EXCEPTIONS;

    /* Subtract.  */
    FP_INIT_ROUNDMODE;
    FP_UNPACK_SEMIRAW_Q(U,u);
    FP_UNPACK_SEMIRAW_Q(Z,z);
    FP_SUB_Q(V,U,Z);

    /* Truncate quad to double.  */
#if _FP_W_TYPE_SIZE < 64
    V_f[3] &= 0x0007ffff;
    FP_TRUNC(D,Q,2,4,R,V);
#else
    V_f1 &= 0x0007ffffffffffffL;
    FP_TRUNC(D,Q,1,2,R,V);
#endif
    FP_PACK_SEMIRAW_D(r,R);
    FP_HANDLE_EXCEPTIONS;

    return r;
}

#endif

#endif
