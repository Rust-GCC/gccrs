/* { dg-do compile } */
/* { dg-options "-mdejagnu-cpu=power9 -mvsx" } */
/* { dg-require-effective-target powerpc_vsx } */

#include <altivec.h>

int doTestBCDSignificance (_Decimal64 *p)
{
  _Decimal64 source = *p;

  if (__builtin_dfp_dtstsfi_eq (63, source))
    return 3;
  else
    return 5;
}

/* { dg-final { scan-assembler	   "dtstsfi" } } */

