/* { dg-do compile } */
/* { dg-options "-mdejagnu-cpu=power6 -maltivec -O2" } */
/* { dg-require-effective-target powerpc_altivec } */
/* { dg-require-effective-target lp64 } */

#include <altivec.h>

#ifdef _ARCH_PWR6
vector int
test1 (vector int a, vector int b)
{
  return vec_add (a, b);
}
#else
#error failed on default power6 pragma target
#endif

#pragma GCC target ("cpu=power7")
#undef _ALTIVEC_H
#undef _RS6000_VECDEFINES_H
#include <altivec.h>
#ifdef _ARCH_PWR7
vector signed int
test2 (vector signed int a, vector signed int b)
{
  return vec_sldw (a, b, 3);
}
#else
#error failed to set power7 pragma target
#endif

#pragma GCC target ("cpu=power8")
#undef _ALTIVEC_H
#undef _RS6000_VECDEFINES_H
#include <altivec.h>
#ifdef _ARCH_PWR8
vector int
test3 (vector int a, vector int b)
{
  return vec_mergee (a, b);
}

typedef __attribute__((altivec(vector__))) long vec_t;
int
test3b (vec_t a, vec_t b)
{
  return __builtin_vec_vcmpeq_p (2, a, b);
}
#else
#error failed to set power8 pragma target.
#endif

#pragma GCC target ("cpu=power9,power9-vector")
#undef _ALTIVEC_H
#undef _RS6000_VECDEFINES_H
#include <altivec.h>
#ifdef _ARCH_PWR9
vector bool int
test4 (vector signed int a, vector signed int b)
{
  return vec_cmpnez (a, b);
}
#else
#error Failed to set cpu=power9 pragma target.
#endif

