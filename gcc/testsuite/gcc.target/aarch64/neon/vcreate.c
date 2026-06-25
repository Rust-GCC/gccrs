/* { dg-do compile } */
/* { dg-final { check-function-bodies "**" "" } } */

#include "arm_neon_test.h"

#define TEST_CREATE(NAME, RET_TYPE)                                            \
  RET_TYPE test_##NAME (uint64_t arg1) { return NAME (arg1); }

/*
** test_vcreate_u8:
** fmov	d0, x0
** ret
*/
TEST_CREATE (vcreate_u8, uint8x8_t)

/*
** test_vcreate_s8:
** fmov	d0, x0
** ret
*/
TEST_CREATE (vcreate_s8, int8x8_t)

/*
** test_vcreate_p8:
** fmov	d0, x0
** ret
*/
TEST_CREATE (vcreate_p8, poly8x8_t)

/*
** test_vcreate_mf8:
** fmov	d0, x0
** ret
*/
TEST_CREATE (vcreate_mf8, mfloat8x8_t)

/*
** test_vcreate_u16:
** fmov	d0, x0
** ret
*/
TEST_CREATE (vcreate_u16, uint16x4_t)

/*
** test_vcreate_s16:
** fmov	d0, x0
** ret
*/
TEST_CREATE (vcreate_s16, int16x4_t)

/*
** test_vcreate_p16:
** fmov	d0, x0
** ret
*/
TEST_CREATE (vcreate_p16, poly16x4_t)

/*
** test_vcreate_f16:
** fmov	d0, x0
** ret
*/
TEST_CREATE (vcreate_f16, float16x4_t)

/*
** test_vcreate_bf16:
** fmov	d0, x0
** ret
*/
TEST_CREATE (vcreate_bf16, bfloat16x4_t)

/*
** test_vcreate_u32:
** fmov	d0, x0
** ret
*/
TEST_CREATE (vcreate_u32, uint32x2_t)

/*
** test_vcreate_s32:
** fmov	d0, x0
** ret
*/
TEST_CREATE (vcreate_s32, int32x2_t)

/*
** test_vcreate_f32:
** fmov	d0, x0
** ret
*/
TEST_CREATE (vcreate_f32, float32x2_t)

/*
** test_vcreate_u64:
** fmov	d0, x0
** ret
*/
TEST_CREATE (vcreate_u64, uint64x1_t)

/*
** test_vcreate_s64:
** fmov	d0, x0
** ret
*/
TEST_CREATE (vcreate_s64, int64x1_t)

/*
** test_vcreate_p64:
** fmov	d0, x0
** ret
*/
TEST_CREATE (vcreate_p64, poly64x1_t)

/*
** test_vcreate_f64:
** fmov	d0, x0
** ret
*/
TEST_CREATE (vcreate_f64, float64x1_t)
