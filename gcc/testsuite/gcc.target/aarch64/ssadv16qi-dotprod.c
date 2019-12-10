/* { dg-do compile } */
/* { dg-require-effective-target arm_v8_2a_dotprod_neon_ok } */
/* { dg-add-options arm_v8_2a_dotprod_neon }  */
/* { dg-additional-options "-O3" } */

#pragma GCC target "+nosve"

#define N 1024

signed char pix1[N], pix2[N];

int foo (void)
{
  int i_sum = 0;
  int i;

  for (i = 0; i < N; i++)
    i_sum += __builtin_abs (pix1[i] - pix2[i]);

  return i_sum;
}

/* { dg-final { scan-assembler-not {\tsshll\t} } } */
/* { dg-final { scan-assembler-not {\tsshll2\t} } } */
/* { dg-final { scan-assembler-not {\tssubl\t} } } */
/* { dg-final { scan-assembler-not {\tssubl2\t} } } */
/* { dg-final { scan-assembler-not {\tabs\t} } } */

/* { dg-final { scan-assembler {\tsabd\t} } } */
/* { dg-final { scan-assembler {\tudot\t} } } */

