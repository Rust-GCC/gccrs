/* { dg-do compile } */
/* { dg-final { check-function-bodies "**" "" } } */

#include "arm_neon_test.h"

#define TEST_SET_LANE(NAME, ELEM_TYPE, VEC_TYPE)                               \
  VEC_TYPE test_##NAME (VEC_TYPE vec, ELEM_TYPE elem)                          \
  {                                                                            \
    return NAME (elem, vec, 0);                                                \
  }

/*
** test_vset_lane_f16:
** ins	v0\.h\[0\], v1\.h\[0\]
** ret
*/
TEST_SET_LANE (vset_lane_f16, float16_t, float16x4_t)

/*
** test_vset_lane_f32:
** ins	v0\.s\[0\], v1\.s\[0\]
** ret
*/
TEST_SET_LANE (vset_lane_f32, float32_t, float32x2_t)

/*
** test_vset_lane_f64:
** fmov	d0, d1
** ret
*/
TEST_SET_LANE (vset_lane_f64, float64_t, float64x1_t)

/*
** test_vset_lane_p8:
** ins	v0\.b\[0\], w0
** ret
*/
TEST_SET_LANE (vset_lane_p8, poly8_t, poly8x8_t)

/*
** test_vset_lane_mf8:
** ins	v0\.b\[0\], v1\.b\[0\]
** ret
*/
TEST_SET_LANE (vset_lane_mf8, mfloat8_t, mfloat8x8_t)

/*
** test_vset_lane_p16:
** ins	v0\.h\[0\], w0
** ret
*/
TEST_SET_LANE (vset_lane_p16, poly16_t, poly16x4_t)

/*
** test_vset_lane_p64:
** fmov	d0, x0
** ret
*/
TEST_SET_LANE (vset_lane_p64, poly64_t, poly64x1_t)

/*
** test_vset_lane_s8:
** ins	v0\.b\[0\], w0
** ret
*/
TEST_SET_LANE (vset_lane_s8, int8_t, int8x8_t)

/*
** test_vset_lane_s16:
** ins	v0\.h\[0\], w0
** ret
*/
TEST_SET_LANE (vset_lane_s16, int16_t, int16x4_t)

/*
** test_vset_lane_s32:
** ins	v0\.s\[0\], w0
** ret
*/
TEST_SET_LANE (vset_lane_s32, int32_t, int32x2_t)

/*
** test_vset_lane_s64:
** fmov	d0, x0
** ret
*/
TEST_SET_LANE (vset_lane_s64, int64_t, int64x1_t)

/*
** test_vset_lane_u8:
** ins	v0\.b\[0\], w0
** ret
*/
TEST_SET_LANE (vset_lane_u8, uint8_t, uint8x8_t)

/*
** test_vset_lane_u16:
** ins	v0\.h\[0\], w0
** ret
*/
TEST_SET_LANE (vset_lane_u16, uint16_t, uint16x4_t)

/*
** test_vset_lane_u32:
** ins	v0\.s\[0\], w0
** ret
*/
TEST_SET_LANE (vset_lane_u32, uint32_t, uint32x2_t)

/*
** test_vset_lane_u64:
** fmov	d0, x0
** ret
*/
TEST_SET_LANE (vset_lane_u64, uint64_t, uint64x1_t)

/*
** test_vsetq_lane_f16:
** ins	v0\.h\[0\], v1\.h\[0\]
** ret
*/
TEST_SET_LANE (vsetq_lane_f16, float16_t, float16x8_t)

/*
** test_vsetq_lane_f32:
** ins	v0\.s\[0\], v1\.s\[0\]
** ret
*/
TEST_SET_LANE (vsetq_lane_f32, float32_t, float32x4_t)

/*
** test_vsetq_lane_f64:
** ins	v0\.d\[0\], v1\.d\[0\]
** ret
*/
TEST_SET_LANE (vsetq_lane_f64, float64_t, float64x2_t)

/*
** test_vsetq_lane_p8:
** ins	v0\.b\[0\], w0
** ret
*/
TEST_SET_LANE (vsetq_lane_p8, poly8_t, poly8x16_t)

/*
** test_vsetq_lane_mf8:
** ins	v0\.b\[0\], v1\.b\[0\]
** ret
*/
TEST_SET_LANE (vsetq_lane_mf8, mfloat8_t, mfloat8x16_t)

/*
** test_vsetq_lane_p16:
** ins	v0\.h\[0\], w0
** ret
*/
TEST_SET_LANE (vsetq_lane_p16, poly16_t, poly16x8_t)

/*
** test_vsetq_lane_p64:
** ins	v0\.d\[0\], x0
** ret
*/
TEST_SET_LANE (vsetq_lane_p64, poly64_t, poly64x2_t)

/*
** test_vsetq_lane_s8:
** ins	v0\.b\[0\], w0
** ret
*/
TEST_SET_LANE (vsetq_lane_s8, int8_t, int8x16_t)

/*
** test_vsetq_lane_s16:
** ins	v0\.h\[0\], w0
** ret
*/
TEST_SET_LANE (vsetq_lane_s16, int16_t, int16x8_t)

/*
** test_vsetq_lane_s32:
** ins	v0\.s\[0\], w0
** ret
*/
TEST_SET_LANE (vsetq_lane_s32, int32_t, int32x4_t)

/*
** test_vsetq_lane_s64:
** ins	v0\.d\[0\], x0
** ret
*/
TEST_SET_LANE (vsetq_lane_s64, int64_t, int64x2_t)

/*
** test_vsetq_lane_u8:
** ins	v0\.b\[0\], w0
** ret
*/
TEST_SET_LANE (vsetq_lane_u8, uint8_t, uint8x16_t)

/*
** test_vsetq_lane_u16:
** ins	v0\.h\[0\], w0
** ret
*/
TEST_SET_LANE (vsetq_lane_u16, uint16_t, uint16x8_t)

/*
** test_vsetq_lane_u32:
** ins	v0\.s\[0\], w0
** ret
*/
TEST_SET_LANE (vsetq_lane_u32, uint32_t, uint32x4_t)

/*
** test_vsetq_lane_u64:
** ins	v0\.d\[0\], x0
** ret
*/
TEST_SET_LANE (vsetq_lane_u64, uint64_t, uint64x2_t)

/*
** test_vset_lane_bf16:
** ins	v0\.h\[0\], v1\.h\[0\]
** ret
*/
TEST_SET_LANE (vset_lane_bf16, bfloat16_t, bfloat16x4_t)

/*
** test_vsetq_lane_bf16:
** ins	v0\.h\[0\], v1\.h\[0\]
** ret
*/
TEST_SET_LANE (vsetq_lane_bf16, bfloat16_t, bfloat16x8_t)
