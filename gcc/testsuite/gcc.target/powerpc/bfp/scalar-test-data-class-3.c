/* { dg-do compile { target { powerpc*-*-* } } } */
/* { dg-options "-mdejagnu-cpu=power9 -mvsx" } */
/* { dg-require-effective-target powerpc_vsx } */

#include <altivec.h>
#include <stdbool.h>

bool
test_data_class (float *p)
{
  float source = *p;

  return scalar_test_data_class (source, 256);	/* { dg-error "argument 2 must be a literal between 0 and 127, inclusive" } */
}

