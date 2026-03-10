/* { dg-do compile } */
/* { dg-options "-O2 -mnarrow-gp-writes -fdump-rtl-narrow_gp_writes" } */
/* { dg-final { scan-assembler "\\tadds\\tw\[0-9\]+, w\[0-9\]+, w\[0-9\]+" } } */
/* { dg-final { scan-rtl-dump-times "Successfully narrowed insn" 1 "narrow_gp_writes" } } */

long z;

int
test_parallel_set (long x, long y)
{
  long a = x & 0x7FFF;
  long b = y & 0x7FFF;

  long result = a + b;
  if (result == 0)
    return 1;

  z = result;
  return 0;
}
