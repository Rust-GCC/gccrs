/* { dg-do compile } */
/* { dg-options "-O2 -ftree-slp-vectorize" } */

/* Test that a group of stores of 4 elements held in scalar registers is
   not vectorized by constructing a vector and storing it, given that the
   GPR-to-SIMD version of INS (which would have had to be used to vectorize
   this code) typically has higher latency and lower throughput than the
   SIMD-to-SIMD version of INS (with which such a transformation might
   have been profitable).  No increase in code size is expected as a
   consequence of forgoing vectorization, either.  */

struct S
{
  short a, b, c, d;
} s;

void
foo (short a, short b, short c, short d)
{
  s.a = a;
  s.b = b;
  s.c = c;
  s.d = d;
}

/* { dg-final { scan-assembler-times {\tstrh\tw[0-9]+, } 4 } } */

/* { dg-final { scan-assembler-not {\tfmov\ts[0-9]+, w[0-9]+\n} } } */
/* { dg-final { scan-assembler-not {\tins\tv[0-9]+\.h\[[0-9]+\], w[0-9]+\n} } } */
/* { dg-final { scan-assembler-not {\tstr\td[0-9]+, } } } */
