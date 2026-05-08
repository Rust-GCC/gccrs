/* { dg-do compile } */
/* { dg-options "-O2 -ftree-slp-vectorize" } */

/* Test that a group of byte-reversed stores of 8 elements derived from
   scalar registers is not vectorized by constructing a vector and
   storing it, given that the GPR-to-SIMD version of INS (which would
   have had to be used to vectorize this code) typically has higher
   latency and lower throughput than the SIMD-to-SIMD version of INS.
   Actually, vectorization would not be profitable in either case because
   it would have the unfortunate side-effect of preventing store-merging
   that would otherwise happen in a later pass, which would prevent the
   byte-reversing pattern from being recognised and lowered using scalar
   instructions.  */

struct S
{
  char a, b, c, d, e, f, g, h;
} s;

void
foo (unsigned int b, unsigned int c)
{
  s.a = b >> 24;
  s.b = b >> 16;
  s.c = b >> 8;
  s.d = b >> 0;
  s.e = c >> 24;
  s.f = c >> 16;
  s.g = c >> 8;
  s.h = c >> 0;
}

/* { dg-final { scan-assembler-times {\trev\t(w[0-9]+), \1\n} 2 } } */

/* { dg-final { scan-assembler-not {\tfmov\ts[0-9]+, w[0-9]+\n} } } */
/* { dg-final { scan-assembler-not {\tins\tv[0-9]+\.b\[[0-9]+\], w[0-9]+\n} } } */
/* { dg-final { scan-assembler-not {\tstr\td[0-9]+, } } } */
