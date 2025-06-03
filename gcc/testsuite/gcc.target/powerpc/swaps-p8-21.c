/* { dg-do compile } */
/* { dg-options "-O2 -mdejagnu-cpu=power8 -maltivec" } */
/* { dg-require-effective-target powerpc_altivec } */

/* The expansion for vector character multiply introduces a vperm operation.
   This tests that changing the vperm mask allows us to remove all swaps
   from the generated code.  */

#include <altivec.h>

void abort ();

vector unsigned char r;
vector unsigned char v =
  { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };
vector unsigned char i =
  { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16 };

int main ()
{
  int j;
  r = v * i;
  return 0;
}

/* { dg-final { scan-assembler-times "vperm" 1 } } */
/* { dg-final { scan-assembler-not "xxpermdi" } } */
