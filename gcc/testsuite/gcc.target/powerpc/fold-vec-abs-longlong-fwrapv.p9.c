/* Verify that overloaded built-ins for vec_abs with long long
   inputs produce the right results.  */

/* { dg-do compile } */
/* { dg-options "-O2 -mdejagnu-cpu=power9 -mvsx" } */
/* { dg-require-effective-target powerpc_vsx } */

#include <altivec.h>

vector signed long long
test3 (vector signed long long x)
{
  return vec_abs (x);
}

/* { dg-final { scan-assembler-times "vnegd" 1 } } */
/* { dg-final { scan-assembler-times "vmaxsd" 1 } } */
