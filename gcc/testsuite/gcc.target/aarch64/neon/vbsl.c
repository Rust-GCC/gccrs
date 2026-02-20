/* { dg-do compile } */
/* { dg-final { check-function-bodies "**" "" } } */

#include "arm_neon_test.h"

/*
** test_vbsl_u8:
** bsl	v0\.8b, v1\.8b, v2\.8b
** ret
*/
TEST_TERNARY (vbsl_u8, uint8x8_t, uint8x8_t, uint8x8_t, uint8x8_t)

/*
** test_vbsl_s8:
** bsl	v0\.8b, v1\.8b, v2\.8b
** ret
*/
TEST_TERNARY (vbsl_s8, int8x8_t, uint8x8_t, int8x8_t, int8x8_t)

/*
** test_vbsl_p8:
** bsl	v0\.8b, v1\.8b, v2\.8b
** ret
*/
TEST_TERNARY (vbsl_p8, poly8x8_t, uint8x8_t, poly8x8_t, poly8x8_t)

/*
** test_vbsl_mf8:
** bsl	v0\.8b, v1\.8b, v2\.8b
** ret
*/
TEST_TERNARY (vbsl_mf8, mfloat8x8_t, uint8x8_t, mfloat8x8_t, mfloat8x8_t)

/*
** test_vbsl_u16:
** bsl	v0\.8b, v1\.8b, v2\.8b
** ret
*/
TEST_TERNARY (vbsl_u16, uint16x4_t, uint16x4_t, uint16x4_t, uint16x4_t)

/*
** test_vbsl_s16:
** bsl	v0\.8b, v1\.8b, v2\.8b
** ret
*/
TEST_TERNARY (vbsl_s16, int16x4_t, uint16x4_t, int16x4_t, int16x4_t)

/*
** test_vbsl_p16:
** bsl	v0\.8b, v1\.8b, v2\.8b
** ret
*/
TEST_TERNARY (vbsl_p16, poly16x4_t, uint16x4_t, poly16x4_t, poly16x4_t)

/*
** test_vbsl_f16:
** bsl	v0\.8b, v1\.8b, v2\.8b
** ret
*/
TEST_TERNARY (vbsl_f16, float16x4_t, uint16x4_t, float16x4_t, float16x4_t)

/*
** test_vbsl_u32:
** bsl	v0\.8b, v1\.8b, v2\.8b
** ret
*/
TEST_TERNARY (vbsl_u32, uint32x2_t, uint32x2_t, uint32x2_t, uint32x2_t)

/*
** test_vbsl_s32:
** bsl	v0\.8b, v1\.8b, v2\.8b
** ret
*/
TEST_TERNARY (vbsl_s32, int32x2_t, uint32x2_t, int32x2_t, int32x2_t)

/*
** test_vbsl_f32:
** bsl	v0\.8b, v1\.8b, v2\.8b
** ret
*/
TEST_TERNARY (vbsl_f32, float32x2_t, uint32x2_t, float32x2_t, float32x2_t)

/*
** test_vbsl_u64:
** bsl	v0\.8b, v1\.8b, v2\.8b
** ret
*/
TEST_TERNARY (vbsl_u64, uint64x1_t, uint64x1_t, uint64x1_t, uint64x1_t)

/*
** test_vbsl_s64:
** bsl	v0\.8b, v1\.8b, v2\.8b
** ret
*/
TEST_TERNARY (vbsl_s64, int64x1_t, uint64x1_t, int64x1_t, int64x1_t)

/*
** test_vbsl_p64:
** bsl	v0\.8b, v1\.8b, v2\.8b
** ret
*/
TEST_TERNARY (vbsl_p64, poly64x1_t, uint64x1_t, poly64x1_t, poly64x1_t)

/*
** test_vbsl_f64:
** bsl	v0\.8b, v1\.8b, v2\.8b
** ret
*/
TEST_TERNARY (vbsl_f64, float64x1_t, uint64x1_t, float64x1_t, float64x1_t)

/*
** test_vbslq_u8:
** bsl	v0\.16b, v1\.16b, v2\.16b
** ret
*/
TEST_TERNARY (vbslq_u8, uint8x16_t, uint8x16_t, uint8x16_t, uint8x16_t)

/*
** test_vbslq_s8:
** bsl	v0\.16b, v1\.16b, v2\.16b
** ret
*/
TEST_TERNARY (vbslq_s8, int8x16_t, uint8x16_t, int8x16_t, int8x16_t)

/*
** test_vbslq_p8:
** bsl	v0\.16b, v1\.16b, v2\.16b
** ret
*/
TEST_TERNARY (vbslq_p8, poly8x16_t, uint8x16_t, poly8x16_t, poly8x16_t)

/*
** test_vbslq_mf8:
** bsl	v0\.16b, v1\.16b, v2\.16b
** ret
*/
TEST_TERNARY (vbslq_mf8, mfloat8x16_t, uint8x16_t, mfloat8x16_t, mfloat8x16_t)

/*
** test_vbslq_u16:
** bsl	v0\.16b, v1\.16b, v2\.16b
** ret
*/
TEST_TERNARY (vbslq_u16, uint16x8_t, uint16x8_t, uint16x8_t, uint16x8_t)

/*
** test_vbslq_s16:
** bsl	v0\.16b, v1\.16b, v2\.16b
** ret
*/
TEST_TERNARY (vbslq_s16, int16x8_t, uint16x8_t, int16x8_t, int16x8_t)

/*
** test_vbslq_p16:
** bsl	v0\.16b, v1\.16b, v2\.16b
** ret
*/
TEST_TERNARY (vbslq_p16, poly16x8_t, uint16x8_t, poly16x8_t, poly16x8_t)

/*
** test_vbslq_f16:
** bsl	v0\.16b, v1\.16b, v2\.16b
** ret
*/
TEST_TERNARY (vbslq_f16, float16x8_t, uint16x8_t, float16x8_t, float16x8_t)

/*
** test_vbslq_u32:
** bsl	v0\.16b, v1\.16b, v2\.16b
** ret
*/
TEST_TERNARY (vbslq_u32, uint32x4_t, uint32x4_t, uint32x4_t, uint32x4_t)

/*
** test_vbslq_s32:
** bsl	v0\.16b, v1\.16b, v2\.16b
** ret
*/
TEST_TERNARY (vbslq_s32, int32x4_t, uint32x4_t, int32x4_t, int32x4_t)

/*
** test_vbslq_f32:
** bsl	v0\.16b, v1\.16b, v2\.16b
** ret
*/
TEST_TERNARY (vbslq_f32, float32x4_t, uint32x4_t, float32x4_t, float32x4_t)

/*
** test_vbslq_u64:
** bsl	v0\.16b, v1\.16b, v2\.16b
** ret
*/
TEST_TERNARY (vbslq_u64, uint64x2_t, uint64x2_t, uint64x2_t, uint64x2_t)

/*
** test_vbslq_s64:
** bsl	v0\.16b, v1\.16b, v2\.16b
** ret
*/
TEST_TERNARY (vbslq_s64, int64x2_t, uint64x2_t, int64x2_t, int64x2_t)

/*
** test_vbslq_p64:
** bsl	v0\.16b, v1\.16b, v2\.16b
** ret
*/
TEST_TERNARY (vbslq_p64, poly64x2_t, uint64x2_t, poly64x2_t, poly64x2_t)

/*
** test_vbslq_f64:
** bsl	v0\.16b, v1\.16b, v2\.16b
** ret
*/
TEST_TERNARY (vbslq_f64, float64x2_t, uint64x2_t, float64x2_t, float64x2_t)
