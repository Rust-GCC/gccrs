/* { dg-do compile } */
/* { dg-options "-mdejagnu-cpu=power8 -mvsx" } */
/* { dg-require-effective-target powerpc_vsx } */

#include <altivec.h>

int doTestBCDSignificance (_Decimal64 *p)
{
  _Decimal64 source = *p;

  return __builtin_dfp_dtstsfi_eq_dd (5, source);	/* { dg-error "'__builtin_dtstsfi_eq_dd' requires" } */
}

