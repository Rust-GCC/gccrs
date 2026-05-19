/* { dg-do compile { target x86_64-*-mingw* } } */
/* { dg-options "-O1 -mavx512f" } */

typedef long long __m512i __attribute__ ((__vector_size__ (64)));

__m512i
foo (void)
{
  return (__m512i) { 1, 2, 3, 4, 5, 6, 7, 8 };
}

/* { dg-final { scan-assembler "zmm0" } } */
