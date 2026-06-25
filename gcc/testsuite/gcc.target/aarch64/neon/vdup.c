/* { dg-do compile } */
/* { dg-final { check-function-bodies "**" "" } } */

#include "arm_neon_test.h"

/*
** test_vdup_n_p8:
** dup	v0\.8b, w0
** ret
*/
TEST_UNARY (vdup_n_p8, poly8x8_t, poly8_t)

/*
** test_vdup_n_mf8:
** dup	v0\.8b, v0\.b\[0\]
** ret
*/
TEST_UNARY (vdup_n_mf8, mfloat8x8_t, mfloat8_t)

/*
** test_vdup_n_u8:
** dup	v0\.8b, w0
** ret
*/
TEST_UNARY (vdup_n_u8, uint8x8_t, uint8_t)

/*
** test_vdup_n_s8:
** dup	v0\.8b, w0
** ret
*/
TEST_UNARY (vdup_n_s8, int8x8_t, int8_t)

/*
** test_vdupq_n_p8:
** dup	v0\.16b, w0
** ret
*/
TEST_UNARY (vdupq_n_p8, poly8x16_t, poly8_t)

/*
** test_vdupq_n_mf8:
** dup	v0\.16b, v0\.b\[0\]
** ret
*/
TEST_UNARY (vdupq_n_mf8, mfloat8x16_t, mfloat8_t)

/*
** test_vdupq_n_u8:
** dup	v0\.16b, w0
** ret
*/
TEST_UNARY (vdupq_n_u8, uint8x16_t, uint8_t)

/*
** test_vdupq_n_s8:
** dup	v0\.16b, w0
** ret
*/
TEST_UNARY (vdupq_n_s8, int8x16_t, int8_t)

/*
** test_vdup_n_p16:
** dup	v0\.4h, w0
** ret
*/
TEST_UNARY (vdup_n_p16, poly16x4_t, poly16_t)

/*
** test_vdup_n_f16:
** dup	v0\.4h, v0\.h\[0\]
** ret
*/
TEST_UNARY (vdup_n_f16, float16x4_t, float16_t)

/*
** test_vdup_n_bf16:
** dup	v0\.4h, v0\.h\[0\]
** ret
*/
TEST_UNARY (vdup_n_bf16, bfloat16x4_t, bfloat16_t)

/*
** test_vdup_n_u16:
** dup	v0\.4h, w0
** ret
*/
TEST_UNARY (vdup_n_u16, uint16x4_t, uint16_t)

/*
** test_vdup_n_s16:
** dup	v0\.4h, w0
** ret
*/
TEST_UNARY (vdup_n_s16, int16x4_t, int16_t)

/*
** test_vdupq_n_p16:
** dup	v0\.8h, w0
** ret
*/
TEST_UNARY (vdupq_n_p16, poly16x8_t, poly16_t)

/*
** test_vdupq_n_f16:
** dup	v0\.8h, v0\.h\[0\]
** ret
*/
TEST_UNARY (vdupq_n_f16, float16x8_t, float16_t)

/*
** test_vdupq_n_bf16:
** dup	v0\.8h, v0\.h\[0\]
** ret
*/
TEST_UNARY (vdupq_n_bf16, bfloat16x8_t, bfloat16_t)

/*
** test_vdupq_n_u16:
** dup	v0\.8h, w0
** ret
*/
TEST_UNARY (vdupq_n_u16, uint16x8_t, uint16_t)

/*
** test_vdupq_n_s16:
** dup	v0\.8h, w0
** ret
*/
TEST_UNARY (vdupq_n_s16, int16x8_t, int16_t)

/*
** test_vdup_n_f32:
** dup	v0\.2s, v0\.s\[0\]
** ret
*/
TEST_UNARY (vdup_n_f32, float32x2_t, float32_t)

/*
** test_vdup_n_u32:
** dup	v0\.2s, w0
** ret
*/
TEST_UNARY (vdup_n_u32, uint32x2_t, uint32_t)

/*
** test_vdup_n_s32:
** dup	v0\.2s, w0
** ret
*/
TEST_UNARY (vdup_n_s32, int32x2_t, int32_t)

/*
** test_vdupq_n_f32:
** dup	v0\.4s, v0\.s\[0\]
** ret
*/
TEST_UNARY (vdupq_n_f32, float32x4_t, float32_t)

/*
** test_vdupq_n_u32:
** dup	v0\.4s, w0
** ret
*/
TEST_UNARY (vdupq_n_u32, uint32x4_t, uint32_t)

/*
** test_vdupq_n_s32:
** dup	v0\.4s, w0
** ret
*/
TEST_UNARY (vdupq_n_s32, int32x4_t, int32_t)

/*
** test_vdup_n_p64:
** fmov	d0, x0
** ret
*/
TEST_UNARY (vdup_n_p64, poly64x1_t, poly64_t)

/*
** test_vdup_n_f64:
** ret
*/
TEST_UNARY (vdup_n_f64, float64x1_t, float64_t)

/*
** test_vdup_n_u64:
** fmov	d0, x0
** ret
*/
TEST_UNARY (vdup_n_u64, uint64x1_t, uint64_t)
/*
** test_vdup_n_s64:
** fmov	d0, x0
** ret
*/
TEST_UNARY (vdup_n_s64, int64x1_t, int64_t)

/*
** test_vdupq_n_p64:
** dup	v0\.2d, x0
** ret
*/
TEST_UNARY (vdupq_n_p64, poly64x2_t, poly64_t)

/*
** test_vdupq_n_f64:
** dup	v0\.2d, v0\.d\[0\]
** ret
*/
TEST_UNARY (vdupq_n_f64, float64x2_t, float64_t)

/*
** test_vdupq_n_u64:
** dup	v0\.2d, x0
** ret
*/
TEST_UNARY (vdupq_n_u64, uint64x2_t, uint64_t)

/*
** test_vdupq_n_s64:
** dup	v0\.2d, x0
** ret
*/
TEST_UNARY (vdupq_n_s64, int64x2_t, int64_t)
