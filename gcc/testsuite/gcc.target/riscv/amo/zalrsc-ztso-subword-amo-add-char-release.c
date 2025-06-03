/* { dg-do compile } */
/* Verify that subword atomic op mappings match the PSABI doc's suggested mapping.  */
/* { dg-add-options riscv_zalrsc } */
/* { dg-add-options riscv_ztso } */
/* { dg-remove-options riscv_zabha } */
/* { dg-final { scan-assembler-times "lr.w\t" 1 } } */
/* { dg-final { scan-assembler-times "sc.w\t" 1 } } */

void foo (short* bar, short baz)
{
  __atomic_add_fetch(bar, baz, __ATOMIC_RELEASE);
}
