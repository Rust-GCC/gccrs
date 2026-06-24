/* { dg-do compile } */
/* { dg-options "-mdejagnu-cpu=future -O2 -mno-vsx" } */
/* { dg-require-effective-target powerpc_future_compile_ok } */

#include <altivec.h>

typedef vector signed short   vss;
typedef vector unsigned short vus;

__attribute__((noinline))
vss short_mulhi (vss x, vss y)
{
  return vec_mulh (x, y);  /* { dg-error "'__builtin_vsx_xvmulhsh' requires the '-mcpu=future' and '-mvsx' options" } */
}

__attribute__((noinline))
vus ushort_mulhi (vus x, vus y)
{
  return vec_mulh (x, y);  /* { dg-error "'__builtin_vsx_xvmulhuh' requires the '-mcpu=future' and '-mvsx' options" } */
}
