/* { dg-do compile { target { ! riscv_abi_e } } } */
/* { dg-options "-O3 -march=rv64gcv -mrvv-max-lmul=m8 -Wno-overflow -mabi=lp64d" { target { rv64 } } } */
/* { dg-options "-O3 -march=rv32gcv -mrvv-max-lmul=m8 -Wno-overflow -mabi=ilp32" { target { rv32 } } } */

typedef int vnx4i __attribute__ ((vector_size (16)));

vnx4i
test (vnx4i x, vnx4i y)
{
  return __builtin_shufflevector (x, y, 2, 3, 4, 5);
}

/* { dg-final { scan-assembler-times "vslideup" 1 } } */
/* { dg-final { scan-assembler-times "vslidedown" 1 } } */
/* { dg-final { scan-assembler-not "vcompress" } } */
