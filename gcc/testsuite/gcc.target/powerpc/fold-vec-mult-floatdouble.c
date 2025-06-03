/* Verify that overloaded built-ins for vec_mul with float and
   double inputs for VSX produce the right results.  */

/* { dg-do compile } */
/* { dg-options "-mvsx" } */
/* { dg-require-effective-target powerpc_vsx } */

#include <altivec.h>

vector float
test1 (vector float x, vector float y)
{
  return vec_mul (x, y);
}

vector double
test2 (vector double x, vector double y)
{
  return vec_mul (x, y);
}

/* { dg-final { scan-assembler-times "\[ \t\]xvmulsp" 1 } } */
/* { dg-final { scan-assembler-times "\[ \t\]xvmuldp" 1 } } */
