/* { dg-do compile } */
/* { dg-options "-O2" } */
/* { dg-require-effective-target arm32 } */

long long ashll_fn (long long a)
{
  long long c;

  c = a << 33;
  c += a;
  return c;
}

/* { dg-final { scan-assembler "add\tr1, r1, r0, lsl #1" } } */
/* { dg-final { scan-assembler-not "adc" } } */
/* { dg-final { scan-assembler-not "adds" } } */
/* { dg-final { scan-assembler-not "lsr" } } */
/* { dg-final { scan-assembler-not "orr" } } */
/* { dg-final { scan-assembler-not "sbc" } } */
/* { dg-final { scan-assembler-not "subs" } } */

