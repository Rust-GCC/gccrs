/* { dg-do compile } */
/* { dg-final { check-function-bodies "**" "" } } */

#include "arm_neon_test.h"

/*
** test_vorn_u8:
** orn	v0\.8b, v0\.8b, v1\.8b
** ret
*/
TEST_UNIFORM_BINARY (vorn_u8, uint8x8_t)

/*
** test_vorn_s8:
** orn	v0\.8b, v0\.8b, v1\.8b
** ret
*/
TEST_UNIFORM_BINARY (vorn_s8, int8x8_t)

/*
** test_vorn_u16:
** orn	v0\.8b, v0\.8b, v1\.8b
** ret
*/
TEST_UNIFORM_BINARY (vorn_u16, uint16x4_t)

/*
** test_vorn_s16:
** orn	v0\.8b, v0\.8b, v1\.8b
** ret
*/
TEST_UNIFORM_BINARY (vorn_s16, int16x4_t)

/*
** test_vorn_u32:
** orn	v0\.8b, v0\.8b, v1\.8b
** ret
*/
TEST_UNIFORM_BINARY (vorn_u32, uint32x2_t)

/*
** test_vorn_s32:
** orn	v0\.8b, v0\.8b, v1\.8b
** ret
*/
TEST_UNIFORM_BINARY (vorn_s32, int32x2_t)

/*
** test_vorn_u64:
** orn	v0\.8b, v0\.8b, v1\.8b
** ret
*/
TEST_UNIFORM_BINARY (vorn_u64, uint64x1_t)

/*
** test_vorn_s64:
** orn	v0\.8b, v0\.8b, v1\.8b
** ret
*/
TEST_UNIFORM_BINARY (vorn_s64, int64x1_t)

/*
** test_vornq_u8:
** orn	v0\.16b, v0\.16b, v1\.16b
** ret
*/
TEST_UNIFORM_BINARY (vornq_u8, uint8x16_t)

/*
** test_vornq_s8:
** orn	v0\.16b, v0\.16b, v1\.16b
** ret
*/
TEST_UNIFORM_BINARY (vornq_s8, int8x16_t)

/*
** test_vornq_u16:
** orn	v0\.16b, v0\.16b, v1\.16b
** ret
*/
TEST_UNIFORM_BINARY (vornq_u16, uint16x8_t)

/*
** test_vornq_s16:
** orn	v0\.16b, v0\.16b, v1\.16b
** ret
*/
TEST_UNIFORM_BINARY (vornq_s16, int16x8_t)

/*
** test_vornq_u32:
** orn	v0\.16b, v0\.16b, v1\.16b
** ret
*/
TEST_UNIFORM_BINARY (vornq_u32, uint32x4_t)

/*
** test_vornq_s32:
** orn	v0\.16b, v0\.16b, v1\.16b
** ret
*/
TEST_UNIFORM_BINARY (vornq_s32, int32x4_t)

/*
** test_vornq_u64:
** orn	v0\.16b, v0\.16b, v1\.16b
** ret
*/
TEST_UNIFORM_BINARY (vornq_u64, uint64x2_t)

/*
** test_vornq_s64:
** orn	v0\.16b, v0\.16b, v1\.16b
** ret
*/
TEST_UNIFORM_BINARY (vornq_s64, int64x2_t)
