/* { dg-do compile } */
/* { dg-options "-O2 -mnarrow-gp-writes -fdump-rtl-narrow_gp_writes" } */
/* { dg-final { scan-assembler-not "\\tand\\tx\[0-9\]+, x\[0-9\]+, 4294967295" } } */
/* { dg-final { scan-assembler "\\tuxtw\\tx\[0-9\]+, w\[0-9\]+" } } */
/* { dg-final { scan-rtl-dump-times "Successfully narrowed insn" 1 "narrow_gp_writes" } } */

unsigned long global_val;

void
test_force_zext (unsigned long x)
{
  volatile unsigned long y = x;
  x = y & 0xFFFFFFFF;
  global_val = x;
}
