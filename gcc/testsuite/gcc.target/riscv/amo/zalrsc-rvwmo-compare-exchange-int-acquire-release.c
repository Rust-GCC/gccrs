/* { dg-do compile } */
/* Verify that compare exchange mappings match the PSABI doc's recommended mapping.  */
/* { dg-add-options riscv_zalrsc } */
/* { dg-remove-options riscv_ztso } */
/* { dg-remove-options riscv_zacas } */
/* Mixed mappings need to be unioned.  */
/* { dg-final { scan-assembler-times "lr.w.aq\t" 1 } } */
/* { dg-final { scan-assembler-times "sc.w.rl\t" 1 } } */

void foo (int bar, int baz, int qux)
{
  __atomic_compare_exchange_n(&bar, &baz, qux, 1, __ATOMIC_RELEASE, __ATOMIC_ACQUIRE);
}
