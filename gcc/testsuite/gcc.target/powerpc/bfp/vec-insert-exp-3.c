/* { dg-do compile { target { powerpc*-*-* } } } */
/* { dg-require-effective-target powerpc_p9vector_ok } */
/* { dg-options "-mdejagnu-cpu=power8" } */

#include <altivec.h>

__vector double
make_doubles (__vector unsigned long long int *significands_p,
	      __vector unsigned long long int *exponents_p)
{
  __vector unsigned long long int significands = *significands_p;
  __vector unsigned long long int exponents = *exponents_p;

  return __builtin_vec_insert_exp (significands, exponents); /* { dg-error "'__builtin_vsx_insert_exp_dp' requires" } */
}
