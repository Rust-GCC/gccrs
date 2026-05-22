/* Ordered FP reduction costs in VLS mode.  */
/* { dg-do compile } */
/* { dg-options "-march=rv64gcv_zvfh_zvl4096b -mabi=lp64d -O3 -mrvv-max-lmul=m8 -fdump-tree-vect-details" } */

#include <stdint-gcc.h>

#define DEF_REDUC_PLUS(TYPE, NUM)			\
  TYPE __attribute__ ((noinline, noclone))		\
  reduc_plus_##TYPE##_##NUM (TYPE *restrict a)		\
  {							\
    TYPE r = 0;						\
    for (int i = 0; i < NUM; ++i)			\
      r += a[i];					\
    return r;						\
  }

DEF_REDUC_PLUS (_Float16, 8)
DEF_REDUC_PLUS (float, 8)
DEF_REDUC_PLUS (double, 8)

/* { dg-final { scan-assembler-not {csrr} } } */

/* f16: 20+2=22, f32: 10+2=12, f64: 5+2=7.  */
/* { dg-final { scan-tree-dump "vec_to_scalar costs 22" "vect" } } */
/* { dg-final { scan-tree-dump "vec_to_scalar costs 12" "vect" } } */
/* { dg-final { scan-tree-dump "vec_to_scalar costs 7" "vect" } } */
