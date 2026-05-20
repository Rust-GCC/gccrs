/* PR target/125357 */
/* { dg-do compile } */
/* { dg-options "-O2 -mavx2 -mno-avx512f -mno-xop" } */
/* { dg-final { scan-assembler-times "\tvpaddq\t" 2 } } */
/* { dg-final { scan-assembler-times "\tvpermilpd\t" 2 } } */

typedef long long v2di __attribute__((vector_size (16)));
typedef double v2df __attribute__((vector_size (16)));

v2di
foo (v2di x, v2di y)
{
  return __builtin_shuffle (x, y);
}

v2df
bar (v2df x, v2di y)
{
  return __builtin_shuffle (x, y);
}
