/* { dg-do compile { target { riscv32*-*-* } } } */
/* { dg-options "-O2 -march=rv32gc_zbkb -mabi=ilp32" } */

int
f (unsigned int *flags)
{
  *flags |= 0x20000U;
  return 1;
}
