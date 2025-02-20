/* { dg-do compile } */
/* { dg-additional-options "-march=rv64gcv_zvfh -mabi=lp64d -Wno-pedantic" } */

#include <stdint-gcc.h>

typedef int64_t vnx4di __attribute__((vector_size (32)));
typedef int32_t vnx8si __attribute__((vector_size (32)));
typedef int16_t vnx16hi __attribute__((vector_size (32)));
typedef int8_t vnx32qi __attribute__((vector_size (32)));
typedef _Float16 vnx16hf __attribute__((vector_size (32)));
typedef float vnx8sf __attribute__((vector_size (32)));
typedef double vnx4df __attribute__((vector_size (32)));

#define VEC_SET(S,V,IDX)			\
  V						\
  __attribute__((noipa))			\
  vec_set_##V##_##IDX (V v, S s)		\
  {						\
    v[IDX] = s;					\
    return v;					\
  }

#define VEC_SET_VAR2(S,V)			\
  V						\
  __attribute__((noipa))			\
  vec_set_var_##V (V v, int16_t idx, S s)	\
  {						\
    v[idx] = s;					\
    return v;					\
  }						\

#define TEST_ALL2(T)				\
  T (_Float16, vnx16hf, 0)			\
  T (_Float16, vnx16hf, 3)			\
  T (_Float16, vnx16hf, 7)			\
  T (_Float16, vnx16hf, 8)			\
  T (_Float16, vnx16hf, 15)			\
  T (float, vnx8sf, 0)				\
  T (float, vnx8sf, 1)				\
  T (float, vnx8sf, 3)				\
  T (float, vnx8sf, 4)				\
  T (float, vnx8sf, 7)				\
  T (double, vnx4df, 0)				\
  T (double, vnx4df, 1)				\
  T (double, vnx4df, 2)				\
  T (double, vnx4df, 3)				\
  T (int64_t, vnx4di, 0)			\
  T (int64_t, vnx4di, 1)			\
  T (int64_t, vnx4di, 2)			\
  T (int64_t, vnx4di, 3)			\
  T (int32_t, vnx8si, 0)			\
  T (int32_t, vnx8si, 1)			\
  T (int32_t, vnx8si, 3)			\
  T (int32_t, vnx8si, 4)			\
  T (int32_t, vnx8si, 7)			\
  T (int16_t, vnx16hi, 0)			\
  T (int16_t, vnx16hi, 1)			\
  T (int16_t, vnx16hi, 7)			\
  T (int16_t, vnx16hi, 8)			\
  T (int16_t, vnx16hi, 15)			\
  T (int8_t, vnx32qi, 0)			\
  T (int8_t, vnx32qi, 1)			\
  T (int8_t, vnx32qi, 15)			\
  T (int8_t, vnx32qi, 16)			\
  T (int8_t, vnx32qi, 31)			\

#define TEST_ALL_VAR2(T)			\
  T (_Float16, vnx16hf)				\
  T (float, vnx8sf)				\
  T (double, vnx4df)				\
  T (int64_t, vnx4di)				\
  T (int32_t, vnx8si)				\
  T (int16_t, vnx16hi)				\
  T (int8_t, vnx32qi)				\

TEST_ALL2 (VEC_SET)
TEST_ALL_VAR2 (VEC_SET_VAR2)

/* { dg-final { scan-assembler-times {vset[i]*vli\s+[a-z0-9,]+,\s*e8,\s*m2,\s*ta,\s*ma} 1 } } */
/* { dg-final { scan-assembler-times {vset[i]*vli\s+[a-z0-9,]+,\s*e8,\s*m2,\s*tu,\s*ma} 5 } } */
/* { dg-final { scan-assembler-times {vset[i]*vli\s+[a-z0-9,]+,\s*e16,\s*m2,\s*ta,\s*ma} 2 } } */
/* { dg-final { scan-assembler-times {vset[i]*vli\s+[a-z0-9,]+,\s*e16,\s*m2,\s*tu,\s*ma} 10 } } */
/* { dg-final { scan-assembler-times {vset[i]*vli\s+[a-z0-9,]+,\s*e32,\s*m2,\s*ta,\s*ma} 2 } } */
/* { dg-final { scan-assembler-times {vset[i]*vli\s+[a-z0-9,]+,\s*e32,\s*m2,\s*tu,\s*ma} 10 } } */
/* { dg-final { scan-assembler-times {vset[i]*vli\s+[a-z0-9,]+,\s*e64,\s*m2,\s*ta,\s*ma} 2 } } */
/* { dg-final { scan-assembler-times {vset[i]*vli\s+[a-z0-9,]+,\s*e64,\s*m2,\s*tu,\s*ma} 8 } } */

/* { dg-final { scan-assembler-times {\tvmv.v.x} 19 } } */
/* { dg-final { scan-assembler-times {\tvfmv.v.f} 14 } } */
/* { dg-final { scan-assembler-times {\tvslideup.vi} 26 } } */
/* { dg-final { scan-assembler-times {\tvslideup.vx} 7 } } */

/* { dg-final { scan-assembler-times {\tvfmv.s.f} 3 } } */
/* { dg-final { scan-assembler-times {\tvmv.s.x} 4 } } */
