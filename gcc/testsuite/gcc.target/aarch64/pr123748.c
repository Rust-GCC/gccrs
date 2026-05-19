/* { dg-do compile } */
/* { dg-options "-Ofast" } */

void
f (float *__restrict f, long long *__restrict l)
{
  for (int i = 0; i < 128; i++)
    f[i] = l[i];
}

/* { dg-final { scan-assembler-times {scvtf\t} 2 } } */
/* { dg-final { scan-assembler-times {fcvtn\t} 2 } } */
/* { dg-final { scan-assembler-times {zip1\t} 1 } } */
