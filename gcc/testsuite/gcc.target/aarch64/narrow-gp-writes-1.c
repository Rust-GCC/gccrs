/* { dg-do compile } */
/* { dg-options "-O2 -mnarrow-gp-writes -fdump-rtl-narrow_gp_writes" } */
/*{ dg-final { scan-assembler-times "\\tadd\\tw\[0-9\]+, w\[0-9\]+, 3" 1 } } */
/* { dg-final { scan-rtl-dump-times "Successfully narrowed insn" 1 "narrow_gp_writes" } } */

long
test_single_set (long a)
{
  return (a & 0xFF) + 3;
}