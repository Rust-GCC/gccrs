/* { dg-do compile } */
/* { dg-options "-O2 -ftree-vectorize" } */

#include <stdint.h>

#define ADD_LOOP(TYPE)				\
  void __attribute__ ((noinline, noclone))	\
  vec_while_##TYPE (TYPE *restrict a, int n)	\
  {						\
    for (int i = 0; i < n; ++i)			\
      a[i] += 1;				\
  }

#define TEST_ALL(T)				\
  T (int8_t)					\
  T (uint8_t)					\
  T (int16_t)					\
  T (uint16_t)					\
  T (int32_t)					\
  T (uint32_t)					\
  T (int64_t)					\
  T (uint64_t)					\
  T (float)					\
  T (double)

TEST_ALL (ADD_LOOP)

/* { dg-final { scan-assembler-not {\tuqdec} } } */
/* { dg-final { scan-assembler-times {\twhilelo\tp[0-7]\.b, wzr,} 2 } } */
/* { dg-final { scan-assembler-times {\twhilelo\tp[0-7]\.b, w[0-9]+,} 2 } } */
/* { dg-final { scan-assembler-times {\twhilelo\tp[0-7]\.h, wzr,} 2 } } */
/* { dg-final { scan-assembler-times {\twhilelo\tp[0-7]\.h, w[0-9]+,} 2 } } */
/* { dg-final { scan-assembler-times {\twhilelo\tp[0-7]\.s, wzr,} 3 } } */
/* { dg-final { scan-assembler-times {\twhilelo\tp[0-7]\.s, w[0-9]+,} 3 } } */
/* { dg-final { scan-assembler-times {\twhilelo\tp[0-7]\.d, wzr,} 3 } } */
/* { dg-final { scan-assembler-times {\twhilelo\tp[0-7]\.d, w[0-9]+,} 3 } } */
/* { dg-final { scan-assembler-times {\tld1b\tz[0-9]+\.b, p[0-7]/z, \[x0, x[0-9]+\]\n} 2 } } */
/* { dg-final { scan-assembler-times {\tst1b\tz[0-9]+\.b, p[0-7], \[x0, x[0-9]+\]\n} 2 } } */
/* { dg-final { scan-assembler-times {\tld1h\tz[0-9]+\.h, p[0-7]/z, \[x0, x[0-9]+, lsl 1\]\n} 2 } } */
/* { dg-final { scan-assembler-times {\tst1h\tz[0-9]+\.h, p[0-7], \[x0, x[0-9]+, lsl 1\]\n} 2 } } */
/* { dg-final { scan-assembler-times {\tld1w\tz[0-9]+\.s, p[0-7]/z, \[x0, x[0-9]+, lsl 2\]\n} 3 } } */
/* { dg-final { scan-assembler-times {\tst1w\tz[0-9]+\.s, p[0-7], \[x0, x[0-9]+, lsl 2\]\n} 3 } } */
/* { dg-final { scan-assembler-times {\tld1d\tz[0-9]+\.d, p[0-7]/z, \[x0, x[0-9]+, lsl 3\]\n} 3 } } */
/* { dg-final { scan-assembler-times {\tst1d\tz[0-9]+\.d, p[0-7], \[x0, x[0-9]+, lsl 3\]\n} 3 } } */
/* { dg-final { scan-assembler-times {\tb\.any\t} 10 } } */
