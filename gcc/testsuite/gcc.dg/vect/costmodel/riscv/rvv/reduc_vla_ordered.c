/* { dg-do compile } */
/* { dg-options "-march=rv64gcv_zvfh -mabi=lp64d -O3 -mrvv-vector-bits=scalable -fdump-tree-vect-details" } */

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

/* Ordered reduction cost: reduc_f*_ordered_cost + vr2fr.  */
/* { dg-final { scan-tree-dump "vec_to_scalar costs 22" "vect" } } */
/* { dg-final { scan-tree-dump "vec_to_scalar costs 12" "vect" } } */
/* { dg-final { scan-tree-dump "vec_to_scalar costs 7" "vect" } } */
