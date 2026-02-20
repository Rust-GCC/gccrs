/* { dg-do compile } */
/* { dg-final { check-function-bodies "**" "" } } */

#include "arm_neon_test.h"

/*
** test_vand_u8:
** and	v0\.8b, (v0\.8b, v1\.8b|v1\.8b, v0\.8b)
** ret
*/
TEST_UNIFORM_BINARY (vand_u8, uint8x8_t)

/*
** test_vand_s8:
** and	v0\.8b, (v0\.8b, v1\.8b|v1\.8b, v0\.8b)
** ret
*/
TEST_UNIFORM_BINARY (vand_s8, int8x8_t)

/*
** test_vand_u16:
** and	v0\.8b, (v0\.8b, v1\.8b|v1\.8b, v0\.8b)
** ret
*/
TEST_UNIFORM_BINARY (vand_u16, uint16x4_t)

/*
** test_vand_s16:
** and	v0\.8b, (v0\.8b, v1\.8b|v1\.8b, v0\.8b)
** ret
*/
TEST_UNIFORM_BINARY (vand_s16, int16x4_t)

/*
** test_vand_u32:
** and	v0\.8b, (v0\.8b, v1\.8b|v1\.8b, v0\.8b)
** ret
*/
TEST_UNIFORM_BINARY (vand_u32, uint32x2_t)

/*
** test_vand_s32:
** and	v0\.8b, (v0\.8b, v1\.8b|v1\.8b, v0\.8b)
** ret
*/
TEST_UNIFORM_BINARY (vand_s32, int32x2_t)

/*
** test_vand_u64:
** and	v0\.8b, (v0\.8b, v1\.8b|v1\.8b, v0\.8b)
** ret
*/
TEST_UNIFORM_BINARY (vand_u64, uint64x1_t)

/*
** test_vand_s64:
** and	v0\.8b, (v0\.8b, v1\.8b|v1\.8b, v0\.8b)
** ret
*/
TEST_UNIFORM_BINARY (vand_s64, int64x1_t)

/*
** test_vandq_u8:
** and	v0\.16b, (v0\.16b, v1\.16b|v1\.16b, v0\.16b)
** ret
*/
TEST_UNIFORM_BINARY (vandq_u8, uint8x16_t)

/*
** test_vandq_s8:
** and	v0\.16b, (v0\.16b, v1\.16b|v1\.16b, v0\.16b)
** ret
*/
TEST_UNIFORM_BINARY (vandq_s8, int8x16_t)

/*
** test_vandq_u16:
** and	v0\.16b, (v0\.16b, v1\.16b|v1\.16b, v0\.16b)
** ret
*/
TEST_UNIFORM_BINARY (vandq_u16, uint16x8_t)

/*
** test_vandq_s16:
** and	v0\.16b, (v0\.16b, v1\.16b|v1\.16b, v0\.16b)
** ret
*/
TEST_UNIFORM_BINARY (vandq_s16, int16x8_t)

/*
** test_vandq_u32:
** and	v0\.16b, (v0\.16b, v1\.16b|v1\.16b, v0\.16b)
** ret
*/
TEST_UNIFORM_BINARY (vandq_u32, uint32x4_t)

/*
** test_vandq_s32:
** and	v0\.16b, (v0\.16b, v1\.16b|v1\.16b, v0\.16b)
** ret
*/
TEST_UNIFORM_BINARY (vandq_s32, int32x4_t)

/*
** test_vandq_u64:
** and	v0\.16b, (v0\.16b, v1\.16b|v1\.16b, v0\.16b)
** ret
*/
TEST_UNIFORM_BINARY (vandq_u64, uint64x2_t)

/*
** test_vandq_s64:
** and	v0\.16b, (v0\.16b, v1\.16b|v1\.16b, v0\.16b)
** ret
*/
TEST_UNIFORM_BINARY (vandq_s64, int64x2_t)
