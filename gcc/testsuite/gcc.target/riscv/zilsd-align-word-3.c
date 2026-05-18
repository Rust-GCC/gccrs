/* { dg-do compile } */
/* { dg-options "-O2 -march=rv32i_zilsd -mabi=ilp32" } */
/* { dg-additional-options "-mstrict-align -mzilsd-word-align" } */

typedef long long ll1 __attribute__((aligned(1)));

long long
load_ll1 (ll1 *p)
{
  return *p;
}

void
store_ll1 (ll1 *p, long long x)
{
  *p = x;
}

/* { dg-final { scan-assembler-not "ld\t" } } */
/* { dg-final { scan-assembler-not "sd\t" } } */
