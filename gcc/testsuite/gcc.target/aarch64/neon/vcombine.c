/* { dg-do compile } */
/* { dg-final { check-function-bodies "**" "" } } */

#include "arm_neon_test.h"

#define TEST_COMBINE(NAME, RET_TYPE, ARG_TYPE)                                 \
  RET_TYPE test_##NAME (ARG_TYPE arg1, ARG_TYPE arg2)                          \
  {                                                                            \
    return NAME (arg1, arg2);                                                  \
  }

/*
** test_vcombine_u8:
** (zip1|uzp1)	v0\.2d, v0\.2d, v1\.2d
** ret
*/
TEST_COMBINE (vcombine_u8, uint8x16_t, uint8x8_t)

/*
** test_vcombine_s8:
** zip1	v0\.2d, v0\.2d, v1\.2d
** ret
*/
TEST_COMBINE (vcombine_s8, int8x16_t, int8x8_t)

/*
** test_vcombine_p8:
** zip1	v0\.2d, v0\.2d, v1\.2d
** ret
*/
TEST_COMBINE (vcombine_p8, poly8x16_t, poly8x8_t)

/*
** test_vcombine_mf8:
** zip1	v0\.2d, v0\.2d, v1\.2d
** ret
*/
TEST_COMBINE (vcombine_mf8, mfloat8x16_t, mfloat8x8_t)

/*
** test_vcombine_u16:
** zip1	v0\.2d, v0\.2d, v1\.2d
** ret
*/
TEST_COMBINE (vcombine_u16, uint16x8_t, uint16x4_t)

/*
** test_vcombine_s16:
** zip1	v0\.2d, v0\.2d, v1\.2d
** ret
*/
TEST_COMBINE (vcombine_s16, int16x8_t, int16x4_t)

/*
** test_vcombine_p16:
** zip1	v0\.2d, v0\.2d, v1\.2d
** ret
*/
TEST_COMBINE (vcombine_p16, poly16x8_t, poly16x4_t)

/*
** test_vcombine_f16:
** zip1	v0\.2d, v0\.2d, v1\.2d
** ret
*/
TEST_COMBINE (vcombine_f16, float16x8_t, float16x4_t)

/*
** test_vcombine_bf16:
** zip1	v0\.2d, v0\.2d, v1\.2d
** ret
*/
TEST_COMBINE (vcombine_bf16, bfloat16x8_t, bfloat16x4_t)

/*
** test_vcombine_u32:
** zip1	v0\.2d, v0\.2d, v1\.2d
** ret
*/
TEST_COMBINE (vcombine_u32, uint32x4_t, uint32x2_t)

/*
** test_vcombine_s32:
** zip1	v0\.2d, v0\.2d, v1\.2d
** ret
*/
TEST_COMBINE (vcombine_s32, int32x4_t, int32x2_t)

/*
** test_vcombine_f32:
** zip1	v0\.2d, v0\.2d, v1\.2d
** ret
*/
TEST_COMBINE (vcombine_f32, float32x4_t, float32x2_t)

/*
** test_vcombine_u64:
** zip1	v0\.2d, v0\.2d, v1\.2d
** ret
*/
TEST_COMBINE (vcombine_u64, uint64x2_t, uint64x1_t)

/*
** test_vcombine_s64:
** zip1	v0\.2d, v0\.2d, v1\.2d
** ret
*/
TEST_COMBINE (vcombine_s64, int64x2_t, int64x1_t)

/*
** test_vcombine_p64:
** zip1	v0\.2d, v0\.2d, v1\.2d
** ret
*/
TEST_COMBINE (vcombine_p64, poly64x2_t, poly64x1_t)

/*
** test_vcombine_f64:
** zip1	v0\.2d, v0\.2d, v1\.2d
** ret
*/
TEST_COMBINE (vcombine_f64, float64x2_t, float64x1_t)
