/* { dg-do compile } */
/* { dg-options "-O2 -mnarrow-gp-writes -fdump-rtl-narrow_gp_writes" } */
/* { dg-final { scan-assembler-not "\\tadd\\tw\[0-9\]+, w\[0-9\]+, 1" } } */
/* { dg-final { scan-assembler "\\tadd\\tx\[0-9\]+, x\[0-9\]+, 1" } } */
/* { dg-final { scan-rtl-dump-not "Successfully narrowed insn" "narrow_gp_writes" } } */

unsigned long
test_no_narrow (unsigned long x)
{
  x = x & (1UL << 32);
  return x + 1;
}
