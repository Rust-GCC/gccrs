/* { dg-do compile } */
/* { dg-options "-march=rv64gcv_zvfh -mabi=lp64d -mcpu=xt-c9501fdvt -O3 -mrvv-vector-bits=scalable -fvect-cost-model=unlimited -fdump-tree-vect-details" } */

#include <stdint-gcc.h>

#define DEF_REDUC_PLUS(TYPE)				\
  TYPE __attribute__ ((noinline, noclone))		\
  reduc_plus_##TYPE (TYPE *restrict a, int n)		\
  {							\
    TYPE r = 0;						\
    for (int i = 0; i < n; ++i)				\
      r += a[i];					\
    return r;						\
  }

DEF_REDUC_PLUS (_Float16)
DEF_REDUC_PLUS (float)
DEF_REDUC_PLUS (double)

/* { dg-final { scan-assembler {vfredosum\.vs\s+v[0-9]+,\s*v[0-9]+,\s*v[0-9]+} } } */
/* { dg-final { scan-tree-dump "vec_to_scalar costs 19 in body" "vect" } } */
/* { dg-final { scan-tree-dump "vec_to_scalar costs 18 in body" "vect" } } */
/* { dg-final { scan-tree-dump "vec_to_scalar costs 14 in body" "vect" } } */
