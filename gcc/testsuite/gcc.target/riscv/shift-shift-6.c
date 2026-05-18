/* { dg-do compile } */
/* { dg-options "-march=rv64gc -mabi=lp64" } */
/* { dg-skip-if "" { *-*-* } { "-O0" "-O1" "-Og" } } */

/* Test for zero_extendsidi2_shifted handling arbitrary mask widths
   (not just 32 bits). */
unsigned sub1(unsigned a, unsigned b)
{
  b = (b << 2) >> 2;
  return a + (b << 1);
}

/* { dg-final { scan-assembler-times "slli" 1 } } */
/* { dg-final { scan-assembler-times "srli" 1 } } */
