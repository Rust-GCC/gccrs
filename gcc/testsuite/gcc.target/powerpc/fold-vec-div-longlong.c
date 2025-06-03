/* Verify that overloaded built-ins for vec_div with long long
   inputs produce the right results.  */

/* { dg-do compile } */
/* { dg-options "-mvsx -O2" } */
/* { dg-require-effective-target powerpc_vsx } */
/* { dg-require-effective-target lp64 } */

#include <altivec.h>

vector signed long long
test3 (vector signed long long x, vector signed long long y)
{
  return vec_div (x, y);
}

vector unsigned long long
test6 (vector unsigned long long x, vector unsigned long long y)
{
  return vec_div (x, y);
}

/* { dg-final { scan-assembler-times {\mdivd\M}   2 { target { ! has_arch_pwr10 } } } } */
/* { dg-final { scan-assembler-times {\mdivdu\M}  2 { target { ! has_arch_pwr10 } } } } */
/* { dg-final { scan-assembler-times {\mvdivsd\M} 1 { target {   has_arch_pwr10 } } } } */
/* { dg-final { scan-assembler-times {\mvdivud\M} 1 { target {   has_arch_pwr10 } } } } */
