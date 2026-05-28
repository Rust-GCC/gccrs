/* { dg-do compile { target int128 } } */
/* { dg-options "-O2 -msse4.1 -mno-stackrealign" } */

typedef long long __m128i __attribute__ ((__vector_size__ (16)));

int foo (__m128i x, __m128i y)
{
  return (__int128)x == (__int128)y;
}

int bar (__m128i x, __m128i y)
{
  return (__int128)(x^y) == 0;
}

int baz (__m128i x, __m128i y)
{
  return (__int128)(x==y) == ~0;
}

/* { dg-final { scan-assembler-times "ptest\[ \\t\]+%" 3 } } */
/* { dg-final { scan-assembler-not "%\[er\]sp" } } */
