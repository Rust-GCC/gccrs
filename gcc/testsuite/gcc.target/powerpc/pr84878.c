/* PR rtl-optimization/84878 */
/* { dg-do compile { target { powerpc*-*-* } } } */
/* { dg-options "-O2 -maltivec -mno-vsx -fmodulo-sched -ftree-vectorize -funroll-loops -fassociative-math -fno-signed-zeros -fno-trapping-math" } */
/* { dg-require-effective-target powerpc_altivec } */

int ek;
float zu;

int
k5 (int ks)
{
  while (ek < 1)
    {
      ks += (int)(0x1000000 + zu + !ek);
      ++ek;
    }
  return ks;
}
