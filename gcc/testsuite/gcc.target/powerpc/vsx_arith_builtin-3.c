/* { dg-do compile } */
/* { dg-options "-mdejagnu-cpu=future -O2 -mno-vsx" } */
/* { dg-require-effective-target powerpc_future_compile_ok } */

/* The -mno-vsx option turns off generation of VSX instructions (such as
   xvadduwm) in favor of the traditional Altivec instructions (such as
   vadduwm).  */

#include <altivec.h>

typedef vector signed int   vsi;
typedef vector signed short vss;
typedef vector unsigned int   vui;

vsi int_add (vsi x, vsi y)
{
  return vec_add (x, y);            /* vadduwm */
}

vsi int_sub (vsi x, vsi y)
{
  return vec_sub (x, y);            /* vsubuwm */
}

vsi int_mulhi (vsi x, vsi y)
{
  return vec_mulh (x, y);           /* vmulhsw */
}

vui uint_mulh (vui x, vui y)
{
  return vec_mulh (x, y);           /* vmulhuw */
}

vss short_add (vss x, vss y)
{
  return vec_add (x, y);            /* vadduhm */
}

vss short_sub (vss x, vss y)
{
  return vec_sub (x, y);            /* vsubuhm */
}

vss short_mul (vss x, vss y)
{
  return vec_mul (x, y);            /* vmladduhm */
}

/* { dg-final { scan-assembler-times "vadduwm" 1 } } */
/* { dg-final { scan-assembler-times "vsubuwm" 1 } } */
/* { dg-final { scan-assembler-times "vmulhsw" 1 } } */
/* { dg-final { scan-assembler-times "vmulhuw" 1 } } */
/* { dg-final { scan-assembler-times "vadduhm" 1 } } */
/* { dg-final { scan-assembler-times "vsubuhm" 1 } } */
/* { dg-final { scan-assembler-times "vmladduhm" 1 } } */
