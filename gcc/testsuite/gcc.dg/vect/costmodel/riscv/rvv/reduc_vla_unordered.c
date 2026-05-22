/* { dg-do compile } */
/* { dg-options "-march=rv64gcv_zvfh -mabi=lp64d -O3 -mrvv-vector-bits=scalable -ffast-math -fdump-tree-vect-details" } */

#include <stdint-gcc.h>

#define DEF_REDUC_PLUS(TYPE)			\
TYPE __attribute__ ((noinline, noclone))	\
reduc_plus_##TYPE (TYPE *restrict a, int n)	\
{						\
  TYPE r = 0;					\
  for (int i = 0; i < n; ++i)			\
    r += a[i];					\
  return r;					\
}

DEF_REDUC_PLUS (int8_t)
DEF_REDUC_PLUS (int16_t)
DEF_REDUC_PLUS (int32_t)
DEF_REDUC_PLUS (int64_t)
DEF_REDUC_PLUS (_Float16)
DEF_REDUC_PLUS (float)
DEF_REDUC_PLUS (double)

/* { dg-final { scan-tree-dump-times "optimized: loop vectorized" 7 "vect" } } */
/* { dg-final { scan-assembler-times {vredsum\.vs\s+v[0-9]+,\s*v[0-9]+,\s*v[0-9]+} 4 } } */
/* { dg-final { scan-assembler-times {vfredusum\.vs\s+v[0-9]+,\s*v[0-9]+,\s*v[0-9]+} 3 } } */

/* Unordered reduction cost: reduc_*_cost + vr2gr/vr2fr.  */
/* { dg-final { scan-tree-dump "vec_to_scalar costs 4" "vect" } } */
