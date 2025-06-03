/* { dg-do compile { target { powerpc*-*-* } } } */
/* { dg-options "-mdejagnu-cpu=power9 -mvsx" } */
/* { dg-require-effective-target lp64 } */
/* { dg-require-effective-target powerpc_vsx } */

#include <altivec.h>
#include <stdbool.h>

bool
test_data_class (__ieee128 *p)
{
  __ieee128 source = *p;

  /* IEEE 128-bit floating point operations are only supported
     on 64-bit targets.  */
  return scalar_test_data_class (source, 3);
}

/* { dg-final { scan-assembler "xststdcqp" } } */
