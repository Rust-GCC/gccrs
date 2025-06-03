/* { dg-do compile } */
/* { dg-options "-O2" } */

float f1 (float x, float y)
{
  return __builtin_copysignf (1.0, x) * __builtin_copysignf (1.0, y);
}

double f2 (double x, double y)
{
  return __builtin_copysign (1.0, x) * __builtin_copysign (1.0, y);
}

/* { dg-final { scan-assembler-times "movi\t" 2 } } */
/* { dg-final { scan-assembler-not "copysign\tw" } } */
/* { dg-final { scan-assembler-not "dup\tw" } } */
