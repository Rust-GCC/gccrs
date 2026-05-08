/* { dg-do compile } */
/* { dg-options "-O2 -ftree-slp-vectorize" } */

/* Test that a group of stores of 4 elements loaded from memory is vectorized by
   constructing a vector and storing it, given that the LD1 (ASIMD load, 1
   element, one lane, B/H/S) instruction typically has similar throughput to the
   LDR (Load vector reg, unscaled immed) instruction that would be used by the
   scalar version of the same code.  Any additional latency of LD1 is assumed to
   be represented by the basic cost of vector construction that is applied
   uniformly.  Since there are no GPR-to-SIMD register transfers, there is no
   need to charge additional costs for them.  Only one STR (Store vector reg,
   unsigned immed, B/H/S/D-form) instruction is required by the vectorized code
   but the overall benefit of vectorization is expected to be marginal (except
   on code size) because scalar loads are easier to parallelize, albeit at the
   cost of using more SIMD registers.  */

struct S { __fp16 a, b, c, d; } s;

void
foo (__fp16 *a, __fp16 *b, __fp16 *c, __fp16 *d)
{
  __fp16 a_ = *a, b_ = *b, c_ = *c, d_ = *d;
  s.a = a_;
  s.b = b_;
  s.c = c_;
  s.d = d_;
}

/* { dg-final { scan-assembler-times {\tld1\t{v[0-9]+\.h}\[[0-9]+\], \[x[0-9]+\]\n} 3 } } */
/* (The fourth load is usually ldr hN, ... but don't require that.) */
/* { dg-final { scan-assembler-times {\tstr\td[0-9]+, \[x[0-9]+.*\]\n} 1 } } */

/* { dg-final { scan-assembler-not {\tstr\th[0-9]+, } } } */
