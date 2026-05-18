/* { dg-do compile } */
/* { dg-options "-O2 -march=rv32i_zilsd -mabi=ilp32" } */

long long
load_ll (long long *p)
{
  return *p;
}

void
store_ll (long long *p, long long x)
{
  *p = x;
}

/* { dg-final { scan-assembler-times "ld\t" 1 } } */
/* { dg-final { scan-assembler-times "sd\t" 1 } } */
