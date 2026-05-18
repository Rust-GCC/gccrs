/* { dg-do compile } */
/* { dg-options "-O2 -march=rv32i_zilsd -mabi=ilp32" } */
/* { dg-additional-options "-mstrict-align -mzilsd-word-align" } */

typedef long long ll4 __attribute__((aligned(4)));

long long
load_ll4 (ll4 *p)
{
  return *p;
}

void
store_ll4 (ll4 *p, long long x)
{
  *p = x;
}

/* { dg-final { scan-assembler-times "ld\t" 1 } } */
/* { dg-final { scan-assembler-times "sd\t" 1 } } */
