/* { dg-do compile } */
/* { dg-options "-O2 -mnarrow-gp-writes -fdump-rtl-narrow_gp_writes" } */
/* { dg-final { scan-assembler-not "\\tadd\\tw\[0-9\]+, w\[0-9\]+, 1" } } */
/* { dg-final { scan-rtl-dump-not "Successfully narrowed insn" "narrow_gp_writes" } } */

unsigned long
test_phi2 (int cond, unsigned long x)
{
  if (cond)
    x = 0xFF;
  return x + 1;
}