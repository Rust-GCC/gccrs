/* { dg-do compile { target { powerpc*-*-* } } } */
/* { dg-skip-if "" { powerpc*-*-darwin* } } */
/* { dg-options "-mdejagnu-cpu=power8 -mvsx -O2" } */
/* { dg-require-effective-target powerpc_vsx } */

#include <altivec.h>

typedef vector int		v_sign;
typedef vector unsigned int	v_uns;

v_sign even_sign (v_sign a, v_sign b)
{
  return vec_vmrgew (a, b);
}

v_uns even_uns (v_uns a, v_uns b)
{
  return vec_vmrgew (a, b);
}

v_sign odd_sign (v_sign a, v_sign b)
{
  return vec_vmrgow (a, b);
}

v_uns odd_uns (v_uns a, v_uns b)
{
  return vec_vmrgow (a, b);
}

/* { dg-final { scan-assembler-times "vmrgew" 2 } } */
/* { dg-final { scan-assembler-times "vmrgow" 2 } } */
