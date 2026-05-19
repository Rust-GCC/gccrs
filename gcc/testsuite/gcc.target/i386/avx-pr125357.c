/* PR target/125357 */
/* { dg-do compile } */
/* { dg-options "-O2 -mavx -mno-avx2 -mno-xop" } */
/* { dg-final { scan-assembler-times "\tvpermilps\t" 2 } } */

typedef int v4si __attribute__((vector_size (16)));
typedef float v4sf __attribute__((vector_size (16)));

v4si
foo (v4si x, v4si y)
{
  return __builtin_shuffle (x, y);
}

v4sf
bar (v4sf x, v4si y)
{
  return __builtin_shuffle (x, y);
}
