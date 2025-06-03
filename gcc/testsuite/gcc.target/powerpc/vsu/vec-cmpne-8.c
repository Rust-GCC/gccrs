/* { dg-do compile { target { powerpc*-*-* } } } */
/* { dg-options "-mdejagnu-cpu=power9 -mvsx" } */
/* { dg-require-effective-target powerpc_vsx } */

#include <altivec.h>

vector bool int
fetch_data (vector float *arg1_p, vector float *arg2_p)
{
  vector float arg_1 = *arg1_p;
  vector float arg_2 = *arg2_p;

  return vec_cmpne (arg_1, arg_2);
}

/* { dg-final { scan-assembler "xvcmpeqsp" } } */
