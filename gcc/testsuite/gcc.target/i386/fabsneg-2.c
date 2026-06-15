/* { dg-do compile } */
/* { dg-options "-O2 -mfpmath=sse -march=skylake" } */

float x;
float y;
float z;

void foo()
{
  x = -x;
  y = -y;
  z = -z;
}

void bar()
{
  x = __builtin_fabsf(x);
  y = __builtin_fabsf(y);
  z = __builtin_fabsf(z);
}

/* { dg-final { scan-assembler-times "LC0\[,(\]" 1 } } */
/* { dg-final { scan-assembler-times "LC1\[,(\]" 1 } } */
