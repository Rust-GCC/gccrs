/* { dg-do compile } */
/* { dg-options "-O3" } */

#include "arm_neon.h"

#define BUILD_TEST(TYPE1, TYPE2, Q1, Q2, SUFFIX, INDEX1, INDEX2)	\
TYPE1 __attribute__((noinline,noclone))					\
test_copy##Q1##_lane##Q2##_##SUFFIX (TYPE1 a, TYPE2 b)			\
{									\
  return vcopy##Q1##_lane##Q2##_##SUFFIX (a, INDEX1, b, INDEX2);	\
}

/* vcopy_lane.  */
BUILD_TEST (poly8x8_t, poly8x8_t, , , p8, 7, 6)
BUILD_TEST (int8x8_t,  int8x8_t,  , , s8, 7, 6)
BUILD_TEST (uint8x8_t, uint8x8_t, , , u8, 7, 6)
BUILD_TEST (mfloat8x8_t, mfloat8x8_t, , , mf8, 7, 6)
/* { dg-final { scan-assembler-times "ins\\tv0.b\\\[7\\\], v1.b\\\[6\\\]" 4 } } */
BUILD_TEST (poly16x4_t, poly16x4_t, , , p16, 3, 2)
BUILD_TEST (int16x4_t,  int16x4_t,  , , s16, 3, 2)
BUILD_TEST (uint16x4_t, uint16x4_t, , , u16, 3, 2)
/* { dg-final { scan-assembler-times "ins\\tv0.h\\\[3\\\], v1.h\\\[2\\\]" 3 } } */
BUILD_TEST (float32x2_t, float32x2_t, , , f32, 1, 0)
BUILD_TEST (int32x2_t,   int32x2_t,   , , s32, 1, 0)
BUILD_TEST (uint32x2_t,  uint32x2_t,  , , u32, 1, 0)
/* { dg-final { scan-assembler-times "zip1\\tv0.2s, v0.2s, v1.2s" 3 } } */
BUILD_TEST (int64x1_t,   int64x1_t,   , , s64, 0, 0)
BUILD_TEST (uint64x1_t,  uint64x1_t,  , , u64, 0, 0)
BUILD_TEST (float64x1_t, float64x1_t, , , f64, 0, 0)
/* { dg-final { scan-assembler-times "fmov\\td0, d1" 3 } } */

/* vcopy_laneq.  */

BUILD_TEST (poly8x8_t, poly8x16_t, , q, p8, 7, 15)
BUILD_TEST (int8x8_t,  int8x16_t,  , q, s8, 7, 15)
BUILD_TEST (uint8x8_t, uint8x16_t, , q, u8, 7, 15)
BUILD_TEST (mfloat8x8_t, mfloat8x16_t, , q, mf8, 7, 15)
/* { dg-final { scan-assembler-times "ins\\tv0.b\\\[7\\\], v1.b\\\[15\\\]" 4 } } */
BUILD_TEST (poly16x4_t, poly16x8_t, , q, p16, 3, 7)
BUILD_TEST (int16x4_t,  int16x8_t,  , q, s16, 3, 7)
BUILD_TEST (uint16x4_t, uint16x8_t, , q, u16, 3, 7)
/* { dg-final { scan-assembler-times "ins\\tv0.h\\\[3\\\], v1.h\\\[7\\\]" 3 } } */
BUILD_TEST (float32x2_t, float32x4_t, , q, f32, 1, 3)
BUILD_TEST (int32x2_t,   int32x4_t,   , q, s32, 1, 3)
BUILD_TEST (uint32x2_t,  uint32x4_t,  , q, u32, 1, 3)
/* { dg-final { scan-assembler-times "ins\\tv0.s\\\[1\\\], v1.s\\\[3\\\]" 3 } } */
BUILD_TEST (float64x1_t, float64x2_t, , q, f64, 0, 1)
BUILD_TEST (int64x1_t,  int64x2_t,    , q, s64, 0, 1)
BUILD_TEST (uint64x1_t, uint64x2_t,   , q, u64, 0, 1)
/* { dg-final { scan-assembler-times "dup\\td0, v1.d\\\[1\\\]" 3 } } */

/* vcopyq_lane.  */
BUILD_TEST (poly8x16_t, poly8x8_t, q, , p8, 15, 7)
BUILD_TEST (int8x16_t,  int8x8_t,  q, , s8, 15, 7)
BUILD_TEST (uint8x16_t, uint8x8_t, q, , u8, 15, 7)
BUILD_TEST (mfloat8x16_t, mfloat8x8_t, q, , mf8, 15, 7)
/* { dg-final { scan-assembler-times "ins\\tv0.b\\\[15\\\], v1.b\\\[7\\\]" 4 } } */
BUILD_TEST (poly16x8_t, poly16x4_t, q, , p16, 7, 3)
BUILD_TEST (int16x8_t,  int16x4_t,  q, , s16, 7, 3)
BUILD_TEST (uint16x8_t, uint16x4_t, q, , u16, 7, 3)
/* { dg-final { scan-assembler-times "ins\\tv0.h\\\[7\\\], v1.h\\\[3\\\]" 3 } } */
BUILD_TEST (float32x4_t, float32x2_t, q, , f32, 3, 1)
BUILD_TEST (int32x4_t,   int32x2_t,   q, , s32, 3, 1)
BUILD_TEST (uint32x4_t,  uint32x2_t,  q, , u32, 3, 1)
/* { dg-final { scan-assembler-times "ins\\tv0.s\\\[3\\\], v1.s\\\[1\\\]" 3 } } */
BUILD_TEST (float64x2_t, float64x1_t, q, , f64, 1, 0)
BUILD_TEST (int64x2_t,   int64x1_t,   q, , s64, 1, 0)
BUILD_TEST (uint64x2_t,  uint64x1_t,  q, , u64, 1, 0)
/* { dg-final { scan-assembler-times "ins\\tv0.d\\\[1\\\], v1.d\\\[0\\\]" 3 } } */

/* vcopyq_laneq.  */

BUILD_TEST (poly8x16_t, poly8x16_t, q, q, p8, 14, 15)
BUILD_TEST (int8x16_t,  int8x16_t,  q, q, s8, 14, 15)
BUILD_TEST (uint8x16_t, uint8x16_t, q, q, u8, 14, 15)
BUILD_TEST (mfloat8x16_t, mfloat8x16_t, q, q, mf8, 14, 15)
/* { dg-final { scan-assembler-times "ins\\tv0.b\\\[14\\\], v1.b\\\[15\\\]" 4 } } */
BUILD_TEST (poly16x8_t, poly16x8_t, q, q, p16, 6, 7)
BUILD_TEST (int16x8_t,  int16x8_t,  q, q, s16, 6, 7)
BUILD_TEST (uint16x8_t, uint16x8_t, q, q, u16, 6, 7)
/* { dg-final { scan-assembler-times "ins\\tv0.h\\\[6\\\], v1.h\\\[7\\\]" 3 } } */
BUILD_TEST (float32x4_t, float32x4_t, q, q, f32, 2, 3)
BUILD_TEST (int32x4_t,   int32x4_t,   q, q, s32, 2, 3)
BUILD_TEST (uint32x4_t,  uint32x4_t,  q, q, u32, 2, 3)
/* { dg-final { scan-assembler-times "ins\\tv0.s\\\[2\\\], v1.s\\\[3\\\]" 3 } } */
BUILD_TEST (float64x2_t, float64x2_t, q, q, f64, 1, 1)
BUILD_TEST (int64x2_t,   int64x2_t,   q,  q, s64, 1, 1)
BUILD_TEST (uint64x2_t,  uint64x2_t,  q, q, u64, 1, 1)
/* { dg-final { scan-assembler-times "ins\\tv0.d\\\[1\\\], v1.d\\\[1\\\]" 3 } } */
