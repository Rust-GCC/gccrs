/* { dg-do compile } */
/* { dg-final { check-function-bodies "**" "" } } */

#include "arm_neon_test.h"

// FIXME: reinterprets to/from p128 generate load and stores to the stack,
// rather than GPR <-> FPR moves

/*
** test_vreinterpret_u8_s8:
** ret
*/
TEST_UNARY (vreinterpret_u8_s8, uint8x8_t, int8x8_t)

/*
** test_vreinterpret_u8_p8:
** ret
*/
TEST_UNARY (vreinterpret_u8_p8, uint8x8_t, poly8x8_t)

/*
** test_vreinterpret_u8_mf8:
** ret
*/
TEST_UNARY (vreinterpret_u8_mf8, uint8x8_t, mfloat8x8_t)

/*
** test_vreinterpret_u8_u16:
** ret
*/
TEST_UNARY (vreinterpret_u8_u16, uint8x8_t, uint16x4_t)

/*
** test_vreinterpret_u8_s16:
** ret
*/
TEST_UNARY (vreinterpret_u8_s16, uint8x8_t, int16x4_t)

/*
** test_vreinterpret_u8_p16:
** ret
*/
TEST_UNARY (vreinterpret_u8_p16, uint8x8_t, poly16x4_t)

/*
** test_vreinterpret_u8_f16:
** ret
*/
TEST_UNARY (vreinterpret_u8_f16, uint8x8_t, float16x4_t)

/*
** test_vreinterpret_u8_bf16:
** ret
*/
TEST_UNARY (vreinterpret_u8_bf16, uint8x8_t, bfloat16x4_t)

/*
** test_vreinterpret_u8_u32:
** ret
*/
TEST_UNARY (vreinterpret_u8_u32, uint8x8_t, uint32x2_t)

/*
** test_vreinterpret_u8_s32:
** ret
*/
TEST_UNARY (vreinterpret_u8_s32, uint8x8_t, int32x2_t)

/*
** test_vreinterpret_u8_f32:
** ret
*/
TEST_UNARY (vreinterpret_u8_f32, uint8x8_t, float32x2_t)

/*
** test_vreinterpret_u8_u64:
** ret
*/
TEST_UNARY (vreinterpret_u8_u64, uint8x8_t, uint64x1_t)

/*
** test_vreinterpret_u8_s64:
** ret
*/
TEST_UNARY (vreinterpret_u8_s64, uint8x8_t, int64x1_t)

/*
** test_vreinterpret_u8_p64:
** ret
*/
TEST_UNARY (vreinterpret_u8_p64, uint8x8_t, poly64x1_t)

/*
** test_vreinterpret_u8_f64:
** ret
*/
TEST_UNARY (vreinterpret_u8_f64, uint8x8_t, float64x1_t)

/*
** test_vreinterpret_s8_u8:
** ret
*/
TEST_UNARY (vreinterpret_s8_u8, int8x8_t, uint8x8_t)

/*
** test_vreinterpret_s8_p8:
** ret
*/
TEST_UNARY (vreinterpret_s8_p8, int8x8_t, poly8x8_t)

/*
** test_vreinterpret_s8_mf8:
** ret
*/
TEST_UNARY (vreinterpret_s8_mf8, int8x8_t, mfloat8x8_t)

/*
** test_vreinterpret_s8_u16:
** ret
*/
TEST_UNARY (vreinterpret_s8_u16, int8x8_t, uint16x4_t)

/*
** test_vreinterpret_s8_s16:
** ret
*/
TEST_UNARY (vreinterpret_s8_s16, int8x8_t, int16x4_t)

/*
** test_vreinterpret_s8_p16:
** ret
*/
TEST_UNARY (vreinterpret_s8_p16, int8x8_t, poly16x4_t)

/*
** test_vreinterpret_s8_f16:
** ret
*/
TEST_UNARY (vreinterpret_s8_f16, int8x8_t, float16x4_t)

/*
** test_vreinterpret_s8_bf16:
** ret
*/
TEST_UNARY (vreinterpret_s8_bf16, int8x8_t, bfloat16x4_t)

/*
** test_vreinterpret_s8_u32:
** ret
*/
TEST_UNARY (vreinterpret_s8_u32, int8x8_t, uint32x2_t)

/*
** test_vreinterpret_s8_s32:
** ret
*/
TEST_UNARY (vreinterpret_s8_s32, int8x8_t, int32x2_t)

/*
** test_vreinterpret_s8_f32:
** ret
*/
TEST_UNARY (vreinterpret_s8_f32, int8x8_t, float32x2_t)

/*
** test_vreinterpret_s8_u64:
** ret
*/
TEST_UNARY (vreinterpret_s8_u64, int8x8_t, uint64x1_t)

/*
** test_vreinterpret_s8_s64:
** ret
*/
TEST_UNARY (vreinterpret_s8_s64, int8x8_t, int64x1_t)

/*
** test_vreinterpret_s8_p64:
** ret
*/
TEST_UNARY (vreinterpret_s8_p64, int8x8_t, poly64x1_t)

/*
** test_vreinterpret_s8_f64:
** ret
*/
TEST_UNARY (vreinterpret_s8_f64, int8x8_t, float64x1_t)

/*
** test_vreinterpret_p8_u8:
** ret
*/
TEST_UNARY (vreinterpret_p8_u8, poly8x8_t, uint8x8_t)

/*
** test_vreinterpret_p8_s8:
** ret
*/
TEST_UNARY (vreinterpret_p8_s8, poly8x8_t, int8x8_t)

/*
** test_vreinterpret_p8_mf8:
** ret
*/
TEST_UNARY (vreinterpret_p8_mf8, poly8x8_t, mfloat8x8_t)

/*
** test_vreinterpret_p8_u16:
** ret
*/
TEST_UNARY (vreinterpret_p8_u16, poly8x8_t, uint16x4_t)

/*
** test_vreinterpret_p8_s16:
** ret
*/
TEST_UNARY (vreinterpret_p8_s16, poly8x8_t, int16x4_t)

/*
** test_vreinterpret_p8_p16:
** ret
*/
TEST_UNARY (vreinterpret_p8_p16, poly8x8_t, poly16x4_t)

/*
** test_vreinterpret_p8_f16:
** ret
*/
TEST_UNARY (vreinterpret_p8_f16, poly8x8_t, float16x4_t)

/*
** test_vreinterpret_p8_bf16:
** ret
*/
TEST_UNARY (vreinterpret_p8_bf16, poly8x8_t, bfloat16x4_t)

/*
** test_vreinterpret_p8_u32:
** ret
*/
TEST_UNARY (vreinterpret_p8_u32, poly8x8_t, uint32x2_t)

/*
** test_vreinterpret_p8_s32:
** ret
*/
TEST_UNARY (vreinterpret_p8_s32, poly8x8_t, int32x2_t)

/*
** test_vreinterpret_p8_f32:
** ret
*/
TEST_UNARY (vreinterpret_p8_f32, poly8x8_t, float32x2_t)

/*
** test_vreinterpret_p8_u64:
** ret
*/
TEST_UNARY (vreinterpret_p8_u64, poly8x8_t, uint64x1_t)

/*
** test_vreinterpret_p8_s64:
** ret
*/
TEST_UNARY (vreinterpret_p8_s64, poly8x8_t, int64x1_t)

/*
** test_vreinterpret_p8_p64:
** ret
*/
TEST_UNARY (vreinterpret_p8_p64, poly8x8_t, poly64x1_t)

/*
** test_vreinterpret_p8_f64:
** ret
*/
TEST_UNARY (vreinterpret_p8_f64, poly8x8_t, float64x1_t)

/*
** test_vreinterpret_mf8_u8:
** ret
*/
TEST_UNARY (vreinterpret_mf8_u8, mfloat8x8_t, uint8x8_t)

/*
** test_vreinterpret_mf8_s8:
** ret
*/
TEST_UNARY (vreinterpret_mf8_s8, mfloat8x8_t, int8x8_t)

/*
** test_vreinterpret_mf8_p8:
** ret
*/
TEST_UNARY (vreinterpret_mf8_p8, mfloat8x8_t, poly8x8_t)

/*
** test_vreinterpret_mf8_u16:
** ret
*/
TEST_UNARY (vreinterpret_mf8_u16, mfloat8x8_t, uint16x4_t)

/*
** test_vreinterpret_mf8_s16:
** ret
*/
TEST_UNARY (vreinterpret_mf8_s16, mfloat8x8_t, int16x4_t)

/*
** test_vreinterpret_mf8_p16:
** ret
*/
TEST_UNARY (vreinterpret_mf8_p16, mfloat8x8_t, poly16x4_t)

/*
** test_vreinterpret_mf8_f16:
** ret
*/
TEST_UNARY (vreinterpret_mf8_f16, mfloat8x8_t, float16x4_t)

/*
** test_vreinterpret_mf8_bf16:
** ret
*/
TEST_UNARY (vreinterpret_mf8_bf16, mfloat8x8_t, bfloat16x4_t)

/*
** test_vreinterpret_mf8_u32:
** ret
*/
TEST_UNARY (vreinterpret_mf8_u32, mfloat8x8_t, uint32x2_t)

/*
** test_vreinterpret_mf8_s32:
** ret
*/
TEST_UNARY (vreinterpret_mf8_s32, mfloat8x8_t, int32x2_t)

/*
** test_vreinterpret_mf8_f32:
** ret
*/
TEST_UNARY (vreinterpret_mf8_f32, mfloat8x8_t, float32x2_t)

/*
** test_vreinterpret_mf8_u64:
** ret
*/
TEST_UNARY (vreinterpret_mf8_u64, mfloat8x8_t, uint64x1_t)

/*
** test_vreinterpret_mf8_s64:
** ret
*/
TEST_UNARY (vreinterpret_mf8_s64, mfloat8x8_t, int64x1_t)

/*
** test_vreinterpret_mf8_p64:
** ret
*/
TEST_UNARY (vreinterpret_mf8_p64, mfloat8x8_t, poly64x1_t)

/*
** test_vreinterpret_mf8_f64:
** ret
*/
TEST_UNARY (vreinterpret_mf8_f64, mfloat8x8_t, float64x1_t)

/*
** test_vreinterpret_u16_u8:
** ret
*/
TEST_UNARY (vreinterpret_u16_u8, uint16x4_t, uint8x8_t)

/*
** test_vreinterpret_u16_s8:
** ret
*/
TEST_UNARY (vreinterpret_u16_s8, uint16x4_t, int8x8_t)

/*
** test_vreinterpret_u16_p8:
** ret
*/
TEST_UNARY (vreinterpret_u16_p8, uint16x4_t, poly8x8_t)

/*
** test_vreinterpret_u16_mf8:
** ret
*/
TEST_UNARY (vreinterpret_u16_mf8, uint16x4_t, mfloat8x8_t)

/*
** test_vreinterpret_u16_s16:
** ret
*/
TEST_UNARY (vreinterpret_u16_s16, uint16x4_t, int16x4_t)

/*
** test_vreinterpret_u16_p16:
** ret
*/
TEST_UNARY (vreinterpret_u16_p16, uint16x4_t, poly16x4_t)

/*
** test_vreinterpret_u16_f16:
** ret
*/
TEST_UNARY (vreinterpret_u16_f16, uint16x4_t, float16x4_t)

/*
** test_vreinterpret_u16_bf16:
** ret
*/
TEST_UNARY (vreinterpret_u16_bf16, uint16x4_t, bfloat16x4_t)

/*
** test_vreinterpret_u16_u32:
** ret
*/
TEST_UNARY (vreinterpret_u16_u32, uint16x4_t, uint32x2_t)

/*
** test_vreinterpret_u16_s32:
** ret
*/
TEST_UNARY (vreinterpret_u16_s32, uint16x4_t, int32x2_t)

/*
** test_vreinterpret_u16_f32:
** ret
*/
TEST_UNARY (vreinterpret_u16_f32, uint16x4_t, float32x2_t)

/*
** test_vreinterpret_u16_u64:
** ret
*/
TEST_UNARY (vreinterpret_u16_u64, uint16x4_t, uint64x1_t)

/*
** test_vreinterpret_u16_s64:
** ret
*/
TEST_UNARY (vreinterpret_u16_s64, uint16x4_t, int64x1_t)

/*
** test_vreinterpret_u16_p64:
** ret
*/
TEST_UNARY (vreinterpret_u16_p64, uint16x4_t, poly64x1_t)

/*
** test_vreinterpret_u16_f64:
** ret
*/
TEST_UNARY (vreinterpret_u16_f64, uint16x4_t, float64x1_t)

/*
** test_vreinterpret_s16_u8:
** ret
*/
TEST_UNARY (vreinterpret_s16_u8, int16x4_t, uint8x8_t)

/*
** test_vreinterpret_s16_s8:
** ret
*/
TEST_UNARY (vreinterpret_s16_s8, int16x4_t, int8x8_t)

/*
** test_vreinterpret_s16_p8:
** ret
*/
TEST_UNARY (vreinterpret_s16_p8, int16x4_t, poly8x8_t)

/*
** test_vreinterpret_s16_mf8:
** ret
*/
TEST_UNARY (vreinterpret_s16_mf8, int16x4_t, mfloat8x8_t)

/*
** test_vreinterpret_s16_u16:
** ret
*/
TEST_UNARY (vreinterpret_s16_u16, int16x4_t, uint16x4_t)

/*
** test_vreinterpret_s16_p16:
** ret
*/
TEST_UNARY (vreinterpret_s16_p16, int16x4_t, poly16x4_t)

/*
** test_vreinterpret_s16_f16:
** ret
*/
TEST_UNARY (vreinterpret_s16_f16, int16x4_t, float16x4_t)

/*
** test_vreinterpret_s16_bf16:
** ret
*/
TEST_UNARY (vreinterpret_s16_bf16, int16x4_t, bfloat16x4_t)

/*
** test_vreinterpret_s16_u32:
** ret
*/
TEST_UNARY (vreinterpret_s16_u32, int16x4_t, uint32x2_t)

/*
** test_vreinterpret_s16_s32:
** ret
*/
TEST_UNARY (vreinterpret_s16_s32, int16x4_t, int32x2_t)

/*
** test_vreinterpret_s16_f32:
** ret
*/
TEST_UNARY (vreinterpret_s16_f32, int16x4_t, float32x2_t)

/*
** test_vreinterpret_s16_u64:
** ret
*/
TEST_UNARY (vreinterpret_s16_u64, int16x4_t, uint64x1_t)

/*
** test_vreinterpret_s16_s64:
** ret
*/
TEST_UNARY (vreinterpret_s16_s64, int16x4_t, int64x1_t)

/*
** test_vreinterpret_s16_p64:
** ret
*/
TEST_UNARY (vreinterpret_s16_p64, int16x4_t, poly64x1_t)

/*
** test_vreinterpret_s16_f64:
** ret
*/
TEST_UNARY (vreinterpret_s16_f64, int16x4_t, float64x1_t)

/*
** test_vreinterpret_p16_u8:
** ret
*/
TEST_UNARY (vreinterpret_p16_u8, poly16x4_t, uint8x8_t)

/*
** test_vreinterpret_p16_s8:
** ret
*/
TEST_UNARY (vreinterpret_p16_s8, poly16x4_t, int8x8_t)

/*
** test_vreinterpret_p16_p8:
** ret
*/
TEST_UNARY (vreinterpret_p16_p8, poly16x4_t, poly8x8_t)

/*
** test_vreinterpret_p16_mf8:
** ret
*/
TEST_UNARY (vreinterpret_p16_mf8, poly16x4_t, mfloat8x8_t)

/*
** test_vreinterpret_p16_u16:
** ret
*/
TEST_UNARY (vreinterpret_p16_u16, poly16x4_t, uint16x4_t)

/*
** test_vreinterpret_p16_s16:
** ret
*/
TEST_UNARY (vreinterpret_p16_s16, poly16x4_t, int16x4_t)

/*
** test_vreinterpret_p16_f16:
** ret
*/
TEST_UNARY (vreinterpret_p16_f16, poly16x4_t, float16x4_t)

/*
** test_vreinterpret_p16_bf16:
** ret
*/
TEST_UNARY (vreinterpret_p16_bf16, poly16x4_t, bfloat16x4_t)

/*
** test_vreinterpret_p16_u32:
** ret
*/
TEST_UNARY (vreinterpret_p16_u32, poly16x4_t, uint32x2_t)

/*
** test_vreinterpret_p16_s32:
** ret
*/
TEST_UNARY (vreinterpret_p16_s32, poly16x4_t, int32x2_t)

/*
** test_vreinterpret_p16_f32:
** ret
*/
TEST_UNARY (vreinterpret_p16_f32, poly16x4_t, float32x2_t)

/*
** test_vreinterpret_p16_u64:
** ret
*/
TEST_UNARY (vreinterpret_p16_u64, poly16x4_t, uint64x1_t)

/*
** test_vreinterpret_p16_s64:
** ret
*/
TEST_UNARY (vreinterpret_p16_s64, poly16x4_t, int64x1_t)

/*
** test_vreinterpret_p16_p64:
** ret
*/
TEST_UNARY (vreinterpret_p16_p64, poly16x4_t, poly64x1_t)

/*
** test_vreinterpret_p16_f64:
** ret
*/
TEST_UNARY (vreinterpret_p16_f64, poly16x4_t, float64x1_t)

/*
** test_vreinterpret_f16_u8:
** ret
*/
TEST_UNARY (vreinterpret_f16_u8, float16x4_t, uint8x8_t)

/*
** test_vreinterpret_f16_s8:
** ret
*/
TEST_UNARY (vreinterpret_f16_s8, float16x4_t, int8x8_t)

/*
** test_vreinterpret_f16_p8:
** ret
*/
TEST_UNARY (vreinterpret_f16_p8, float16x4_t, poly8x8_t)

/*
** test_vreinterpret_f16_mf8:
** ret
*/
TEST_UNARY (vreinterpret_f16_mf8, float16x4_t, mfloat8x8_t)

/*
** test_vreinterpret_f16_u16:
** ret
*/
TEST_UNARY (vreinterpret_f16_u16, float16x4_t, uint16x4_t)

/*
** test_vreinterpret_f16_s16:
** ret
*/
TEST_UNARY (vreinterpret_f16_s16, float16x4_t, int16x4_t)

/*
** test_vreinterpret_f16_p16:
** ret
*/
TEST_UNARY (vreinterpret_f16_p16, float16x4_t, poly16x4_t)

/*
** test_vreinterpret_f16_bf16:
** ret
*/
TEST_UNARY (vreinterpret_f16_bf16, float16x4_t, bfloat16x4_t)

/*
** test_vreinterpret_f16_u32:
** ret
*/
TEST_UNARY (vreinterpret_f16_u32, float16x4_t, uint32x2_t)

/*
** test_vreinterpret_f16_s32:
** ret
*/
TEST_UNARY (vreinterpret_f16_s32, float16x4_t, int32x2_t)

/*
** test_vreinterpret_f16_f32:
** ret
*/
TEST_UNARY (vreinterpret_f16_f32, float16x4_t, float32x2_t)

/*
** test_vreinterpret_f16_u64:
** ret
*/
TEST_UNARY (vreinterpret_f16_u64, float16x4_t, uint64x1_t)

/*
** test_vreinterpret_f16_s64:
** ret
*/
TEST_UNARY (vreinterpret_f16_s64, float16x4_t, int64x1_t)

/*
** test_vreinterpret_f16_p64:
** ret
*/
TEST_UNARY (vreinterpret_f16_p64, float16x4_t, poly64x1_t)

/*
** test_vreinterpret_f16_f64:
** ret
*/
TEST_UNARY (vreinterpret_f16_f64, float16x4_t, float64x1_t)

/*
** test_vreinterpret_bf16_u8:
** ret
*/
TEST_UNARY (vreinterpret_bf16_u8, bfloat16x4_t, uint8x8_t)

/*
** test_vreinterpret_bf16_s8:
** ret
*/
TEST_UNARY (vreinterpret_bf16_s8, bfloat16x4_t, int8x8_t)

/*
** test_vreinterpret_bf16_p8:
** ret
*/
TEST_UNARY (vreinterpret_bf16_p8, bfloat16x4_t, poly8x8_t)

/*
** test_vreinterpret_bf16_mf8:
** ret
*/
TEST_UNARY (vreinterpret_bf16_mf8, bfloat16x4_t, mfloat8x8_t)

/*
** test_vreinterpret_bf16_u16:
** ret
*/
TEST_UNARY (vreinterpret_bf16_u16, bfloat16x4_t, uint16x4_t)

/*
** test_vreinterpret_bf16_s16:
** ret
*/
TEST_UNARY (vreinterpret_bf16_s16, bfloat16x4_t, int16x4_t)

/*
** test_vreinterpret_bf16_p16:
** ret
*/
TEST_UNARY (vreinterpret_bf16_p16, bfloat16x4_t, poly16x4_t)

/*
** test_vreinterpret_bf16_f16:
** ret
*/
TEST_UNARY (vreinterpret_bf16_f16, bfloat16x4_t, float16x4_t)

/*
** test_vreinterpret_bf16_u32:
** ret
*/
TEST_UNARY (vreinterpret_bf16_u32, bfloat16x4_t, uint32x2_t)

/*
** test_vreinterpret_bf16_s32:
** ret
*/
TEST_UNARY (vreinterpret_bf16_s32, bfloat16x4_t, int32x2_t)

/*
** test_vreinterpret_bf16_f32:
** ret
*/
TEST_UNARY (vreinterpret_bf16_f32, bfloat16x4_t, float32x2_t)

/*
** test_vreinterpret_bf16_u64:
** ret
*/
TEST_UNARY (vreinterpret_bf16_u64, bfloat16x4_t, uint64x1_t)

/*
** test_vreinterpret_bf16_s64:
** ret
*/
TEST_UNARY (vreinterpret_bf16_s64, bfloat16x4_t, int64x1_t)

/*
** test_vreinterpret_bf16_p64:
** ret
*/
TEST_UNARY (vreinterpret_bf16_p64, bfloat16x4_t, poly64x1_t)

/*
** test_vreinterpret_bf16_f64:
** ret
*/
TEST_UNARY (vreinterpret_bf16_f64, bfloat16x4_t, float64x1_t)

/*
** test_vreinterpret_u32_u8:
** ret
*/
TEST_UNARY (vreinterpret_u32_u8, uint32x2_t, uint8x8_t)

/*
** test_vreinterpret_u32_s8:
** ret
*/
TEST_UNARY (vreinterpret_u32_s8, uint32x2_t, int8x8_t)

/*
** test_vreinterpret_u32_p8:
** ret
*/
TEST_UNARY (vreinterpret_u32_p8, uint32x2_t, poly8x8_t)

/*
** test_vreinterpret_u32_mf8:
** ret
*/
TEST_UNARY (vreinterpret_u32_mf8, uint32x2_t, mfloat8x8_t)

/*
** test_vreinterpret_u32_u16:
** ret
*/
TEST_UNARY (vreinterpret_u32_u16, uint32x2_t, uint16x4_t)

/*
** test_vreinterpret_u32_s16:
** ret
*/
TEST_UNARY (vreinterpret_u32_s16, uint32x2_t, int16x4_t)

/*
** test_vreinterpret_u32_p16:
** ret
*/
TEST_UNARY (vreinterpret_u32_p16, uint32x2_t, poly16x4_t)

/*
** test_vreinterpret_u32_f16:
** ret
*/
TEST_UNARY (vreinterpret_u32_f16, uint32x2_t, float16x4_t)

/*
** test_vreinterpret_u32_bf16:
** ret
*/
TEST_UNARY (vreinterpret_u32_bf16, uint32x2_t, bfloat16x4_t)

/*
** test_vreinterpret_u32_s32:
** ret
*/
TEST_UNARY (vreinterpret_u32_s32, uint32x2_t, int32x2_t)

/*
** test_vreinterpret_u32_f32:
** ret
*/
TEST_UNARY (vreinterpret_u32_f32, uint32x2_t, float32x2_t)

/*
** test_vreinterpret_u32_u64:
** ret
*/
TEST_UNARY (vreinterpret_u32_u64, uint32x2_t, uint64x1_t)

/*
** test_vreinterpret_u32_s64:
** ret
*/
TEST_UNARY (vreinterpret_u32_s64, uint32x2_t, int64x1_t)

/*
** test_vreinterpret_u32_p64:
** ret
*/
TEST_UNARY (vreinterpret_u32_p64, uint32x2_t, poly64x1_t)

/*
** test_vreinterpret_u32_f64:
** ret
*/
TEST_UNARY (vreinterpret_u32_f64, uint32x2_t, float64x1_t)

/*
** test_vreinterpret_s32_u8:
** ret
*/
TEST_UNARY (vreinterpret_s32_u8, int32x2_t, uint8x8_t)

/*
** test_vreinterpret_s32_s8:
** ret
*/
TEST_UNARY (vreinterpret_s32_s8, int32x2_t, int8x8_t)

/*
** test_vreinterpret_s32_p8:
** ret
*/
TEST_UNARY (vreinterpret_s32_p8, int32x2_t, poly8x8_t)

/*
** test_vreinterpret_s32_mf8:
** ret
*/
TEST_UNARY (vreinterpret_s32_mf8, int32x2_t, mfloat8x8_t)

/*
** test_vreinterpret_s32_u16:
** ret
*/
TEST_UNARY (vreinterpret_s32_u16, int32x2_t, uint16x4_t)

/*
** test_vreinterpret_s32_s16:
** ret
*/
TEST_UNARY (vreinterpret_s32_s16, int32x2_t, int16x4_t)

/*
** test_vreinterpret_s32_p16:
** ret
*/
TEST_UNARY (vreinterpret_s32_p16, int32x2_t, poly16x4_t)

/*
** test_vreinterpret_s32_f16:
** ret
*/
TEST_UNARY (vreinterpret_s32_f16, int32x2_t, float16x4_t)

/*
** test_vreinterpret_s32_bf16:
** ret
*/
TEST_UNARY (vreinterpret_s32_bf16, int32x2_t, bfloat16x4_t)

/*
** test_vreinterpret_s32_u32:
** ret
*/
TEST_UNARY (vreinterpret_s32_u32, int32x2_t, uint32x2_t)

/*
** test_vreinterpret_s32_f32:
** ret
*/
TEST_UNARY (vreinterpret_s32_f32, int32x2_t, float32x2_t)

/*
** test_vreinterpret_s32_u64:
** ret
*/
TEST_UNARY (vreinterpret_s32_u64, int32x2_t, uint64x1_t)

/*
** test_vreinterpret_s32_s64:
** ret
*/
TEST_UNARY (vreinterpret_s32_s64, int32x2_t, int64x1_t)

/*
** test_vreinterpret_s32_p64:
** ret
*/
TEST_UNARY (vreinterpret_s32_p64, int32x2_t, poly64x1_t)

/*
** test_vreinterpret_s32_f64:
** ret
*/
TEST_UNARY (vreinterpret_s32_f64, int32x2_t, float64x1_t)

/*
** test_vreinterpret_f32_u8:
** ret
*/
TEST_UNARY (vreinterpret_f32_u8, float32x2_t, uint8x8_t)

/*
** test_vreinterpret_f32_s8:
** ret
*/
TEST_UNARY (vreinterpret_f32_s8, float32x2_t, int8x8_t)

/*
** test_vreinterpret_f32_p8:
** ret
*/
TEST_UNARY (vreinterpret_f32_p8, float32x2_t, poly8x8_t)

/*
** test_vreinterpret_f32_mf8:
** ret
*/
TEST_UNARY (vreinterpret_f32_mf8, float32x2_t, mfloat8x8_t)

/*
** test_vreinterpret_f32_u16:
** ret
*/
TEST_UNARY (vreinterpret_f32_u16, float32x2_t, uint16x4_t)

/*
** test_vreinterpret_f32_s16:
** ret
*/
TEST_UNARY (vreinterpret_f32_s16, float32x2_t, int16x4_t)

/*
** test_vreinterpret_f32_p16:
** ret
*/
TEST_UNARY (vreinterpret_f32_p16, float32x2_t, poly16x4_t)

/*
** test_vreinterpret_f32_f16:
** ret
*/
TEST_UNARY (vreinterpret_f32_f16, float32x2_t, float16x4_t)

/*
** test_vreinterpret_f32_bf16:
** ret
*/
TEST_UNARY (vreinterpret_f32_bf16, float32x2_t, bfloat16x4_t)

/*
** test_vreinterpret_f32_u32:
** ret
*/
TEST_UNARY (vreinterpret_f32_u32, float32x2_t, uint32x2_t)

/*
** test_vreinterpret_f32_s32:
** ret
*/
TEST_UNARY (vreinterpret_f32_s32, float32x2_t, int32x2_t)

/*
** test_vreinterpret_f32_u64:
** ret
*/
TEST_UNARY (vreinterpret_f32_u64, float32x2_t, uint64x1_t)

/*
** test_vreinterpret_f32_s64:
** ret
*/
TEST_UNARY (vreinterpret_f32_s64, float32x2_t, int64x1_t)

/*
** test_vreinterpret_f32_p64:
** ret
*/
TEST_UNARY (vreinterpret_f32_p64, float32x2_t, poly64x1_t)

/*
** test_vreinterpret_f32_f64:
** ret
*/
TEST_UNARY (vreinterpret_f32_f64, float32x2_t, float64x1_t)

/*
** test_vreinterpret_u64_u8:
** ret
*/
TEST_UNARY (vreinterpret_u64_u8, uint64x1_t, uint8x8_t)

/*
** test_vreinterpret_u64_s8:
** ret
*/
TEST_UNARY (vreinterpret_u64_s8, uint64x1_t, int8x8_t)

/*
** test_vreinterpret_u64_p8:
** ret
*/
TEST_UNARY (vreinterpret_u64_p8, uint64x1_t, poly8x8_t)

/*
** test_vreinterpret_u64_mf8:
** ret
*/
TEST_UNARY (vreinterpret_u64_mf8, uint64x1_t, mfloat8x8_t)

/*
** test_vreinterpret_u64_u16:
** ret
*/
TEST_UNARY (vreinterpret_u64_u16, uint64x1_t, uint16x4_t)

/*
** test_vreinterpret_u64_s16:
** ret
*/
TEST_UNARY (vreinterpret_u64_s16, uint64x1_t, int16x4_t)

/*
** test_vreinterpret_u64_p16:
** ret
*/
TEST_UNARY (vreinterpret_u64_p16, uint64x1_t, poly16x4_t)

/*
** test_vreinterpret_u64_f16:
** ret
*/
TEST_UNARY (vreinterpret_u64_f16, uint64x1_t, float16x4_t)

/*
** test_vreinterpret_u64_bf16:
** ret
*/
TEST_UNARY (vreinterpret_u64_bf16, uint64x1_t, bfloat16x4_t)

/*
** test_vreinterpret_u64_u32:
** ret
*/
TEST_UNARY (vreinterpret_u64_u32, uint64x1_t, uint32x2_t)

/*
** test_vreinterpret_u64_s32:
** ret
*/
TEST_UNARY (vreinterpret_u64_s32, uint64x1_t, int32x2_t)

/*
** test_vreinterpret_u64_f32:
** ret
*/
TEST_UNARY (vreinterpret_u64_f32, uint64x1_t, float32x2_t)

/*
** test_vreinterpret_u64_s64:
** ret
*/
TEST_UNARY (vreinterpret_u64_s64, uint64x1_t, int64x1_t)

/*
** test_vreinterpret_u64_p64:
** ret
*/
TEST_UNARY (vreinterpret_u64_p64, uint64x1_t, poly64x1_t)

/*
** test_vreinterpret_u64_f64:
** ret
*/
TEST_UNARY (vreinterpret_u64_f64, uint64x1_t, float64x1_t)

/*
** test_vreinterpret_s64_u8:
** ret
*/
TEST_UNARY (vreinterpret_s64_u8, int64x1_t, uint8x8_t)

/*
** test_vreinterpret_s64_s8:
** ret
*/
TEST_UNARY (vreinterpret_s64_s8, int64x1_t, int8x8_t)

/*
** test_vreinterpret_s64_p8:
** ret
*/
TEST_UNARY (vreinterpret_s64_p8, int64x1_t, poly8x8_t)

/*
** test_vreinterpret_s64_mf8:
** ret
*/
TEST_UNARY (vreinterpret_s64_mf8, int64x1_t, mfloat8x8_t)

/*
** test_vreinterpret_s64_u16:
** ret
*/
TEST_UNARY (vreinterpret_s64_u16, int64x1_t, uint16x4_t)

/*
** test_vreinterpret_s64_s16:
** ret
*/
TEST_UNARY (vreinterpret_s64_s16, int64x1_t, int16x4_t)

/*
** test_vreinterpret_s64_p16:
** ret
*/
TEST_UNARY (vreinterpret_s64_p16, int64x1_t, poly16x4_t)

/*
** test_vreinterpret_s64_f16:
** ret
*/
TEST_UNARY (vreinterpret_s64_f16, int64x1_t, float16x4_t)

/*
** test_vreinterpret_s64_bf16:
** ret
*/
TEST_UNARY (vreinterpret_s64_bf16, int64x1_t, bfloat16x4_t)

/*
** test_vreinterpret_s64_u32:
** ret
*/
TEST_UNARY (vreinterpret_s64_u32, int64x1_t, uint32x2_t)

/*
** test_vreinterpret_s64_s32:
** ret
*/
TEST_UNARY (vreinterpret_s64_s32, int64x1_t, int32x2_t)

/*
** test_vreinterpret_s64_f32:
** ret
*/
TEST_UNARY (vreinterpret_s64_f32, int64x1_t, float32x2_t)

/*
** test_vreinterpret_s64_u64:
** ret
*/
TEST_UNARY (vreinterpret_s64_u64, int64x1_t, uint64x1_t)

/*
** test_vreinterpret_s64_p64:
** ret
*/
TEST_UNARY (vreinterpret_s64_p64, int64x1_t, poly64x1_t)

/*
** test_vreinterpret_s64_f64:
** ret
*/
TEST_UNARY (vreinterpret_s64_f64, int64x1_t, float64x1_t)

/*
** test_vreinterpret_p64_u8:
** ret
*/
TEST_UNARY (vreinterpret_p64_u8, poly64x1_t, uint8x8_t)

/*
** test_vreinterpret_p64_s8:
** ret
*/
TEST_UNARY (vreinterpret_p64_s8, poly64x1_t, int8x8_t)

/*
** test_vreinterpret_p64_p8:
** ret
*/
TEST_UNARY (vreinterpret_p64_p8, poly64x1_t, poly8x8_t)

/*
** test_vreinterpret_p64_mf8:
** ret
*/
TEST_UNARY (vreinterpret_p64_mf8, poly64x1_t, mfloat8x8_t)

/*
** test_vreinterpret_p64_u16:
** ret
*/
TEST_UNARY (vreinterpret_p64_u16, poly64x1_t, uint16x4_t)

/*
** test_vreinterpret_p64_s16:
** ret
*/
TEST_UNARY (vreinterpret_p64_s16, poly64x1_t, int16x4_t)

/*
** test_vreinterpret_p64_p16:
** ret
*/
TEST_UNARY (vreinterpret_p64_p16, poly64x1_t, poly16x4_t)

/*
** test_vreinterpret_p64_f16:
** ret
*/
TEST_UNARY (vreinterpret_p64_f16, poly64x1_t, float16x4_t)

/*
** test_vreinterpret_p64_bf16:
** ret
*/
TEST_UNARY (vreinterpret_p64_bf16, poly64x1_t, bfloat16x4_t)

/*
** test_vreinterpret_p64_u32:
** ret
*/
TEST_UNARY (vreinterpret_p64_u32, poly64x1_t, uint32x2_t)

/*
** test_vreinterpret_p64_s32:
** ret
*/
TEST_UNARY (vreinterpret_p64_s32, poly64x1_t, int32x2_t)

/*
** test_vreinterpret_p64_f32:
** ret
*/
TEST_UNARY (vreinterpret_p64_f32, poly64x1_t, float32x2_t)

/*
** test_vreinterpret_p64_u64:
** ret
*/
TEST_UNARY (vreinterpret_p64_u64, poly64x1_t, uint64x1_t)

/*
** test_vreinterpret_p64_s64:
** ret
*/
TEST_UNARY (vreinterpret_p64_s64, poly64x1_t, int64x1_t)

/*
** test_vreinterpret_p64_f64:
** ret
*/
TEST_UNARY (vreinterpret_p64_f64, poly64x1_t, float64x1_t)

/*
** test_vreinterpret_f64_u8:
** ret
*/
TEST_UNARY (vreinterpret_f64_u8, float64x1_t, uint8x8_t)

/*
** test_vreinterpret_f64_s8:
** ret
*/
TEST_UNARY (vreinterpret_f64_s8, float64x1_t, int8x8_t)

/*
** test_vreinterpret_f64_p8:
** ret
*/
TEST_UNARY (vreinterpret_f64_p8, float64x1_t, poly8x8_t)

/*
** test_vreinterpret_f64_mf8:
** ret
*/
TEST_UNARY (vreinterpret_f64_mf8, float64x1_t, mfloat8x8_t)

/*
** test_vreinterpret_f64_u16:
** ret
*/
TEST_UNARY (vreinterpret_f64_u16, float64x1_t, uint16x4_t)

/*
** test_vreinterpret_f64_s16:
** ret
*/
TEST_UNARY (vreinterpret_f64_s16, float64x1_t, int16x4_t)

/*
** test_vreinterpret_f64_p16:
** ret
*/
TEST_UNARY (vreinterpret_f64_p16, float64x1_t, poly16x4_t)

/*
** test_vreinterpret_f64_f16:
** ret
*/
TEST_UNARY (vreinterpret_f64_f16, float64x1_t, float16x4_t)

/*
** test_vreinterpret_f64_bf16:
** ret
*/
TEST_UNARY (vreinterpret_f64_bf16, float64x1_t, bfloat16x4_t)

/*
** test_vreinterpret_f64_u32:
** ret
*/
TEST_UNARY (vreinterpret_f64_u32, float64x1_t, uint32x2_t)

/*
** test_vreinterpret_f64_s32:
** ret
*/
TEST_UNARY (vreinterpret_f64_s32, float64x1_t, int32x2_t)

/*
** test_vreinterpret_f64_f32:
** ret
*/
TEST_UNARY (vreinterpret_f64_f32, float64x1_t, float32x2_t)

/*
** test_vreinterpret_f64_u64:
** ret
*/
TEST_UNARY (vreinterpret_f64_u64, float64x1_t, uint64x1_t)

/*
** test_vreinterpret_f64_s64:
** ret
*/
TEST_UNARY (vreinterpret_f64_s64, float64x1_t, int64x1_t)

/*
** test_vreinterpret_f64_p64:
** ret
*/
TEST_UNARY (vreinterpret_f64_p64, float64x1_t, poly64x1_t)

/*
** test_vreinterpretq_u8_s8:
** ret
*/
TEST_UNARY (vreinterpretq_u8_s8, uint8x16_t, int8x16_t)

/*
** test_vreinterpretq_u8_p8:
** ret
*/
TEST_UNARY (vreinterpretq_u8_p8, uint8x16_t, poly8x16_t)

/*
** test_vreinterpretq_u8_mf8:
** ret
*/
TEST_UNARY (vreinterpretq_u8_mf8, uint8x16_t, mfloat8x16_t)

/*
** test_vreinterpretq_u8_u16:
** ret
*/
TEST_UNARY (vreinterpretq_u8_u16, uint8x16_t, uint16x8_t)

/*
** test_vreinterpretq_u8_s16:
** ret
*/
TEST_UNARY (vreinterpretq_u8_s16, uint8x16_t, int16x8_t)

/*
** test_vreinterpretq_u8_p16:
** ret
*/
TEST_UNARY (vreinterpretq_u8_p16, uint8x16_t, poly16x8_t)

/*
** test_vreinterpretq_u8_f16:
** ret
*/
TEST_UNARY (vreinterpretq_u8_f16, uint8x16_t, float16x8_t)

/*
** test_vreinterpretq_u8_bf16:
** ret
*/
TEST_UNARY (vreinterpretq_u8_bf16, uint8x16_t, bfloat16x8_t)

/*
** test_vreinterpretq_u8_u32:
** ret
*/
TEST_UNARY (vreinterpretq_u8_u32, uint8x16_t, uint32x4_t)

/*
** test_vreinterpretq_u8_s32:
** ret
*/
TEST_UNARY (vreinterpretq_u8_s32, uint8x16_t, int32x4_t)

/*
** test_vreinterpretq_u8_f32:
** ret
*/
TEST_UNARY (vreinterpretq_u8_f32, uint8x16_t, float32x4_t)

/*
** test_vreinterpretq_u8_u64:
** ret
*/
TEST_UNARY (vreinterpretq_u8_u64, uint8x16_t, uint64x2_t)

/*
** test_vreinterpretq_u8_s64:
** ret
*/
TEST_UNARY (vreinterpretq_u8_s64, uint8x16_t, int64x2_t)

/*
** test_vreinterpretq_u8_p64:
** ret
*/
TEST_UNARY (vreinterpretq_u8_p64, uint8x16_t, poly64x2_t)

/*
** test_vreinterpretq_u8_p128: { xfail *-*-* }
** fmov	d0, x0
** mov	v0\.d\[1\], x1
** ret
*/
TEST_UNARY (vreinterpretq_u8_p128, uint8x16_t, poly128_t)

/*
** test_vreinterpretq_u8_f64:
** ret
*/
TEST_UNARY (vreinterpretq_u8_f64, uint8x16_t, float64x2_t)

/*
** test_vreinterpretq_s8_u8:
** ret
*/
TEST_UNARY (vreinterpretq_s8_u8, int8x16_t, uint8x16_t)

/*
** test_vreinterpretq_s8_p8:
** ret
*/
TEST_UNARY (vreinterpretq_s8_p8, int8x16_t, poly8x16_t)

/*
** test_vreinterpretq_s8_mf8:
** ret
*/
TEST_UNARY (vreinterpretq_s8_mf8, int8x16_t, mfloat8x16_t)

/*
** test_vreinterpretq_s8_u16:
** ret
*/
TEST_UNARY (vreinterpretq_s8_u16, int8x16_t, uint16x8_t)

/*
** test_vreinterpretq_s8_s16:
** ret
*/
TEST_UNARY (vreinterpretq_s8_s16, int8x16_t, int16x8_t)

/*
** test_vreinterpretq_s8_p16:
** ret
*/
TEST_UNARY (vreinterpretq_s8_p16, int8x16_t, poly16x8_t)

/*
** test_vreinterpretq_s8_f16:
** ret
*/
TEST_UNARY (vreinterpretq_s8_f16, int8x16_t, float16x8_t)

/*
** test_vreinterpretq_s8_bf16:
** ret
*/
TEST_UNARY (vreinterpretq_s8_bf16, int8x16_t, bfloat16x8_t)

/*
** test_vreinterpretq_s8_u32:
** ret
*/
TEST_UNARY (vreinterpretq_s8_u32, int8x16_t, uint32x4_t)

/*
** test_vreinterpretq_s8_s32:
** ret
*/
TEST_UNARY (vreinterpretq_s8_s32, int8x16_t, int32x4_t)

/*
** test_vreinterpretq_s8_f32:
** ret
*/
TEST_UNARY (vreinterpretq_s8_f32, int8x16_t, float32x4_t)

/*
** test_vreinterpretq_s8_u64:
** ret
*/
TEST_UNARY (vreinterpretq_s8_u64, int8x16_t, uint64x2_t)

/*
** test_vreinterpretq_s8_s64:
** ret
*/
TEST_UNARY (vreinterpretq_s8_s64, int8x16_t, int64x2_t)

/*
** test_vreinterpretq_s8_p64:
** ret
*/
TEST_UNARY (vreinterpretq_s8_p64, int8x16_t, poly64x2_t)

/*
** test_vreinterpretq_s8_p128: { xfail *-*-* }
** fmov	d0, x0
** mov	v0\.d\[1\], x1
** ret
*/
TEST_UNARY (vreinterpretq_s8_p128, int8x16_t, poly128_t)

/*
** test_vreinterpretq_s8_f64:
** ret
*/
TEST_UNARY (vreinterpretq_s8_f64, int8x16_t, float64x2_t)

/*
** test_vreinterpretq_p8_u8:
** ret
*/
TEST_UNARY (vreinterpretq_p8_u8, poly8x16_t, uint8x16_t)

/*
** test_vreinterpretq_p8_s8:
** ret
*/
TEST_UNARY (vreinterpretq_p8_s8, poly8x16_t, int8x16_t)

/*
** test_vreinterpretq_p8_mf8:
** ret
*/
TEST_UNARY (vreinterpretq_p8_mf8, poly8x16_t, mfloat8x16_t)

/*
** test_vreinterpretq_p8_u16:
** ret
*/
TEST_UNARY (vreinterpretq_p8_u16, poly8x16_t, uint16x8_t)

/*
** test_vreinterpretq_p8_s16:
** ret
*/
TEST_UNARY (vreinterpretq_p8_s16, poly8x16_t, int16x8_t)

/*
** test_vreinterpretq_p8_p16:
** ret
*/
TEST_UNARY (vreinterpretq_p8_p16, poly8x16_t, poly16x8_t)

/*
** test_vreinterpretq_p8_f16:
** ret
*/
TEST_UNARY (vreinterpretq_p8_f16, poly8x16_t, float16x8_t)

/*
** test_vreinterpretq_p8_bf16:
** ret
*/
TEST_UNARY (vreinterpretq_p8_bf16, poly8x16_t, bfloat16x8_t)

/*
** test_vreinterpretq_p8_u32:
** ret
*/
TEST_UNARY (vreinterpretq_p8_u32, poly8x16_t, uint32x4_t)

/*
** test_vreinterpretq_p8_s32:
** ret
*/
TEST_UNARY (vreinterpretq_p8_s32, poly8x16_t, int32x4_t)

/*
** test_vreinterpretq_p8_f32:
** ret
*/
TEST_UNARY (vreinterpretq_p8_f32, poly8x16_t, float32x4_t)

/*
** test_vreinterpretq_p8_u64:
** ret
*/
TEST_UNARY (vreinterpretq_p8_u64, poly8x16_t, uint64x2_t)

/*
** test_vreinterpretq_p8_s64:
** ret
*/
TEST_UNARY (vreinterpretq_p8_s64, poly8x16_t, int64x2_t)

/*
** test_vreinterpretq_p8_p64:
** ret
*/
TEST_UNARY (vreinterpretq_p8_p64, poly8x16_t, poly64x2_t)

/*
** test_vreinterpretq_p8_p128: { xfail *-*-* }
** fmov	d0, x0
** mov	v0\.d\[1\], x1
** ret
*/
TEST_UNARY (vreinterpretq_p8_p128, poly8x16_t, poly128_t)

/*
** test_vreinterpretq_p8_f64:
** ret
*/
TEST_UNARY (vreinterpretq_p8_f64, poly8x16_t, float64x2_t)

/*
** test_vreinterpretq_mf8_u8:
** ret
*/
TEST_UNARY (vreinterpretq_mf8_u8, mfloat8x16_t, uint8x16_t)

/*
** test_vreinterpretq_mf8_s8:
** ret
*/
TEST_UNARY (vreinterpretq_mf8_s8, mfloat8x16_t, int8x16_t)

/*
** test_vreinterpretq_mf8_p8:
** ret
*/
TEST_UNARY (vreinterpretq_mf8_p8, mfloat8x16_t, poly8x16_t)

/*
** test_vreinterpretq_mf8_u16:
** ret
*/
TEST_UNARY (vreinterpretq_mf8_u16, mfloat8x16_t, uint16x8_t)

/*
** test_vreinterpretq_mf8_s16:
** ret
*/
TEST_UNARY (vreinterpretq_mf8_s16, mfloat8x16_t, int16x8_t)

/*
** test_vreinterpretq_mf8_p16:
** ret
*/
TEST_UNARY (vreinterpretq_mf8_p16, mfloat8x16_t, poly16x8_t)

/*
** test_vreinterpretq_mf8_f16:
** ret
*/
TEST_UNARY (vreinterpretq_mf8_f16, mfloat8x16_t, float16x8_t)

/*
** test_vreinterpretq_mf8_bf16:
** ret
*/
TEST_UNARY (vreinterpretq_mf8_bf16, mfloat8x16_t, bfloat16x8_t)

/*
** test_vreinterpretq_mf8_u32:
** ret
*/
TEST_UNARY (vreinterpretq_mf8_u32, mfloat8x16_t, uint32x4_t)

/*
** test_vreinterpretq_mf8_s32:
** ret
*/
TEST_UNARY (vreinterpretq_mf8_s32, mfloat8x16_t, int32x4_t)

/*
** test_vreinterpretq_mf8_f32:
** ret
*/
TEST_UNARY (vreinterpretq_mf8_f32, mfloat8x16_t, float32x4_t)

/*
** test_vreinterpretq_mf8_u64:
** ret
*/
TEST_UNARY (vreinterpretq_mf8_u64, mfloat8x16_t, uint64x2_t)

/*
** test_vreinterpretq_mf8_s64:
** ret
*/
TEST_UNARY (vreinterpretq_mf8_s64, mfloat8x16_t, int64x2_t)

/*
** test_vreinterpretq_mf8_p64:
** ret
*/
TEST_UNARY (vreinterpretq_mf8_p64, mfloat8x16_t, poly64x2_t)

/*
** test_vreinterpretq_mf8_p128: { xfail *-*-* }
** fmov	d0, x0
** mov	v0\.d\[1\], x1
** ret
*/
TEST_UNARY (vreinterpretq_mf8_p128, mfloat8x16_t, poly128_t)

/*
** test_vreinterpretq_mf8_f64:
** ret
*/
TEST_UNARY (vreinterpretq_mf8_f64, mfloat8x16_t, float64x2_t)

/*
** test_vreinterpretq_u16_u8:
** ret
*/
TEST_UNARY (vreinterpretq_u16_u8, uint16x8_t, uint8x16_t)

/*
** test_vreinterpretq_u16_s8:
** ret
*/
TEST_UNARY (vreinterpretq_u16_s8, uint16x8_t, int8x16_t)

/*
** test_vreinterpretq_u16_p8:
** ret
*/
TEST_UNARY (vreinterpretq_u16_p8, uint16x8_t, poly8x16_t)

/*
** test_vreinterpretq_u16_mf8:
** ret
*/
TEST_UNARY (vreinterpretq_u16_mf8, uint16x8_t, mfloat8x16_t)

/*
** test_vreinterpretq_u16_s16:
** ret
*/
TEST_UNARY (vreinterpretq_u16_s16, uint16x8_t, int16x8_t)

/*
** test_vreinterpretq_u16_p16:
** ret
*/
TEST_UNARY (vreinterpretq_u16_p16, uint16x8_t, poly16x8_t)

/*
** test_vreinterpretq_u16_f16:
** ret
*/
TEST_UNARY (vreinterpretq_u16_f16, uint16x8_t, float16x8_t)

/*
** test_vreinterpretq_u16_bf16:
** ret
*/
TEST_UNARY (vreinterpretq_u16_bf16, uint16x8_t, bfloat16x8_t)

/*
** test_vreinterpretq_u16_u32:
** ret
*/
TEST_UNARY (vreinterpretq_u16_u32, uint16x8_t, uint32x4_t)

/*
** test_vreinterpretq_u16_s32:
** ret
*/
TEST_UNARY (vreinterpretq_u16_s32, uint16x8_t, int32x4_t)

/*
** test_vreinterpretq_u16_f32:
** ret
*/
TEST_UNARY (vreinterpretq_u16_f32, uint16x8_t, float32x4_t)

/*
** test_vreinterpretq_u16_u64:
** ret
*/
TEST_UNARY (vreinterpretq_u16_u64, uint16x8_t, uint64x2_t)

/*
** test_vreinterpretq_u16_s64:
** ret
*/
TEST_UNARY (vreinterpretq_u16_s64, uint16x8_t, int64x2_t)

/*
** test_vreinterpretq_u16_p64:
** ret
*/
TEST_UNARY (vreinterpretq_u16_p64, uint16x8_t, poly64x2_t)

/*
** test_vreinterpretq_u16_p128: { xfail *-*-* }
** fmov	d0, x0
** mov	v0\.d\[1\], x1
** ret
*/
TEST_UNARY (vreinterpretq_u16_p128, uint16x8_t, poly128_t)

/*
** test_vreinterpretq_u16_f64:
** ret
*/
TEST_UNARY (vreinterpretq_u16_f64, uint16x8_t, float64x2_t)

/*
** test_vreinterpretq_s16_u8:
** ret
*/
TEST_UNARY (vreinterpretq_s16_u8, int16x8_t, uint8x16_t)

/*
** test_vreinterpretq_s16_s8:
** ret
*/
TEST_UNARY (vreinterpretq_s16_s8, int16x8_t, int8x16_t)

/*
** test_vreinterpretq_s16_p8:
** ret
*/
TEST_UNARY (vreinterpretq_s16_p8, int16x8_t, poly8x16_t)

/*
** test_vreinterpretq_s16_mf8:
** ret
*/
TEST_UNARY (vreinterpretq_s16_mf8, int16x8_t, mfloat8x16_t)

/*
** test_vreinterpretq_s16_u16:
** ret
*/
TEST_UNARY (vreinterpretq_s16_u16, int16x8_t, uint16x8_t)

/*
** test_vreinterpretq_s16_p16:
** ret
*/
TEST_UNARY (vreinterpretq_s16_p16, int16x8_t, poly16x8_t)

/*
** test_vreinterpretq_s16_f16:
** ret
*/
TEST_UNARY (vreinterpretq_s16_f16, int16x8_t, float16x8_t)

/*
** test_vreinterpretq_s16_bf16:
** ret
*/
TEST_UNARY (vreinterpretq_s16_bf16, int16x8_t, bfloat16x8_t)

/*
** test_vreinterpretq_s16_u32:
** ret
*/
TEST_UNARY (vreinterpretq_s16_u32, int16x8_t, uint32x4_t)

/*
** test_vreinterpretq_s16_s32:
** ret
*/
TEST_UNARY (vreinterpretq_s16_s32, int16x8_t, int32x4_t)

/*
** test_vreinterpretq_s16_f32:
** ret
*/
TEST_UNARY (vreinterpretq_s16_f32, int16x8_t, float32x4_t)

/*
** test_vreinterpretq_s16_u64:
** ret
*/
TEST_UNARY (vreinterpretq_s16_u64, int16x8_t, uint64x2_t)

/*
** test_vreinterpretq_s16_s64:
** ret
*/
TEST_UNARY (vreinterpretq_s16_s64, int16x8_t, int64x2_t)

/*
** test_vreinterpretq_s16_p64:
** ret
*/
TEST_UNARY (vreinterpretq_s16_p64, int16x8_t, poly64x2_t)

/*
** test_vreinterpretq_s16_p128: { xfail *-*-* }
** fmov	d0, x0
** mov	v0\.d\[1\], x1
** ret
*/
TEST_UNARY (vreinterpretq_s16_p128, int16x8_t, poly128_t)

/*
** test_vreinterpretq_s16_f64:
** ret
*/
TEST_UNARY (vreinterpretq_s16_f64, int16x8_t, float64x2_t)

/*
** test_vreinterpretq_p16_u8:
** ret
*/
TEST_UNARY (vreinterpretq_p16_u8, poly16x8_t, uint8x16_t)

/*
** test_vreinterpretq_p16_s8:
** ret
*/
TEST_UNARY (vreinterpretq_p16_s8, poly16x8_t, int8x16_t)

/*
** test_vreinterpretq_p16_p8:
** ret
*/
TEST_UNARY (vreinterpretq_p16_p8, poly16x8_t, poly8x16_t)

/*
** test_vreinterpretq_p16_mf8:
** ret
*/
TEST_UNARY (vreinterpretq_p16_mf8, poly16x8_t, mfloat8x16_t)

/*
** test_vreinterpretq_p16_u16:
** ret
*/
TEST_UNARY (vreinterpretq_p16_u16, poly16x8_t, uint16x8_t)

/*
** test_vreinterpretq_p16_s16:
** ret
*/
TEST_UNARY (vreinterpretq_p16_s16, poly16x8_t, int16x8_t)

/*
** test_vreinterpretq_p16_f16:
** ret
*/
TEST_UNARY (vreinterpretq_p16_f16, poly16x8_t, float16x8_t)

/*
** test_vreinterpretq_p16_bf16:
** ret
*/
TEST_UNARY (vreinterpretq_p16_bf16, poly16x8_t, bfloat16x8_t)

/*
** test_vreinterpretq_p16_u32:
** ret
*/
TEST_UNARY (vreinterpretq_p16_u32, poly16x8_t, uint32x4_t)

/*
** test_vreinterpretq_p16_s32:
** ret
*/
TEST_UNARY (vreinterpretq_p16_s32, poly16x8_t, int32x4_t)

/*
** test_vreinterpretq_p16_f32:
** ret
*/
TEST_UNARY (vreinterpretq_p16_f32, poly16x8_t, float32x4_t)

/*
** test_vreinterpretq_p16_u64:
** ret
*/
TEST_UNARY (vreinterpretq_p16_u64, poly16x8_t, uint64x2_t)

/*
** test_vreinterpretq_p16_s64:
** ret
*/
TEST_UNARY (vreinterpretq_p16_s64, poly16x8_t, int64x2_t)

/*
** test_vreinterpretq_p16_p64:
** ret
*/
TEST_UNARY (vreinterpretq_p16_p64, poly16x8_t, poly64x2_t)

/*
** test_vreinterpretq_p16_p128: { xfail *-*-* }
** fmov	d0, x0
** mov	v0\.d\[1\], x1
** ret
*/
TEST_UNARY (vreinterpretq_p16_p128, poly16x8_t, poly128_t)

/*
** test_vreinterpretq_p16_f64:
** ret
*/
TEST_UNARY (vreinterpretq_p16_f64, poly16x8_t, float64x2_t)

/*
** test_vreinterpretq_f16_u8:
** ret
*/
TEST_UNARY (vreinterpretq_f16_u8, float16x8_t, uint8x16_t)

/*
** test_vreinterpretq_f16_s8:
** ret
*/
TEST_UNARY (vreinterpretq_f16_s8, float16x8_t, int8x16_t)

/*
** test_vreinterpretq_f16_p8:
** ret
*/
TEST_UNARY (vreinterpretq_f16_p8, float16x8_t, poly8x16_t)

/*
** test_vreinterpretq_f16_mf8:
** ret
*/
TEST_UNARY (vreinterpretq_f16_mf8, float16x8_t, mfloat8x16_t)

/*
** test_vreinterpretq_f16_u16:
** ret
*/
TEST_UNARY (vreinterpretq_f16_u16, float16x8_t, uint16x8_t)

/*
** test_vreinterpretq_f16_s16:
** ret
*/
TEST_UNARY (vreinterpretq_f16_s16, float16x8_t, int16x8_t)

/*
** test_vreinterpretq_f16_p16:
** ret
*/
TEST_UNARY (vreinterpretq_f16_p16, float16x8_t, poly16x8_t)

/*
** test_vreinterpretq_f16_bf16:
** ret
*/
TEST_UNARY (vreinterpretq_f16_bf16, float16x8_t, bfloat16x8_t)

/*
** test_vreinterpretq_f16_u32:
** ret
*/
TEST_UNARY (vreinterpretq_f16_u32, float16x8_t, uint32x4_t)

/*
** test_vreinterpretq_f16_s32:
** ret
*/
TEST_UNARY (vreinterpretq_f16_s32, float16x8_t, int32x4_t)

/*
** test_vreinterpretq_f16_f32:
** ret
*/
TEST_UNARY (vreinterpretq_f16_f32, float16x8_t, float32x4_t)

/*
** test_vreinterpretq_f16_u64:
** ret
*/
TEST_UNARY (vreinterpretq_f16_u64, float16x8_t, uint64x2_t)

/*
** test_vreinterpretq_f16_s64:
** ret
*/
TEST_UNARY (vreinterpretq_f16_s64, float16x8_t, int64x2_t)

/*
** test_vreinterpretq_f16_p64:
** ret
*/
TEST_UNARY (vreinterpretq_f16_p64, float16x8_t, poly64x2_t)

/*
** test_vreinterpretq_f16_p128: { xfail *-*-* }
** fmov	d0, x0
** mov	v0\.d\[1\], x1
** ret
*/
TEST_UNARY (vreinterpretq_f16_p128, float16x8_t, poly128_t)

/*
** test_vreinterpretq_f16_f64:
** ret
*/
TEST_UNARY (vreinterpretq_f16_f64, float16x8_t, float64x2_t)

/*
** test_vreinterpretq_bf16_u8:
** ret
*/
TEST_UNARY (vreinterpretq_bf16_u8, bfloat16x8_t, uint8x16_t)

/*
** test_vreinterpretq_bf16_s8:
** ret
*/
TEST_UNARY (vreinterpretq_bf16_s8, bfloat16x8_t, int8x16_t)

/*
** test_vreinterpretq_bf16_p8:
** ret
*/
TEST_UNARY (vreinterpretq_bf16_p8, bfloat16x8_t, poly8x16_t)

/*
** test_vreinterpretq_bf16_mf8:
** ret
*/
TEST_UNARY (vreinterpretq_bf16_mf8, bfloat16x8_t, mfloat8x16_t)

/*
** test_vreinterpretq_bf16_u16:
** ret
*/
TEST_UNARY (vreinterpretq_bf16_u16, bfloat16x8_t, uint16x8_t)

/*
** test_vreinterpretq_bf16_s16:
** ret
*/
TEST_UNARY (vreinterpretq_bf16_s16, bfloat16x8_t, int16x8_t)

/*
** test_vreinterpretq_bf16_p16:
** ret
*/
TEST_UNARY (vreinterpretq_bf16_p16, bfloat16x8_t, poly16x8_t)

/*
** test_vreinterpretq_bf16_f16:
** ret
*/
TEST_UNARY (vreinterpretq_bf16_f16, bfloat16x8_t, float16x8_t)

/*
** test_vreinterpretq_bf16_u32:
** ret
*/
TEST_UNARY (vreinterpretq_bf16_u32, bfloat16x8_t, uint32x4_t)

/*
** test_vreinterpretq_bf16_s32:
** ret
*/
TEST_UNARY (vreinterpretq_bf16_s32, bfloat16x8_t, int32x4_t)

/*
** test_vreinterpretq_bf16_f32:
** ret
*/
TEST_UNARY (vreinterpretq_bf16_f32, bfloat16x8_t, float32x4_t)

/*
** test_vreinterpretq_bf16_u64:
** ret
*/
TEST_UNARY (vreinterpretq_bf16_u64, bfloat16x8_t, uint64x2_t)

/*
** test_vreinterpretq_bf16_s64:
** ret
*/
TEST_UNARY (vreinterpretq_bf16_s64, bfloat16x8_t, int64x2_t)

/*
** test_vreinterpretq_bf16_p64:
** ret
*/
TEST_UNARY (vreinterpretq_bf16_p64, bfloat16x8_t, poly64x2_t)

/*
** test_vreinterpretq_bf16_p128: { xfail *-*-* }
** fmov	d0, x0
** mov	v0\.d\[1\], x1
** ret
*/
TEST_UNARY (vreinterpretq_bf16_p128, bfloat16x8_t, poly128_t)

/*
** test_vreinterpretq_bf16_f64:
** ret
*/
TEST_UNARY (vreinterpretq_bf16_f64, bfloat16x8_t, float64x2_t)

/*
** test_vreinterpretq_u32_u8:
** ret
*/
TEST_UNARY (vreinterpretq_u32_u8, uint32x4_t, uint8x16_t)

/*
** test_vreinterpretq_u32_s8:
** ret
*/
TEST_UNARY (vreinterpretq_u32_s8, uint32x4_t, int8x16_t)

/*
** test_vreinterpretq_u32_p8:
** ret
*/
TEST_UNARY (vreinterpretq_u32_p8, uint32x4_t, poly8x16_t)

/*
** test_vreinterpretq_u32_mf8:
** ret
*/
TEST_UNARY (vreinterpretq_u32_mf8, uint32x4_t, mfloat8x16_t)

/*
** test_vreinterpretq_u32_u16:
** ret
*/
TEST_UNARY (vreinterpretq_u32_u16, uint32x4_t, uint16x8_t)

/*
** test_vreinterpretq_u32_s16:
** ret
*/
TEST_UNARY (vreinterpretq_u32_s16, uint32x4_t, int16x8_t)

/*
** test_vreinterpretq_u32_p16:
** ret
*/
TEST_UNARY (vreinterpretq_u32_p16, uint32x4_t, poly16x8_t)

/*
** test_vreinterpretq_u32_f16:
** ret
*/
TEST_UNARY (vreinterpretq_u32_f16, uint32x4_t, float16x8_t)

/*
** test_vreinterpretq_u32_bf16:
** ret
*/
TEST_UNARY (vreinterpretq_u32_bf16, uint32x4_t, bfloat16x8_t)

/*
** test_vreinterpretq_u32_s32:
** ret
*/
TEST_UNARY (vreinterpretq_u32_s32, uint32x4_t, int32x4_t)

/*
** test_vreinterpretq_u32_f32:
** ret
*/
TEST_UNARY (vreinterpretq_u32_f32, uint32x4_t, float32x4_t)

/*
** test_vreinterpretq_u32_u64:
** ret
*/
TEST_UNARY (vreinterpretq_u32_u64, uint32x4_t, uint64x2_t)

/*
** test_vreinterpretq_u32_s64:
** ret
*/
TEST_UNARY (vreinterpretq_u32_s64, uint32x4_t, int64x2_t)

/*
** test_vreinterpretq_u32_p64:
** ret
*/
TEST_UNARY (vreinterpretq_u32_p64, uint32x4_t, poly64x2_t)

/*
** test_vreinterpretq_u32_p128: { xfail *-*-* }
** fmov	d0, x0
** mov	v0\.d\[1\], x1
** ret
*/
TEST_UNARY (vreinterpretq_u32_p128, uint32x4_t, poly128_t)

/*
** test_vreinterpretq_u32_f64:
** ret
*/
TEST_UNARY (vreinterpretq_u32_f64, uint32x4_t, float64x2_t)

/*
** test_vreinterpretq_s32_u8:
** ret
*/
TEST_UNARY (vreinterpretq_s32_u8, int32x4_t, uint8x16_t)

/*
** test_vreinterpretq_s32_s8:
** ret
*/
TEST_UNARY (vreinterpretq_s32_s8, int32x4_t, int8x16_t)

/*
** test_vreinterpretq_s32_p8:
** ret
*/
TEST_UNARY (vreinterpretq_s32_p8, int32x4_t, poly8x16_t)

/*
** test_vreinterpretq_s32_mf8:
** ret
*/
TEST_UNARY (vreinterpretq_s32_mf8, int32x4_t, mfloat8x16_t)

/*
** test_vreinterpretq_s32_u16:
** ret
*/
TEST_UNARY (vreinterpretq_s32_u16, int32x4_t, uint16x8_t)

/*
** test_vreinterpretq_s32_s16:
** ret
*/
TEST_UNARY (vreinterpretq_s32_s16, int32x4_t, int16x8_t)

/*
** test_vreinterpretq_s32_p16:
** ret
*/
TEST_UNARY (vreinterpretq_s32_p16, int32x4_t, poly16x8_t)

/*
** test_vreinterpretq_s32_f16:
** ret
*/
TEST_UNARY (vreinterpretq_s32_f16, int32x4_t, float16x8_t)

/*
** test_vreinterpretq_s32_bf16:
** ret
*/
TEST_UNARY (vreinterpretq_s32_bf16, int32x4_t, bfloat16x8_t)

/*
** test_vreinterpretq_s32_u32:
** ret
*/
TEST_UNARY (vreinterpretq_s32_u32, int32x4_t, uint32x4_t)

/*
** test_vreinterpretq_s32_f32:
** ret
*/
TEST_UNARY (vreinterpretq_s32_f32, int32x4_t, float32x4_t)

/*
** test_vreinterpretq_s32_u64:
** ret
*/
TEST_UNARY (vreinterpretq_s32_u64, int32x4_t, uint64x2_t)

/*
** test_vreinterpretq_s32_s64:
** ret
*/
TEST_UNARY (vreinterpretq_s32_s64, int32x4_t, int64x2_t)

/*
** test_vreinterpretq_s32_p64:
** ret
*/
TEST_UNARY (vreinterpretq_s32_p64, int32x4_t, poly64x2_t)

/*
** test_vreinterpretq_s32_p128: { xfail *-*-* }
** fmov	d0, x0
** mov	v0\.d\[1\], x1
** ret
*/
TEST_UNARY (vreinterpretq_s32_p128, int32x4_t, poly128_t)

/*
** test_vreinterpretq_s32_f64:
** ret
*/
TEST_UNARY (vreinterpretq_s32_f64, int32x4_t, float64x2_t)

/*
** test_vreinterpretq_f32_u8:
** ret
*/
TEST_UNARY (vreinterpretq_f32_u8, float32x4_t, uint8x16_t)

/*
** test_vreinterpretq_f32_s8:
** ret
*/
TEST_UNARY (vreinterpretq_f32_s8, float32x4_t, int8x16_t)

/*
** test_vreinterpretq_f32_p8:
** ret
*/
TEST_UNARY (vreinterpretq_f32_p8, float32x4_t, poly8x16_t)

/*
** test_vreinterpretq_f32_mf8:
** ret
*/
TEST_UNARY (vreinterpretq_f32_mf8, float32x4_t, mfloat8x16_t)

/*
** test_vreinterpretq_f32_u16:
** ret
*/
TEST_UNARY (vreinterpretq_f32_u16, float32x4_t, uint16x8_t)

/*
** test_vreinterpretq_f32_s16:
** ret
*/
TEST_UNARY (vreinterpretq_f32_s16, float32x4_t, int16x8_t)

/*
** test_vreinterpretq_f32_p16:
** ret
*/
TEST_UNARY (vreinterpretq_f32_p16, float32x4_t, poly16x8_t)

/*
** test_vreinterpretq_f32_f16:
** ret
*/
TEST_UNARY (vreinterpretq_f32_f16, float32x4_t, float16x8_t)

/*
** test_vreinterpretq_f32_bf16:
** ret
*/
TEST_UNARY (vreinterpretq_f32_bf16, float32x4_t, bfloat16x8_t)

/*
** test_vreinterpretq_f32_u32:
** ret
*/
TEST_UNARY (vreinterpretq_f32_u32, float32x4_t, uint32x4_t)

/*
** test_vreinterpretq_f32_s32:
** ret
*/
TEST_UNARY (vreinterpretq_f32_s32, float32x4_t, int32x4_t)

/*
** test_vreinterpretq_f32_u64:
** ret
*/
TEST_UNARY (vreinterpretq_f32_u64, float32x4_t, uint64x2_t)

/*
** test_vreinterpretq_f32_s64:
** ret
*/
TEST_UNARY (vreinterpretq_f32_s64, float32x4_t, int64x2_t)

/*
** test_vreinterpretq_f32_p64:
** ret
*/
TEST_UNARY (vreinterpretq_f32_p64, float32x4_t, poly64x2_t)

/*
** test_vreinterpretq_f32_p128: { xfail *-*-* }
** fmov	d0, x0
** mov	v0\.d\[1\], x1
** ret
*/
TEST_UNARY (vreinterpretq_f32_p128, float32x4_t, poly128_t)

/*
** test_vreinterpretq_f32_f64:
** ret
*/
TEST_UNARY (vreinterpretq_f32_f64, float32x4_t, float64x2_t)

/*
** test_vreinterpretq_u64_u8:
** ret
*/
TEST_UNARY (vreinterpretq_u64_u8, uint64x2_t, uint8x16_t)

/*
** test_vreinterpretq_u64_s8:
** ret
*/
TEST_UNARY (vreinterpretq_u64_s8, uint64x2_t, int8x16_t)

/*
** test_vreinterpretq_u64_p8:
** ret
*/
TEST_UNARY (vreinterpretq_u64_p8, uint64x2_t, poly8x16_t)

/*
** test_vreinterpretq_u64_mf8:
** ret
*/
TEST_UNARY (vreinterpretq_u64_mf8, uint64x2_t, mfloat8x16_t)

/*
** test_vreinterpretq_u64_u16:
** ret
*/
TEST_UNARY (vreinterpretq_u64_u16, uint64x2_t, uint16x8_t)

/*
** test_vreinterpretq_u64_s16:
** ret
*/
TEST_UNARY (vreinterpretq_u64_s16, uint64x2_t, int16x8_t)

/*
** test_vreinterpretq_u64_p16:
** ret
*/
TEST_UNARY (vreinterpretq_u64_p16, uint64x2_t, poly16x8_t)

/*
** test_vreinterpretq_u64_f16:
** ret
*/
TEST_UNARY (vreinterpretq_u64_f16, uint64x2_t, float16x8_t)

/*
** test_vreinterpretq_u64_bf16:
** ret
*/
TEST_UNARY (vreinterpretq_u64_bf16, uint64x2_t, bfloat16x8_t)

/*
** test_vreinterpretq_u64_u32:
** ret
*/
TEST_UNARY (vreinterpretq_u64_u32, uint64x2_t, uint32x4_t)

/*
** test_vreinterpretq_u64_s32:
** ret
*/
TEST_UNARY (vreinterpretq_u64_s32, uint64x2_t, int32x4_t)

/*
** test_vreinterpretq_u64_f32:
** ret
*/
TEST_UNARY (vreinterpretq_u64_f32, uint64x2_t, float32x4_t)

/*
** test_vreinterpretq_u64_s64:
** ret
*/
TEST_UNARY (vreinterpretq_u64_s64, uint64x2_t, int64x2_t)

/*
** test_vreinterpretq_u64_p64:
** ret
*/
TEST_UNARY (vreinterpretq_u64_p64, uint64x2_t, poly64x2_t)

/*
** test_vreinterpretq_u64_p128: { xfail *-*-* }
** fmov	d0, x0
** mov	v0\.d\[1\], x1
** ret
*/
TEST_UNARY (vreinterpretq_u64_p128, uint64x2_t, poly128_t)

/*
** test_vreinterpretq_u64_f64:
** ret
*/
TEST_UNARY (vreinterpretq_u64_f64, uint64x2_t, float64x2_t)

/*
** test_vreinterpretq_s64_u8:
** ret
*/
TEST_UNARY (vreinterpretq_s64_u8, int64x2_t, uint8x16_t)

/*
** test_vreinterpretq_s64_s8:
** ret
*/
TEST_UNARY (vreinterpretq_s64_s8, int64x2_t, int8x16_t)

/*
** test_vreinterpretq_s64_p8:
** ret
*/
TEST_UNARY (vreinterpretq_s64_p8, int64x2_t, poly8x16_t)

/*
** test_vreinterpretq_s64_mf8:
** ret
*/
TEST_UNARY (vreinterpretq_s64_mf8, int64x2_t, mfloat8x16_t)

/*
** test_vreinterpretq_s64_u16:
** ret
*/
TEST_UNARY (vreinterpretq_s64_u16, int64x2_t, uint16x8_t)

/*
** test_vreinterpretq_s64_s16:
** ret
*/
TEST_UNARY (vreinterpretq_s64_s16, int64x2_t, int16x8_t)

/*
** test_vreinterpretq_s64_p16:
** ret
*/
TEST_UNARY (vreinterpretq_s64_p16, int64x2_t, poly16x8_t)

/*
** test_vreinterpretq_s64_f16:
** ret
*/
TEST_UNARY (vreinterpretq_s64_f16, int64x2_t, float16x8_t)

/*
** test_vreinterpretq_s64_bf16:
** ret
*/
TEST_UNARY (vreinterpretq_s64_bf16, int64x2_t, bfloat16x8_t)

/*
** test_vreinterpretq_s64_u32:
** ret
*/
TEST_UNARY (vreinterpretq_s64_u32, int64x2_t, uint32x4_t)

/*
** test_vreinterpretq_s64_s32:
** ret
*/
TEST_UNARY (vreinterpretq_s64_s32, int64x2_t, int32x4_t)

/*
** test_vreinterpretq_s64_f32:
** ret
*/
TEST_UNARY (vreinterpretq_s64_f32, int64x2_t, float32x4_t)

/*
** test_vreinterpretq_s64_u64:
** ret
*/
TEST_UNARY (vreinterpretq_s64_u64, int64x2_t, uint64x2_t)

/*
** test_vreinterpretq_s64_p64:
** ret
*/
TEST_UNARY (vreinterpretq_s64_p64, int64x2_t, poly64x2_t)

/*
** test_vreinterpretq_s64_p128: { xfail *-*-* }
** fmov	d0, x0
** mov	v0\.d\[1\], x1
** ret
*/
TEST_UNARY (vreinterpretq_s64_p128, int64x2_t, poly128_t)

/*
** test_vreinterpretq_s64_f64:
** ret
*/
TEST_UNARY (vreinterpretq_s64_f64, int64x2_t, float64x2_t)

/*
** test_vreinterpretq_p64_u8:
** ret
*/
TEST_UNARY (vreinterpretq_p64_u8, poly64x2_t, uint8x16_t)

/*
** test_vreinterpretq_p64_s8:
** ret
*/
TEST_UNARY (vreinterpretq_p64_s8, poly64x2_t, int8x16_t)

/*
** test_vreinterpretq_p64_p8:
** ret
*/
TEST_UNARY (vreinterpretq_p64_p8, poly64x2_t, poly8x16_t)

/*
** test_vreinterpretq_p64_mf8:
** ret
*/
TEST_UNARY (vreinterpretq_p64_mf8, poly64x2_t, mfloat8x16_t)

/*
** test_vreinterpretq_p64_u16:
** ret
*/
TEST_UNARY (vreinterpretq_p64_u16, poly64x2_t, uint16x8_t)

/*
** test_vreinterpretq_p64_s16:
** ret
*/
TEST_UNARY (vreinterpretq_p64_s16, poly64x2_t, int16x8_t)

/*
** test_vreinterpretq_p64_p16:
** ret
*/
TEST_UNARY (vreinterpretq_p64_p16, poly64x2_t, poly16x8_t)

/*
** test_vreinterpretq_p64_f16:
** ret
*/
TEST_UNARY (vreinterpretq_p64_f16, poly64x2_t, float16x8_t)

/*
** test_vreinterpretq_p64_bf16:
** ret
*/
TEST_UNARY (vreinterpretq_p64_bf16, poly64x2_t, bfloat16x8_t)

/*
** test_vreinterpretq_p64_u32:
** ret
*/
TEST_UNARY (vreinterpretq_p64_u32, poly64x2_t, uint32x4_t)

/*
** test_vreinterpretq_p64_s32:
** ret
*/
TEST_UNARY (vreinterpretq_p64_s32, poly64x2_t, int32x4_t)

/*
** test_vreinterpretq_p64_f32:
** ret
*/
TEST_UNARY (vreinterpretq_p64_f32, poly64x2_t, float32x4_t)

/*
** test_vreinterpretq_p64_u64:
** ret
*/
TEST_UNARY (vreinterpretq_p64_u64, poly64x2_t, uint64x2_t)

/*
** test_vreinterpretq_p64_s64:
** ret
*/
TEST_UNARY (vreinterpretq_p64_s64, poly64x2_t, int64x2_t)

/*
** test_vreinterpretq_p64_p128: { xfail *-*-* }
** fmov	d0, x0
** mov	v0\.d\[1\], x1
** ret
*/
TEST_UNARY (vreinterpretq_p64_p128, poly64x2_t, poly128_t)

/*
** test_vreinterpretq_p64_f64:
** ret
*/
TEST_UNARY (vreinterpretq_p64_f64, poly64x2_t, float64x2_t)

/*
** test_vreinterpretq_p128_u8: { xfail *-*-* }
** fmov	x0, d0
** mov	x1, v0\.d\[1\]
** ret
*/
TEST_UNARY (vreinterpretq_p128_u8, poly128_t, uint8x16_t)

/*
** test_vreinterpretq_p128_s8: { xfail *-*-* }
** fmov	x0, d0
** mov	x1, v0\.d\[1\]
** ret
*/
TEST_UNARY (vreinterpretq_p128_s8, poly128_t, int8x16_t)

/*
** test_vreinterpretq_p128_p8: { xfail *-*-* }
** fmov	x0, d0
** mov	x1, v0\.d\[1\]
** ret
*/
TEST_UNARY (vreinterpretq_p128_p8, poly128_t, poly8x16_t)

/*
** test_vreinterpretq_p128_mf8: { xfail *-*-* }
** fmov	x0, d0
** mov	x1, v0\.d\[1\]
** ret
*/
TEST_UNARY (vreinterpretq_p128_mf8, poly128_t, mfloat8x16_t)

/*
** test_vreinterpretq_p128_u16: { xfail *-*-* }
** fmov	x0, d0
** mov	x1, v0\.d\[1\]
** ret
*/
TEST_UNARY (vreinterpretq_p128_u16, poly128_t, uint16x8_t)

/*
** test_vreinterpretq_p128_s16: { xfail *-*-* }
** fmov	x0, d0
** mov	x1, v0\.d\[1\]
** ret
*/
TEST_UNARY (vreinterpretq_p128_s16, poly128_t, int16x8_t)

/*
** test_vreinterpretq_p128_p16: { xfail *-*-* }
** fmov	x0, d0
** mov	x1, v0\.d\[1\]
** ret
*/
TEST_UNARY (vreinterpretq_p128_p16, poly128_t, poly16x8_t)

/*
** test_vreinterpretq_p128_f16: { xfail *-*-* }
** fmov	x0, d0
** mov	x1, v0\.d\[1\]
** ret
*/
TEST_UNARY (vreinterpretq_p128_f16, poly128_t, float16x8_t)

/*
** test_vreinterpretq_p128_bf16: { xfail *-*-* }
** fmov	x0, d0
** mov	x1, v0\.d\[1\]
** ret
*/
TEST_UNARY (vreinterpretq_p128_bf16, poly128_t, bfloat16x8_t)

/*
** test_vreinterpretq_p128_u32: { xfail *-*-* }
** fmov	x0, d0
** mov	x1, v0\.d\[1\]
** ret
*/
TEST_UNARY (vreinterpretq_p128_u32, poly128_t, uint32x4_t)

/*
** test_vreinterpretq_p128_s32: { xfail *-*-* }
** fmov	x0, d0
** mov	x1, v0\.d\[1\]
** ret
*/
TEST_UNARY (vreinterpretq_p128_s32, poly128_t, int32x4_t)

/*
** test_vreinterpretq_p128_f32: { xfail *-*-* }
** fmov	x0, d0
** mov	x1, v0\.d\[1\]
** ret
*/
TEST_UNARY (vreinterpretq_p128_f32, poly128_t, float32x4_t)

/*
** test_vreinterpretq_p128_u64: { xfail *-*-* }
** fmov	x0, d0
** mov	x1, v0\.d\[1\]
** ret
*/
TEST_UNARY (vreinterpretq_p128_u64, poly128_t, uint64x2_t)

/*
** test_vreinterpretq_p128_s64: { xfail *-*-* }
** fmov	x0, d0
** mov	x1, v0\.d\[1\]
** ret
*/
TEST_UNARY (vreinterpretq_p128_s64, poly128_t, int64x2_t)

/*
** test_vreinterpretq_p128_p64: { xfail *-*-* }
** fmov	x0, d0
** mov	x1, v0\.d\[1\]
** ret
*/
TEST_UNARY (vreinterpretq_p128_p64, poly128_t, poly64x2_t)

/*
** test_vreinterpretq_p128_f64: { xfail *-*-* }
** fmov	x0, d0
** mov	x1, v0\.d\[1\]
** ret
*/
TEST_UNARY (vreinterpretq_p128_f64, poly128_t, float64x2_t)

/*
** test_vreinterpretq_f64_u8:
** ret
*/
TEST_UNARY (vreinterpretq_f64_u8, float64x2_t, uint8x16_t)

/*
** test_vreinterpretq_f64_s8:
** ret
*/
TEST_UNARY (vreinterpretq_f64_s8, float64x2_t, int8x16_t)

/*
** test_vreinterpretq_f64_p8:
** ret
*/
TEST_UNARY (vreinterpretq_f64_p8, float64x2_t, poly8x16_t)

/*
** test_vreinterpretq_f64_mf8:
** ret
*/
TEST_UNARY (vreinterpretq_f64_mf8, float64x2_t, mfloat8x16_t)

/*
** test_vreinterpretq_f64_u16:
** ret
*/
TEST_UNARY (vreinterpretq_f64_u16, float64x2_t, uint16x8_t)

/*
** test_vreinterpretq_f64_s16:
** ret
*/
TEST_UNARY (vreinterpretq_f64_s16, float64x2_t, int16x8_t)

/*
** test_vreinterpretq_f64_p16:
** ret
*/
TEST_UNARY (vreinterpretq_f64_p16, float64x2_t, poly16x8_t)

/*
** test_vreinterpretq_f64_f16:
** ret
*/
TEST_UNARY (vreinterpretq_f64_f16, float64x2_t, float16x8_t)

/*
** test_vreinterpretq_f64_bf16:
** ret
*/
TEST_UNARY (vreinterpretq_f64_bf16, float64x2_t, bfloat16x8_t)

/*
** test_vreinterpretq_f64_u32:
** ret
*/
TEST_UNARY (vreinterpretq_f64_u32, float64x2_t, uint32x4_t)

/*
** test_vreinterpretq_f64_s32:
** ret
*/
TEST_UNARY (vreinterpretq_f64_s32, float64x2_t, int32x4_t)

/*
** test_vreinterpretq_f64_f32:
** ret
*/
TEST_UNARY (vreinterpretq_f64_f32, float64x2_t, float32x4_t)

/*
** test_vreinterpretq_f64_u64:
** ret
*/
TEST_UNARY (vreinterpretq_f64_u64, float64x2_t, uint64x2_t)

/*
** test_vreinterpretq_f64_s64:
** ret
*/
TEST_UNARY (vreinterpretq_f64_s64, float64x2_t, int64x2_t)

/*
** test_vreinterpretq_f64_p64:
** ret
*/
TEST_UNARY (vreinterpretq_f64_p64, float64x2_t, poly64x2_t)

/*
** test_vreinterpretq_f64_p128: { xfail *-*-* }
** fmov	x0, d0
** mov	x1, v0\.d\[1\]
** ret
*/
TEST_UNARY (vreinterpretq_f64_p128, float64x2_t, poly128_t)
