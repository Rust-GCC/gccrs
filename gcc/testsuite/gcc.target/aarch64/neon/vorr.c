/* { dg-do compile } */
/* { dg-final { check-function-bodies "**" "" } } */

#include "arm_neon_test.h"

/*
** test_vorr_u8:
** orr	v0\.8b, (v0\.8b, v1\.8b|v1\.8b, v0\.8b)
** ret
*/
TEST_UNIFORM_BINARY (vorr_u8, uint8x8_t)

/*
** test_vorr_s8:
** orr	v0\.8b, (v0\.8b, v1\.8b|v1\.8b, v0\.8b)
** ret
*/
TEST_UNIFORM_BINARY (vorr_s8, int8x8_t)

/*
** test_vorr_u16:
** orr	v0\.8b, (v0\.8b, v1\.8b|v1\.8b, v0\.8b)
** ret
*/
TEST_UNIFORM_BINARY (vorr_u16, uint16x4_t)

/*
** test_vorr_s16:
** orr	v0\.8b, (v0\.8b, v1\.8b|v1\.8b, v0\.8b)
** ret
*/
TEST_UNIFORM_BINARY (vorr_s16, int16x4_t)

/*
** test_vorr_u32:
** orr	v0\.8b, (v0\.8b, v1\.8b|v1\.8b, v0\.8b)
** ret
*/
TEST_UNIFORM_BINARY (vorr_u32, uint32x2_t)

/*
** test_vorr_s32:
** orr	v0\.8b, (v0\.8b, v1\.8b|v1\.8b, v0\.8b)
** ret
*/
TEST_UNIFORM_BINARY (vorr_s32, int32x2_t)

/*
** test_vorr_u64:
** orr	v0\.8b, (v0\.8b, v1\.8b|v1\.8b, v0\.8b)
** ret
*/
TEST_UNIFORM_BINARY (vorr_u64, uint64x1_t)

/*
** test_vorr_s64:
** orr	v0\.8b, (v0\.8b, v1\.8b|v1\.8b, v0\.8b)
** ret
*/
TEST_UNIFORM_BINARY (vorr_s64, int64x1_t)

/*
** test_vorrq_u8:
** orr	v0\.16b, (v0\.16b, v1\.16b|v1\.16b, v0\.16b)
** ret
*/
TEST_UNIFORM_BINARY (vorrq_u8, uint8x16_t)

/*
** test_vorrq_s8:
** orr	v0\.16b, (v0\.16b, v1\.16b|v1\.16b, v0\.16b)
** ret
*/
TEST_UNIFORM_BINARY (vorrq_s8, int8x16_t)

/*
** test_vorrq_u16:
** orr	v0\.16b, (v0\.16b, v1\.16b|v1\.16b, v0\.16b)
** ret
*/
TEST_UNIFORM_BINARY (vorrq_u16, uint16x8_t)

/*
** test_vorrq_s16:
** orr	v0\.16b, (v0\.16b, v1\.16b|v1\.16b, v0\.16b)
** ret
*/
TEST_UNIFORM_BINARY (vorrq_s16, int16x8_t)

/*
** test_vorrq_u32:
** orr	v0\.16b, (v0\.16b, v1\.16b|v1\.16b, v0\.16b)
** ret
*/
TEST_UNIFORM_BINARY (vorrq_u32, uint32x4_t)

/*
** test_vorrq_s32:
** orr	v0\.16b, (v0\.16b, v1\.16b|v1\.16b, v0\.16b)
** ret
*/
TEST_UNIFORM_BINARY (vorrq_s32, int32x4_t)

/*
** test_vorrq_u64:
** orr	v0\.16b, (v0\.16b, v1\.16b|v1\.16b, v0\.16b)
** ret
*/
TEST_UNIFORM_BINARY (vorrq_u64, uint64x2_t)

/*
** test_vorrq_s64:
** orr	v0\.16b, (v0\.16b, v1\.16b|v1\.16b, v0\.16b)
** ret
*/
TEST_UNIFORM_BINARY (vorrq_s64, int64x2_t)
