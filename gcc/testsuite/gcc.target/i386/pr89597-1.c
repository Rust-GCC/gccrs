/* { dg-do compile { target x86_64-*-mingw* } } */
/* { dg-options "-O1 -mavx" } */

typedef long long __m256i __attribute__ ((__vector_size__ (32)));

__m256i
foo (void)
{
  return (__m256i) { 1, 2, 3, 4 };
}

/* { dg-final { scan-assembler "ymm0" } } */
