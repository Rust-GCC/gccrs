/* Verify that overloaded built-ins for vec_perm with pixel
   inputs produce the right code.  */

/* { dg-do compile } */
/* { dg-options "-maltivec -O2" } */
/* { dg-require-effective-target powerpc_altivec } */

#include <altivec.h>

vector pixel
testpx (vector pixel px2, vector pixel px3, vector unsigned char vuc)
{
  return vec_perm (px2, px3, vuc);
}

/* { dg-final { scan-assembler-times {\m(?:v|xx)permr?\M} 1 } } */
