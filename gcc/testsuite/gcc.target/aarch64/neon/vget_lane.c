/* { dg-do compile } */
/* { dg-final { check-function-bodies "**" "" } } */

#include "arm_neon_test.h"

#define TEST_GET_FIRST_LANE(NAME, RET_TYPE, ARG_TYPE)                          \
  RET_TYPE test_##NAME##_first (ARG_TYPE arg1) { return NAME (arg1, 0); }

#define TEST_GET_LAST_LANE(NAME, RET_TYPE, ARG_TYPE)                           \
  RET_TYPE test_##NAME##_last (ARG_TYPE arg1)                                  \
  {                                                                            \
    return NAME (arg1, VEC_LEN (arg1) - 1);                                    \
  }

/*
** test_vget_lane_u8_first:
** umov	w0, v0\.b\[0\]
** ret
*/
TEST_GET_FIRST_LANE (vget_lane_u8, uint8_t, uint8x8_t)

/*
** test_vget_lane_s8_first:
** umov	w0, v0\.b\[0\]
** ret
*/
TEST_GET_FIRST_LANE (vget_lane_s8, int8_t, int8x8_t)

/*
** test_vget_lane_p8_first:
** umov	w0, v0\.b\[0\]
** ret
*/
TEST_GET_FIRST_LANE (vget_lane_p8, poly8_t, poly8x8_t)

/*
** test_vget_lane_mf8_first:
** ret
*/
TEST_GET_FIRST_LANE (vget_lane_mf8, mfloat8_t, mfloat8x8_t)

/*
** test_vgetq_lane_u8_first:
** umov	w0, v0\.b\[0\]
** ret
*/
TEST_GET_FIRST_LANE (vgetq_lane_u8, uint8_t, uint8x16_t)

/*
** test_vgetq_lane_s8_first:
** umov	w0, v0\.b\[0\]
** ret
*/
TEST_GET_FIRST_LANE (vgetq_lane_s8, int8_t, int8x16_t)

/*
** test_vgetq_lane_p8_first:
** umov	w0, v0\.b\[0\]
** ret
*/
TEST_GET_FIRST_LANE (vgetq_lane_p8, poly8_t, poly8x16_t)

/*
** test_vgetq_lane_mf8_first:
** ret
*/
TEST_GET_FIRST_LANE (vgetq_lane_mf8, mfloat8_t, mfloat8x16_t)

/*
** test_vget_lane_u16_first:
** umov	w0, v0\.h\[0\]
** ret
*/
TEST_GET_FIRST_LANE (vget_lane_u16, uint16_t, uint16x4_t)

/*
** test_vget_lane_s16_first:
** umov	w0, v0\.h\[0\]
** ret
*/
TEST_GET_FIRST_LANE (vget_lane_s16, int16_t, int16x4_t)

/*
** test_vget_lane_p16_first:
** umov	w0, v0\.h\[0\]
** ret
*/
TEST_GET_FIRST_LANE (vget_lane_p16, poly16_t, poly16x4_t)

/*
** test_vget_lane_f16_first:
** ret
*/
TEST_GET_FIRST_LANE (vget_lane_f16, float16_t, float16x4_t)

/*
** test_vget_lane_bf16_first:
** ret
*/
TEST_GET_FIRST_LANE (vget_lane_bf16, bfloat16_t, bfloat16x4_t)

/*
** test_vgetq_lane_u16_first:
** umov	w0, v0\.h\[0\]
** ret
*/
TEST_GET_FIRST_LANE (vgetq_lane_u16, uint16_t, uint16x8_t)

/*
** test_vgetq_lane_s16_first:
** umov	w0, v0\.h\[0\]
** ret
*/
TEST_GET_FIRST_LANE (vgetq_lane_s16, int16_t, int16x8_t)

/*
** test_vgetq_lane_p16_first:
** umov	w0, v0\.h\[0\]
** ret
*/
TEST_GET_FIRST_LANE (vgetq_lane_p16, poly16_t, poly16x8_t)

/*
** test_vgetq_lane_f16_first:
** ret
*/
TEST_GET_FIRST_LANE (vgetq_lane_f16, float16_t, float16x8_t)

/*
** test_vgetq_lane_bf16_first:
** ret
*/
TEST_GET_FIRST_LANE (vgetq_lane_bf16, bfloat16_t, bfloat16x8_t)

/*
** test_vget_lane_u32_first:
** fmov	w0, s0
** ret
*/
TEST_GET_FIRST_LANE (vget_lane_u32, uint32_t, uint32x2_t)

/*
** test_vget_lane_s32_first:
** fmov	w0, s0
** ret
*/
TEST_GET_FIRST_LANE (vget_lane_s32, int32_t, int32x2_t)

/*
** test_vget_lane_f32_first:
** ret
*/
TEST_GET_FIRST_LANE (vget_lane_f32, float32_t, float32x2_t)

/*
** test_vgetq_lane_u32_first:
** fmov	w0, s0
** ret
*/
TEST_GET_FIRST_LANE (vgetq_lane_u32, uint32_t, uint32x4_t)

/*
** test_vgetq_lane_s32_first:
** fmov	w0, s0
** ret
*/
TEST_GET_FIRST_LANE (vgetq_lane_s32, int32_t, int32x4_t)

/*
** test_vgetq_lane_f32_first:
** ret
*/
TEST_GET_FIRST_LANE (vgetq_lane_f32, float32_t, float32x4_t)

/*
** test_vget_lane_u64_first:
** fmov	x0, d0
** ret
*/
TEST_GET_FIRST_LANE (vget_lane_u64, uint64_t, uint64x1_t)

/*
** test_vget_lane_s64_first:
** fmov	x0, d0
** ret
*/
TEST_GET_FIRST_LANE (vget_lane_s64, int64_t, int64x1_t)

/*
** test_vget_lane_p64_first:
** fmov	x0, d0
** ret
*/
TEST_GET_FIRST_LANE (vget_lane_p64, poly64_t, poly64x1_t)

/*
** test_vget_lane_f64_first:
** ret
*/
TEST_GET_FIRST_LANE (vget_lane_f64, float64_t, float64x1_t)

/*
** test_vgetq_lane_u64_first:
** fmov	x0, d0
** ret
*/
TEST_GET_FIRST_LANE (vgetq_lane_u64, uint64_t, uint64x2_t)

/*
** test_vgetq_lane_s64_first:
** fmov	x0, d0
** ret
*/
TEST_GET_FIRST_LANE (vgetq_lane_s64, int64_t, int64x2_t)

/*
** test_vgetq_lane_p64_first:
** fmov	x0, d0
** ret
*/
TEST_GET_FIRST_LANE (vgetq_lane_p64, poly64_t, poly64x2_t)

/*
** test_vgetq_lane_f64_first:
** ret
*/
TEST_GET_FIRST_LANE (vgetq_lane_f64, float64_t, float64x2_t)

/*
** test_vget_lane_u8_last:
** umov	w0, v0\.b\[7\]
** ret
*/
TEST_GET_LAST_LANE (vget_lane_u8, uint8_t, uint8x8_t)

/*
** test_vget_lane_s8_last:
** umov	w0, v0\.b\[7\]
** ret
*/
TEST_GET_LAST_LANE (vget_lane_s8, int8_t, int8x8_t)

/*
** test_vget_lane_p8_last:
** umov	w0, v0\.b\[7\]
** ret
*/
TEST_GET_LAST_LANE (vget_lane_p8, poly8_t, poly8x8_t)

/*
** test_vget_lane_mf8_last:
** dup	b0, v0\.b\[7\]
** ret
*/
TEST_GET_LAST_LANE (vget_lane_mf8, mfloat8_t, mfloat8x8_t)

/*
** test_vgetq_lane_u8_last:
** umov	w0, v0\.b\[15\]
** ret
*/
TEST_GET_LAST_LANE (vgetq_lane_u8, uint8_t, uint8x16_t)

/*
** test_vgetq_lane_s8_last:
** umov	w0, v0\.b\[15\]
** ret
*/
TEST_GET_LAST_LANE (vgetq_lane_s8, int8_t, int8x16_t)

/*
** test_vgetq_lane_p8_last:
** umov	w0, v0\.b\[15\]
** ret
*/
TEST_GET_LAST_LANE (vgetq_lane_p8, poly8_t, poly8x16_t)

/*
** test_vgetq_lane_mf8_last:
** dup	b0, v0\.b\[15\]
** ret
*/
TEST_GET_LAST_LANE (vgetq_lane_mf8, mfloat8_t, mfloat8x16_t)

/*
** test_vget_lane_u16_last:
** umov	w0, v0\.h\[3\]
** ret
*/
TEST_GET_LAST_LANE (vget_lane_u16, uint16_t, uint16x4_t)

/*
** test_vget_lane_s16_last:
** umov	w0, v0\.h\[3\]
** ret
*/
TEST_GET_LAST_LANE (vget_lane_s16, int16_t, int16x4_t)

/*
** test_vget_lane_p16_last:
** umov	w0, v0\.h\[3\]
** ret
*/
TEST_GET_LAST_LANE (vget_lane_p16, poly16_t, poly16x4_t)

/*
** test_vget_lane_f16_last:
** dup	h0, v0\.h\[3\]
** ret
*/
TEST_GET_LAST_LANE (vget_lane_f16, float16_t, float16x4_t)

/*
** test_vget_lane_bf16_last:
** dup	h0, v0\.h\[3\]
** ret
*/
TEST_GET_LAST_LANE (vget_lane_bf16, bfloat16_t, bfloat16x4_t)

/*
** test_vgetq_lane_u16_last:
** umov	w0, v0\.h\[7\]
** ret
*/
TEST_GET_LAST_LANE (vgetq_lane_u16, uint16_t, uint16x8_t)

/*
** test_vgetq_lane_s16_last:
** umov	w0, v0\.h\[7\]
** ret
*/
TEST_GET_LAST_LANE (vgetq_lane_s16, int16_t, int16x8_t)

/*
** test_vgetq_lane_p16_last:
** umov	w0, v0\.h\[7\]
** ret
*/
TEST_GET_LAST_LANE (vgetq_lane_p16, poly16_t, poly16x8_t)

/*
** test_vgetq_lane_f16_last:
** dup	h0, v0\.h\[7\]
** ret
*/
TEST_GET_LAST_LANE (vgetq_lane_f16, float16_t, float16x8_t)

/*
** test_vgetq_lane_bf16_last:
** dup	h0, v0\.h\[7\]
** ret
*/
TEST_GET_LAST_LANE (vgetq_lane_bf16, bfloat16_t, bfloat16x8_t)

/*
** test_vget_lane_u32_last:
** umov	w0, v0\.s\[1\]
** ret
*/
TEST_GET_LAST_LANE (vget_lane_u32, uint32_t, uint32x2_t)

/*
** test_vget_lane_s32_last:
** umov	w0, v0\.s\[1\]
** ret
*/
TEST_GET_LAST_LANE (vget_lane_s32, int32_t, int32x2_t)

/*
** test_vget_lane_f32_last:
** dup	s0, v0\.s\[1\]
** ret
*/
TEST_GET_LAST_LANE (vget_lane_f32, float32_t, float32x2_t)

/*
** test_vgetq_lane_u32_last:
** umov	w0, v0\.s\[3\]
** ret
*/
TEST_GET_LAST_LANE (vgetq_lane_u32, uint32_t, uint32x4_t)

/*
** test_vgetq_lane_s32_last:
** umov	w0, v0\.s\[3\]
** ret
*/
TEST_GET_LAST_LANE (vgetq_lane_s32, int32_t, int32x4_t)

/*
** test_vgetq_lane_f32_last:
** dup	s0, v0\.s\[3\]
** ret
*/
TEST_GET_LAST_LANE (vgetq_lane_f32, float32_t, float32x4_t)

/*
** test_vget_lane_u64_last:
** fmov	x0, d0
** ret
*/
TEST_GET_LAST_LANE (vget_lane_u64, uint64_t, uint64x1_t)

/*
** test_vget_lane_s64_last:
** fmov	x0, d0
** ret
*/
TEST_GET_LAST_LANE (vget_lane_s64, int64_t, int64x1_t)

/*
** test_vget_lane_p64_last:
** fmov	x0, d0
** ret
*/
TEST_GET_LAST_LANE (vget_lane_p64, poly64_t, poly64x1_t)

/*
** test_vget_lane_f64_last:
** ret
*/
TEST_GET_LAST_LANE (vget_lane_f64, float64_t, float64x1_t)
/*
** test_vgetq_lane_u64_last:
** umov	x0, v0\.d\[1\]
** ret
*/
TEST_GET_LAST_LANE (vgetq_lane_u64, uint64_t, uint64x2_t)

/*
** test_vgetq_lane_s64_last:
** umov	x0, v0\.d\[1\]
** ret
*/
TEST_GET_LAST_LANE (vgetq_lane_s64, int64_t, int64x2_t)

/*
** test_vgetq_lane_p64_last:
** umov	x0, v0\.d\[1\]
** ret
*/
TEST_GET_LAST_LANE (vgetq_lane_p64, poly64_t, poly64x2_t)

/*
** test_vgetq_lane_f64_last:
** dup	d0, v0\.d\[1\]
** ret
*/
TEST_GET_LAST_LANE (vgetq_lane_f64, float64_t, float64x2_t)
