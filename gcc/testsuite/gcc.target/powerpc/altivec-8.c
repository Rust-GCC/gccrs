/* Origin: Aldy Hernandez  <aldyh@redhat.com>  */
/* Test rs6000_legitimate_address.  PRE_INC should be invalid.  */

/* { dg-do compile { target powerpc*-*-* } } */
/* { dg-options "-maltivec" } */
/* { dg-require-effective-target powerpc_altivec } */

#include <altivec.h>

vector signed short *hannah;

int
main ()
{
  *hannah++ = __builtin_altivec_vspltish (5);
  *hannah++ = __builtin_altivec_vspltish (6);

  return 0;
}
