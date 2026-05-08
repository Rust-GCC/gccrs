/* { dg-do compile } */
/* { dg-options "-O2 -ftree-slp-vectorize" } */

/* Test that a group of stores of 4 elements held in SIMD registers is
   vectorized by constructing a vector and storing it, given that the
   SIMD-to-SIMD version of INS (used here) typically has lower latency
   and higher throughput than the GPR-to-SIMD version of INS.  Despite
   that, any benefit of vectorization is expected to be marginal
   (including very little reduction in code size). */

struct S
{
  __fp16 a, b, c, d;
} s;

void
foo (__fp16 a, __fp16 b, __fp16 c, __fp16 d)
{
  s.a = a;
  s.b = b;
  s.c = c;
  s.d = d;
}

/* { dg-final { scan-assembler-times {\tins\tv[0-9]+\.h\[[0-9]+\], v[0-9]+\.h\[[0-9]+\]\n} 3 } } */
/* { dg-final { scan-assembler-times {\tstr\td[0-9]+, \[x[0-9]+.*\]\n} 1 } } */

/* { dg-final { scan-assembler-not {\tstr\th[0-9]+, } } } */
