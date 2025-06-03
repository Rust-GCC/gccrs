/* { dg-do compile { target { powerpc*-*-* } } } */
/* { dg-options "-mdejagnu-cpu=power9 -mvsx" } */
/* { dg-require-effective-target powerpc_vsx } */

#include <altivec.h>

vector bool short
fetch_data (vector signed short *arg1_p, vector signed short *arg2_p)
{
  vector signed short arg_1 = *arg1_p;
  vector signed short arg_2 = *arg2_p;

  return vec_cmpnez (arg_1, arg_2);
}

/* { dg-final { scan-assembler "vcmpnezh" } } */
