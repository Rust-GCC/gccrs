/* { dg-do compile { target { rv32 } } } */
/* { dg-options "-march=rv32gc_xtheadfmv" } */
/* { dg-skip-if "" { *-*-* } { "-O0" } } */

double
ll2d (long long ll)
{
  return *(double*)&ll;
}

long long
d2ll (double d)
{
  return *(long long*)&d;
}

/* { dg-final { scan-assembler "fmv.w.x" } } */
/* { dg-final { scan-assembler "th.fmv.hw.x" } } */
/* { dg-final { scan-assembler "fmv.x.w" } } */
/* { dg-final { scan-assembler "th.fmv.x.hw" } } */
/* { dg-final { scan-assembler-not "sw" } } */
/* { dg-final { scan-assembler-not "fld" } } */
/* { dg-final { scan-assembler-not "fsd" } } */
/* { dg-final { scan-assembler-not "lw" } } */
