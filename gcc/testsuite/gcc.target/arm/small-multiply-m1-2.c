/* { dg-do compile } */
/* { dg-require-effective-target arm_cpu_cortex_m1_small_ok } */
/* { dg-options "-Os" } */
/* { dg-add-options arm_cpu_cortex_m1_small } */

int
test (int a)
{
  return a * 0x123456;
}

/* { dg-final { scan-assembler "\[\\t \]+mul" } } */
