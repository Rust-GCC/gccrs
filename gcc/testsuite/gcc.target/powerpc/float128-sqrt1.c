/* { dg-do compile { target lp64 } } */
/* { dg-options "-mvsx -O2" } */
/* { dg-additional-options "-mdejagnu-cpu=power9" { target { ! has_arch_pwr9 } } } */
/* { dg-require-effective-target powerpc_vsx } */
/* { dg-require-effective-target float128 } */

__float128
xsqrt (__float128 a)
{
  return __builtin_sqrtf128 (a);
}

/* { dg-final { scan-assembler "xssqrtqp"  } } */
