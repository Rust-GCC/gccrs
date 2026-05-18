/* { dg-do compile } */
/* { dg-options "-O2 -march=rv32i_zilsd -mabi=ilp32 -mzilsd-strict-align" } */

typedef double df4 __attribute__((aligned(4)));

double
load_df4 (df4 *p)
{
  return *p;
}

void
store_df4 (df4 *p, double x)
{
  *p = x;
}

/* { dg-final { scan-assembler-not "ld\t" } } */
/* { dg-final { scan-assembler-not "sd\t" } } */
