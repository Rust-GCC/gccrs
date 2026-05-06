/* { dg-do compile } */
/* { dg-additional-options "-mfma -ffinite-math-only -fdump-tree-slp-details" } */

struct S { __complex__ float f; };

struct S
foo (const struct S *a, const struct S *b)
{
  struct S r;
  r.f = a->f * b->f;
  return r;
}

/* { dg-final { scan-tree-dump "optimized: basic block part vectorized" "slp2" } } */
/* { dg-final { scan-tree-dump ".VEC_FMADDSUB" "slp2" } } */
