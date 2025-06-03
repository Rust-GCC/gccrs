/* { dg-do compile { target { ! riscv_abi_e } } } */
/* { dg-options "-minline-strlen -march=rv32gc_xtheadbb" { target { rv32 } } } */
/* { dg-options "-minline-strlen -march=rv64gc_xtheadbb" { target { rv64 } } } */
/* { dg-skip-if "" { *-*-* } { "-O0" "-Os" "-Og" "-Oz" } } */

typedef long unsigned int size_t;

size_t
my_str_len (const char *s)
{
  s = __builtin_assume_aligned (s, 4096);
  return __builtin_strlen (s);
}

/* { dg-final { scan-assembler "th.tstnbz\t" } } */
/* { dg-final { scan-assembler-not {\mjalr} } } */
/* { dg-final { scan-assembler-not {\mcall} } } */
/* { dg-final { scan-assembler-not {\mjr} } } */
/* { dg-final { scan-assembler-not {\mtail} } } */
