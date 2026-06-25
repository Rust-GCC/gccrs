/* { dg-do compile } */
/* { dg-final { check-function-bodies "**" "" } } */

#include "arm_neon_test.h"

/*
** test_vget_high_u8:
** dup	d0, v0\.d\[1\]
** ret
*/
TEST_UNARY (vget_high_u8, uint8x8_t, uint8x16_t)

/*
** test_vget_high_s8:
** dup	d0, v0\.d\[1\]
** ret
*/
TEST_UNARY (vget_high_s8, int8x8_t, int8x16_t)

/*
** test_vget_high_p8:
** dup	d0, v0\.d\[1\]
** ret
*/
TEST_UNARY (vget_high_p8, poly8x8_t, poly8x16_t)

/*
** test_vget_high_mf8:
** dup	d0, v0\.d\[1\]
** ret
*/
TEST_UNARY (vget_high_mf8, mfloat8x8_t, mfloat8x16_t)

/*
** test_vget_high_u16:
** dup	d0, v0\.d\[1\]
** ret
*/
TEST_UNARY (vget_high_u16, uint16x4_t, uint16x8_t)

/*
** test_vget_high_s16:
** dup	d0, v0\.d\[1\]
** ret
*/
TEST_UNARY (vget_high_s16, int16x4_t, int16x8_t)

/*
** test_vget_high_p16:
** dup	d0, v0\.d\[1\]
** ret
*/
TEST_UNARY (vget_high_p16, poly16x4_t, poly16x8_t)

/*
** test_vget_high_f16:
** dup	d0, v0\.d\[1\]
** ret
*/
TEST_UNARY (vget_high_f16, float16x4_t, float16x8_t)

/*
** test_vget_high_bf16:
** dup	d0, v0\.d\[1\]
** ret
*/
TEST_UNARY (vget_high_bf16, bfloat16x4_t, bfloat16x8_t)

/*
** test_vget_high_u32:
** dup	d0, v0\.d\[1\]
** ret
*/
TEST_UNARY (vget_high_u32, uint32x2_t, uint32x4_t)

/*
** test_vget_high_s32:
** dup	d0, v0\.d\[1\]
** ret
*/
TEST_UNARY (vget_high_s32, int32x2_t, int32x4_t)

/*
** test_vget_high_f32:
** dup	d0, v0\.d\[1\]
** ret
*/
TEST_UNARY (vget_high_f32, float32x2_t, float32x4_t)

/*
** test_vget_high_u64:
** dup	d0, v0\.d\[1\]
** ret
*/
TEST_UNARY (vget_high_u64, uint64x1_t, uint64x2_t)

/*
** test_vget_high_s64:
** dup	d0, v0\.d\[1\]
** ret
*/
TEST_UNARY (vget_high_s64, int64x1_t, int64x2_t)

/*
** test_vget_high_p64:
** dup	d0, v0\.d\[1\]
** ret
*/
TEST_UNARY (vget_high_p64, poly64x1_t, poly64x2_t)

/*
** test_vget_high_f64:
** dup	d0, v0\.d\[1\]
** ret
*/
TEST_UNARY (vget_high_f64, float64x1_t, float64x2_t)
