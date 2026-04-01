/* PR target/124710 */
/* { dg-do assemble } */
/* { dg-options "-O -masm=intel -mavx10.2" } */
/* { dg-require-effective-target masm_intel } */
/* { dg-require-effective-target avx10_2 } */

#include <immintrin.h>

__m128i v;
__m128 w;

void
foo()
{
  v = _mm_mask_cvtts_ps_epi64(v, -1, w);
}
