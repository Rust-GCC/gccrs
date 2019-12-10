/* { dg-do compile { target { powerpc*-*-* } } } */
/* { dg-require-effective-target powerpc_p9vector_ok } */
/* { dg-options "-mdejagnu-cpu=power8" } */

#include <altivec.h>

__vector unsigned int
get_significands (__vector float *p)
{
  __vector float source = *p;

  return __builtin_vec_extract_sig (source);	/* { dg-error "'__builtin_vsx_extract_sig_sp' requires" } */
}
