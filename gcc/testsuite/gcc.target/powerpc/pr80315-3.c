/* { dg-do compile { target { powerpc*-*-* } } } */
/* { dg-require-effective-target powerpc_p8vector_ok } */
/* { dg-options "-mdejagnu-cpu=power8" } */

#include <altivec.h>

vector unsigned int
main ()
{
  vector unsigned int test, res;
  const int s0 = 0;
  int mask;

  /* Argument 2 must be 0 or 1.  Argument 3 must be in range 0..15.  */
  res = vec_shasigma_be (test, 1, 0xff); /* { dg-error {argument 3 must be in the range \[0, 15\]} } */
  return res;
}
