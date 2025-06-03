/* { dg-do compile } */
/* Verify that compare exchange mappings match the PSABI doc's recommended mapping.  */
/* { dg-add-options riscv_zalrsc } */
/* { dg-add-options riscv_ztso } */
/* { dg-remove-options riscv_zacas } */
/* { dg-final { scan-assembler-times "lr.w.aqrl\t" 1 } } */
/* { dg-final { scan-assembler-times "sc.w.rl\t" 1 } } */

void foo (int bar, int baz, int qux)
{
  __atomic_compare_exchange_n(&bar, &baz, qux, 1, __ATOMIC_SEQ_CST, __ATOMIC_SEQ_CST);
}
