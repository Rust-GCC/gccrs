/* { dg-do compile } */
/* { dg-options "-O2 -mavx512f" } */
/* { dg-final { scan-assembler "vunpcklpd\[ \\t\]+%zmm\[0-9\]+, %zmm\[0-9\]+" } } */
/* { dg-final { scan-assembler "vunpcklpd\[ \\t\]+%ymm\[0-9\]+, %ymm\[0-9\]+" } } */
/* { dg-final { scan-assembler "vmovddup\[ \\t\]+\\(" } } */
/* { dg-final { scan-assembler-not "vmovddup\[^\\n\]*\\(%\[er\]sp\\)" } } */

#include <immintrin.h>

__m512d dup512 (__m512d x) { return _mm512_movedup_pd (x); }
__m256d dup256 (__m256d x) { return _mm256_movedup_pd (x); }
__m512d interleave512 (__m512d x) { return _mm512_unpacklo_pd (x, x); }
__m256d interleave256 (__m256d x) { return _mm256_unpacklo_pd (x, x); }
__m512d load512 (double *p) { return _mm512_movedup_pd (_mm512_loadu_pd (p)); }
