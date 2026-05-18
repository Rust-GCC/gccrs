/* { dg-do compile } */
/* { dg-options "-march=rv64gc -mabi=lp64" } */
/* { dg-skip-if "" { *-*-* } { "-O0" } } */

/* Test for zero_extendsidi2_shifted handling arbitrary mask widths
   (not just 32 bits). */
unsigned long f(unsigned int a, unsigned long b)
{
  a = a << 1;
  unsigned long c = (unsigned long) a;
  c = b + (c<<4);
  return c;
}

/* { dg-final { scan-assembler-times "slli" 1 } } */
/* { dg-final { scan-assembler-times "srli" 1 } } */
