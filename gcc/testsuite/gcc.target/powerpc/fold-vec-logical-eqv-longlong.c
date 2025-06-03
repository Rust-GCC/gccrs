/* Verify that overloaded built-ins for vec_eqv with long long
   inputs produce the right results.  */

/* { dg-do compile } */
/* { dg-options "-mvsx -O2" } */
/* { dg-additional-options "-mdejagnu-cpu=power8" { target { ! has_arch_pwr8 } } } */
/* { dg-require-effective-target powerpc_vsx } */

#include <altivec.h>

vector bool long long
test1 (vector bool long long x, vector bool long long y)
{
  return vec_eqv (x, y);
}

vector signed long long
test3 (vector signed long long x, vector signed long long y)
{
  return vec_eqv (x, y);
}

vector unsigned long long
test6 (vector unsigned long long x, vector unsigned long long y)
{
  return vec_eqv (x, y);
}

/* { dg-final { scan-assembler-times "xxleqv" 3 } } */
