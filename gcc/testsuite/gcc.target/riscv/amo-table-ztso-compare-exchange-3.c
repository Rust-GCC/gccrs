/* { dg-do compile } */
/* Verify that compare exchange mappings match the Ztso suggested mapping.  */
/* { dg-add-options riscv_a } */
/* { dg-add-options riscv_ztso } */
/* { dg-final { scan-assembler-times "lr.w\t" 1 } } */
/* { dg-final { scan-assembler-times "sc.w\t" 1 } } */

void foo (int bar, int baz, int qux)
{
  __atomic_compare_exchange_n(&bar, &baz, qux, 1, __ATOMIC_ACQUIRE, __ATOMIC_ACQUIRE);
}
