/* { dg-do compile } */
/* { dg-options "-march=rv64gcv_zvfh_zvl4096b -mabi=lp64d -O3 -mrvv-max-lmul=m8 -ffast-math -fdump-tree-vect-details" } */

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

DEF_REDUC_PLUS (int8_t, 8)
DEF_REDUC_PLUS (int16_t, 8)
DEF_REDUC_PLUS (int32_t, 8)
DEF_REDUC_PLUS (int64_t, 8)
DEF_REDUC_PLUS (_Float16, 8)
DEF_REDUC_PLUS (float, 8)
DEF_REDUC_PLUS (double, 8)

/* { dg-final { scan-assembler-times {vredsum\.vs} 4 } } */
/* { dg-final { scan-assembler-times {vfredusum\.vs} 3 } } */
/* { dg-final { scan-assembler-not {csrr} } } */

/* Unordered reduction cost: reduc_*_cost (2) + vr2gr/vr2fr (2) = 4.  */
/* { dg-final { scan-tree-dump "vec_to_scalar costs 4" "vect" } } */
