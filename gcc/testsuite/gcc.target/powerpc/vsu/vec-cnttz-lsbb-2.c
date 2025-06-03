/* { dg-do compile } */
/* { dg-options "-mdejagnu-cpu=power8 -mvsx" } */
/* { dg-require-effective-target powerpc_vsx } */

#include <altivec.h>

int
count_trailing_zero_byte_bits (vector unsigned char *arg1_p)
{
  vector unsigned char arg_1 = *arg1_p;

  return __builtin_vec_vctzlsbb (arg_1);	/* { dg-error "'__builtin_altivec_vctzlsbb_v16qi' requires the '-mcpu=power9' and '-mvsx' options" } */
}
