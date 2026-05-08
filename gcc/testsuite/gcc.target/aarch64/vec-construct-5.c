/* { dg-do compile } */
/* { dg-options "-O2 -ftree-slp-vectorize" } */

/* Test that a group of stores of 8 elements held in scalar registers is
   not vectorized by constructing a vector and storing it, given that the
   GPR-to-SIMD version of INS (which would have had to be used to vectorize
   this code) typically has higher latency and lower throughput than the
   SIMD-to-SIMD version of INS (with which such a transformation might
   have been profitable).  No increase in code size is expected as a
   consequence of forgoing vectorization, either.  */

struct S
{
  char a, b, c, d, e, f, g, h;
} s;

void
foo (char a, char b, char c, char d, char e, char f, char g, char h)
{
  s.a = a;
  s.b = b;
  s.c = c;
  s.d = d;
  s.e = e;
  s.f = f;
  s.g = g;
  s.h = h;
}

/* { dg-final { scan-assembler-times {\tstrb\tw[0-9]+, } 8 } } */

/* { dg-final { scan-assembler-not {\tfmov\ts[0-9]+, w[0-9]+\n} } } */
/* { dg-final { scan-assembler-not {\tins\tv[0-9]+\.b\[[0-9]+\], w[0-9]+\n} } } */
/* { dg-final { scan-assembler-not {\tstr\td[0-9]+, } } } */
