/* { dg-do compile } */
/* { dg-final { check-function-bodies "**" "" } } */

#include "arm_neon_test.h"

#define TEST_DUP_LANE(NAME, TYPE1, TYPE2)                                      \
  TYPE1 test_##NAME (TYPE2 vec2) { return NAME (vec2, 0); }

/*
** test_vdup_lane_p8:
** dup	v0\.8b, v0\.b\[0\]
** ret
*/
TEST_DUP_LANE (vdup_lane_p8, poly8x8_t, poly8x8_t)

/*
** test_vdup_lane_mf8:
** dup	v0\.8b, v0\.b\[0\]
** ret
*/
TEST_DUP_LANE (vdup_lane_mf8, mfloat8x8_t, mfloat8x8_t)

/*
** test_vdup_lane_u8:
** dup	v0\.8b, v0\.b\[0\]
** ret
*/
TEST_DUP_LANE (vdup_lane_u8, uint8x8_t, uint8x8_t)

/*
** test_vdup_lane_s8:
** dup	v0\.8b, v0\.b\[0\]
** ret
*/
TEST_DUP_LANE (vdup_lane_s8, int8x8_t, int8x8_t)

/*
** test_vdup_laneq_p8:
** dup	v0\.8b, v0\.b\[0\]
** ret
*/
TEST_DUP_LANE (vdup_laneq_p8, poly8x8_t, poly8x16_t)

/*
** test_vdup_laneq_mf8:
** dup	v0\.8b, v0\.b\[0\]
** ret
*/
TEST_DUP_LANE (vdup_laneq_mf8, mfloat8x8_t, mfloat8x16_t)

/*
** test_vdup_laneq_u8:
** dup	v0\.8b, v0\.b\[0\]
** ret
*/
TEST_DUP_LANE (vdup_laneq_u8, uint8x8_t, uint8x16_t)

/*
** test_vdup_laneq_s8:
** dup	v0\.8b, v0\.b\[0\]
** ret
*/
TEST_DUP_LANE (vdup_laneq_s8, int8x8_t, int8x16_t)

/*
** test_vdupq_lane_p8:
** dup	v0\.16b, v0\.b\[0\]
** ret
*/
TEST_DUP_LANE (vdupq_lane_p8, poly8x16_t, poly8x8_t)

/*
** test_vdupq_lane_mf8:
** dup	v0\.16b, v0\.b\[0\]
** ret
*/
TEST_DUP_LANE (vdupq_lane_mf8, mfloat8x16_t, mfloat8x8_t)

/*
** test_vdupq_lane_u8:
** dup	v0\.16b, v0\.b\[0\]
** ret
*/
TEST_DUP_LANE (vdupq_lane_u8, uint8x16_t, uint8x8_t)

/*
** test_vdupq_lane_s8:
** dup	v0\.16b, v0\.b\[0\]
** ret
*/
TEST_DUP_LANE (vdupq_lane_s8, int8x16_t, int8x8_t)

/*
** test_vdupq_laneq_p8:
** dup	v0\.16b, v0\.b\[0\]
** ret
*/
TEST_DUP_LANE (vdupq_laneq_p8, poly8x16_t, poly8x16_t)

/*
** test_vdupq_laneq_mf8:
** dup	v0\.16b, v0\.b\[0\]
** ret
*/
TEST_DUP_LANE (vdupq_laneq_mf8, mfloat8x16_t, mfloat8x16_t)

/*
** test_vdupq_laneq_u8:
** dup	v0\.16b, v0\.b\[0\]
** ret
*/
TEST_DUP_LANE (vdupq_laneq_u8, uint8x16_t, uint8x16_t)

/*
** test_vdupq_laneq_s8:
** dup	v0\.16b, v0\.b\[0\]
** ret
*/
TEST_DUP_LANE (vdupq_laneq_s8, int8x16_t, int8x16_t)

/*
** test_vdup_lane_p16:
** dup	v0\.4h, v0\.h\[0\]
** ret
*/
TEST_DUP_LANE (vdup_lane_p16, poly16x4_t, poly16x4_t)

/*
** test_vdup_lane_u16:
** dup	v0\.4h, v0\.h\[0\]
** ret
*/
TEST_DUP_LANE (vdup_lane_u16, uint16x4_t, uint16x4_t)

/*
** test_vdup_lane_s16:
** dup	v0\.4h, v0\.h\[0\]
** ret
*/
TEST_DUP_LANE (vdup_lane_s16, int16x4_t, int16x4_t)

/*
** test_vdup_lane_f16:
** dup	v0\.4h, v0\.h\[0\]
** ret
*/
TEST_DUP_LANE (vdup_lane_f16, float16x4_t, float16x4_t)

/*
** test_vdup_lane_bf16:
** dup	v0\.4h, v0\.h\[0\]
** ret
*/
TEST_DUP_LANE (vdup_lane_bf16, bfloat16x4_t, bfloat16x4_t)

/*
** test_vdup_laneq_p16:
** dup	v0\.4h, v0\.h\[0\]
** ret
*/
TEST_DUP_LANE (vdup_laneq_p16, poly16x4_t, poly16x8_t)

/*
** test_vdup_laneq_u16:
** dup	v0\.4h, v0\.h\[0\]
** ret
*/
TEST_DUP_LANE (vdup_laneq_u16, uint16x4_t, uint16x8_t)

/*
** test_vdup_laneq_s16:
** dup	v0\.4h, v0\.h\[0\]
** ret
*/
TEST_DUP_LANE (vdup_laneq_s16, int16x4_t, int16x8_t)

/*
** test_vdup_laneq_f16:
** dup	v0\.4h, v0\.h\[0\]
** ret
*/
TEST_DUP_LANE (vdup_laneq_f16, float16x4_t, float16x8_t)

/*
** test_vdup_laneq_bf16:
** dup	v0\.4h, v0\.h\[0\]
** ret
*/
TEST_DUP_LANE (vdup_laneq_bf16, bfloat16x4_t, bfloat16x8_t)

/*
** test_vdupq_lane_p16:
** dup	v0\.8h, v0\.h\[0\]
** ret
*/
TEST_DUP_LANE (vdupq_lane_p16, poly16x8_t, poly16x4_t)

/*
** test_vdupq_lane_u16:
** dup	v0\.8h, v0\.h\[0\]
** ret
*/
TEST_DUP_LANE (vdupq_lane_u16, uint16x8_t, uint16x4_t)

/*
** test_vdupq_lane_s16:
** dup	v0\.8h, v0\.h\[0\]
** ret
*/
TEST_DUP_LANE (vdupq_lane_s16, int16x8_t, int16x4_t)

/*
** test_vdupq_lane_f16:
** dup	v0\.8h, v0\.h\[0\]
** ret
*/
TEST_DUP_LANE (vdupq_lane_f16, float16x8_t, float16x4_t)

/*
** test_vdupq_lane_bf16:
** dup	v0\.8h, v0\.h\[0\]
** ret
*/
TEST_DUP_LANE (vdupq_lane_bf16, bfloat16x8_t, bfloat16x4_t)

/*
** test_vdupq_laneq_p16:
** dup	v0\.8h, v0\.h\[0\]
** ret
*/
TEST_DUP_LANE (vdupq_laneq_p16, poly16x8_t, poly16x8_t)

/*
** test_vdupq_laneq_u16:
** dup	v0\.8h, v0\.h\[0\]
** ret
*/
TEST_DUP_LANE (vdupq_laneq_u16, uint16x8_t, uint16x8_t)

/*
** test_vdupq_laneq_s16:
** dup	v0\.8h, v0\.h\[0\]
** ret
*/
TEST_DUP_LANE (vdupq_laneq_s16, int16x8_t, int16x8_t)

/*
** test_vdupq_laneq_f16:
** dup	v0\.8h, v0\.h\[0\]
** ret
*/
TEST_DUP_LANE (vdupq_laneq_f16, float16x8_t, float16x8_t)

/*
** test_vdupq_laneq_bf16:
** dup	v0\.8h, v0\.h\[0\]
** ret
*/
TEST_DUP_LANE (vdupq_laneq_bf16, bfloat16x8_t, bfloat16x8_t)

/*
** test_vdup_lane_u32:
** dup	v0\.2s, v0\.s\[0\]
** ret
*/
TEST_DUP_LANE (vdup_lane_u32, uint32x2_t, uint32x2_t)

/*
** test_vdup_lane_s32:
** dup	v0\.2s, v0\.s\[0\]
** ret
*/
TEST_DUP_LANE (vdup_lane_s32, int32x2_t, int32x2_t)

/*
** test_vdup_lane_f32:
** dup	v0\.2s, v0\.s\[0\]
** ret
*/
TEST_DUP_LANE (vdup_lane_f32, float32x2_t, float32x2_t)

/*
** test_vdup_laneq_u32:
** dup	v0\.2s, v0\.s\[0\]
** ret
*/
TEST_DUP_LANE (vdup_laneq_u32, uint32x2_t, uint32x4_t)

/*
** test_vdup_laneq_s32:
** dup	v0\.2s, v0\.s\[0\]
** ret
*/
TEST_DUP_LANE (vdup_laneq_s32, int32x2_t, int32x4_t)

/*
** test_vdup_laneq_f32:
** dup	v0\.2s, v0\.s\[0\]
** ret
*/
TEST_DUP_LANE (vdup_laneq_f32, float32x2_t, float32x4_t)

/*
** test_vdupq_lane_u32:
** dup	v0\.4s, v0\.s\[0\]
** ret
*/
TEST_DUP_LANE (vdupq_lane_u32, uint32x4_t, uint32x2_t)

/*
** test_vdupq_lane_s32:
** dup	v0\.4s, v0\.s\[0\]
** ret
*/
TEST_DUP_LANE (vdupq_lane_s32, int32x4_t, int32x2_t)

/*
** test_vdupq_lane_f32:
** dup	v0\.4s, v0\.s\[0\]
** ret
*/
TEST_DUP_LANE (vdupq_lane_f32, float32x4_t, float32x2_t)

/*
** test_vdupq_laneq_u32:
** dup	v0\.4s, v0\.s\[0\]
** ret
*/
TEST_DUP_LANE (vdupq_laneq_u32, uint32x4_t, uint32x4_t)

/*
** test_vdupq_laneq_s32:
** dup	v0\.4s, v0\.s\[0\]
** ret
*/
TEST_DUP_LANE (vdupq_laneq_s32, int32x4_t, int32x4_t)

/*
** test_vdupq_laneq_f32:
** dup	v0\.4s, v0\.s\[0\]
** ret
*/
TEST_DUP_LANE (vdupq_laneq_f32, float32x4_t, float32x4_t)

/*
** test_vdup_lane_p64:
** ret
*/
TEST_DUP_LANE (vdup_lane_p64, poly64x1_t, poly64x1_t)

/*
** test_vdup_lane_u64:
** ret
*/
TEST_DUP_LANE (vdup_lane_u64, uint64x1_t, uint64x1_t)

/*
** test_vdup_lane_s64:
** ret
*/
TEST_DUP_LANE (vdup_lane_s64, int64x1_t, int64x1_t)

/*
** test_vdup_lane_f64:
** ret
*/
TEST_DUP_LANE (vdup_lane_f64, float64x1_t, float64x1_t)

/*
** test_vdup_laneq_p64:
** ret
*/
TEST_DUP_LANE (vdup_laneq_p64, poly64x1_t, poly64x2_t)

/*
** test_vdup_laneq_u64:
** ret
*/
TEST_DUP_LANE (vdup_laneq_u64, uint64x1_t, uint64x2_t)

/*
** test_vdup_laneq_s64:
** ret
*/
TEST_DUP_LANE (vdup_laneq_s64, int64x1_t, int64x2_t)

/*
** test_vdup_laneq_f64:
** ret
*/
TEST_DUP_LANE (vdup_laneq_f64, float64x1_t, float64x2_t)

/*
** test_vdupq_lane_p64:
** dup	v0\.2d, v0\.d\[0\]
** ret
*/
TEST_DUP_LANE (vdupq_lane_p64, poly64x2_t, poly64x1_t)

/*
** test_vdupq_lane_u64:
** dup	v0\.2d, v0\.d\[0\]
** ret
*/
TEST_DUP_LANE (vdupq_lane_u64, uint64x2_t, uint64x1_t)

/*
** test_vdupq_lane_s64:
** dup	v0\.2d, v0\.d\[0\]
** ret
*/
TEST_DUP_LANE (vdupq_lane_s64, int64x2_t, int64x1_t)

/*
** test_vdupq_lane_f64:
** dup	v0\.2d, v0\.d\[0\]
** ret
*/
TEST_DUP_LANE (vdupq_lane_f64, float64x2_t, float64x1_t)

/*
** test_vdupq_laneq_p64:
** dup	v0\.2d, v0\.d\[0\]
** ret
*/
TEST_DUP_LANE (vdupq_laneq_p64, poly64x2_t, poly64x2_t)

/*
** test_vdupq_laneq_u64:
** dup	v0\.2d, v0\.d\[0\]
** ret
*/
TEST_DUP_LANE (vdupq_laneq_u64, uint64x2_t, uint64x2_t)

/*
** test_vdupq_laneq_s64:
** dup	v0\.2d, v0\.d\[0\]
** ret
*/
TEST_DUP_LANE (vdupq_laneq_s64, int64x2_t, int64x2_t)

/*
** test_vdupq_laneq_f64:
** dup	v0\.2d, v0\.d\[0\]
** ret
*/
TEST_DUP_LANE (vdupq_laneq_f64, float64x2_t, float64x2_t)

/*
** test_vdupb_lane_p8:
** umov	w0, v0\.b\[0\]
** ret
*/
TEST_DUP_LANE (vdupb_lane_p8, poly8_t, poly8x8_t)

/*
** test_vdupb_laneq_p8:
** umov	w0, v0\.b\[0\]
** ret
*/
TEST_DUP_LANE (vdupb_laneq_p8, poly8_t, poly8x16_t)

/*
** test_vdupb_lane_mf8:
** ret
*/
TEST_DUP_LANE (vdupb_lane_mf8, mfloat8_t, mfloat8x8_t)

/*
** test_vdupb_laneq_mf8:
** ret
*/
TEST_DUP_LANE (vdupb_laneq_mf8, mfloat8_t, mfloat8x16_t)

/*
** test_vdupb_lane_u8:
** umov	w0, v0\.b\[0\]
** ret
*/
TEST_DUP_LANE (vdupb_lane_u8, uint8_t, uint8x8_t)

/*
** test_vdupb_laneq_u8:
** umov	w0, v0\.b\[0\]
** ret
*/
TEST_DUP_LANE (vdupb_laneq_u8, uint8_t, uint8x16_t)

/*
** test_vdupb_lane_s8:
** umov	w0, v0\.b\[0\]
** ret
*/
TEST_DUP_LANE (vdupb_lane_s8, int8_t, int8x8_t)

/*
** test_vdupb_laneq_s8:
** umov	w0, v0\.b\[0\]
** ret
*/
TEST_DUP_LANE (vdupb_laneq_s8, int8_t, int8x16_t)

/*
** test_vduph_lane_u16:
** umov	w0, v0\.h\[0\]
** ret
*/
TEST_DUP_LANE (vduph_lane_u16, uint16_t, uint16x4_t)

/*
** test_vduph_laneq_u16:
** umov	w0, v0\.h\[0\]
** ret
*/
TEST_DUP_LANE (vduph_laneq_u16, uint16_t, uint16x8_t)

/*
** test_vduph_lane_s16:
** umov	w0, v0\.h\[0\]
** ret
*/
TEST_DUP_LANE (vduph_lane_s16, int16_t, int16x4_t)

/*
** test_vduph_laneq_s16:
** umov	w0, v0\.h\[0\]
** ret
*/
TEST_DUP_LANE (vduph_laneq_s16, int16_t, int16x8_t)

/*
** test_vduph_lane_f16:
** ret
*/
TEST_DUP_LANE (vduph_lane_f16, float16_t, float16x4_t)

/*
** test_vduph_laneq_f16:
** ret
*/
TEST_DUP_LANE (vduph_laneq_f16, float16_t, float16x8_t)

/*
** test_vduph_lane_bf16:
** ret
*/
TEST_DUP_LANE (vduph_lane_bf16, bfloat16_t, bfloat16x4_t)

/*
** test_vduph_laneq_bf16:
** ret
*/
TEST_DUP_LANE (vduph_laneq_bf16, bfloat16_t, bfloat16x8_t)

/*
** test_vduph_lane_p16:
** umov	w0, v0\.h\[0\]
** ret
*/
TEST_DUP_LANE (vduph_lane_p16, poly16_t, poly16x4_t)

/*
** test_vduph_laneq_p16:
** umov	w0, v0\.h\[0\]
** ret
*/
TEST_DUP_LANE (vduph_laneq_p16, poly16_t, poly16x8_t)

/*
** test_vdups_lane_u32:
** fmov	w0, s0
** ret
*/
TEST_DUP_LANE (vdups_lane_u32, uint32_t, uint32x2_t)

/*
** test_vdups_laneq_u32:
** fmov	w0, s0
** ret
*/
TEST_DUP_LANE (vdups_laneq_u32, uint32_t, uint32x4_t)

/*
** test_vdups_lane_s32:
** fmov	w0, s0
** ret
*/
TEST_DUP_LANE (vdups_lane_s32, int32_t, int32x2_t)

/*
** test_vdups_laneq_s32:
** fmov	w0, s0
** ret
*/
TEST_DUP_LANE (vdups_laneq_s32, int32_t, int32x4_t)

/*
** test_vdups_lane_f32:
** ret
*/
TEST_DUP_LANE (vdups_lane_f32, float32_t, float32x2_t)

/*
** test_vdups_laneq_f32:
** ret
*/
TEST_DUP_LANE (vdups_laneq_f32, float32_t, float32x4_t)

/*
** test_vdupd_lane_u64:
** fmov	x0, d0
** ret
*/
TEST_DUP_LANE (vdupd_lane_u64, uint64_t, uint64x1_t)

/*
** test_vdupd_laneq_u64:
** fmov	x0, d0
** ret
*/
TEST_DUP_LANE (vdupd_laneq_u64, uint64_t, uint64x2_t)

/*
** test_vdupd_lane_s64:
** fmov	x0, d0
** ret
*/
TEST_DUP_LANE (vdupd_lane_s64, int64_t, int64x1_t)

/*
** test_vdupd_laneq_s64:
** fmov	x0, d0
** ret
*/
TEST_DUP_LANE (vdupd_laneq_s64, int64_t, int64x2_t)

/*
** test_vdupd_lane_f64:
** ret
*/
TEST_DUP_LANE (vdupd_lane_f64, float64_t, float64x1_t)

/*
** test_vdupd_laneq_f64:
** ret
*/
TEST_DUP_LANE (vdupd_laneq_f64, float64_t, float64x2_t)
