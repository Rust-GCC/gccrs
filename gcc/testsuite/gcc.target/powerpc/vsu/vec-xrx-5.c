/* { dg-do compile { target { powerpc*-*-* } } } */
/* { dg-options "-mdejagnu-cpu=power9 -mvsx" } */
/* { dg-require-effective-target powerpc_vsx } */

#include <stddef.h>
#include <altivec.h>

unsigned int
fetch_data (unsigned int offset, vector unsigned int *datap)
{
  vector unsigned int data = *datap;

  return vec_xrx (offset, data);
}

/* { dg-final { scan-assembler "vextuwrx" } } */
