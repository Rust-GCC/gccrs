/* { dg-do compile } */
/* { dg-final { check-function-bodies "**" "" } } */

#include "arm_neon_test.h"

/*
** test_veor_u8:
** eor	v0\.8b, (v0\.8b, v1\.8b|v1\.8b, v0\.8b)
** ret
*/
TEST_UNIFORM_BINARY (veor_u8, uint8x8_t)

/*
** test_veor_s8:
** eor	v0\.8b, (v0\.8b, v1\.8b|v1\.8b, v0\.8b)
** ret
*/
TEST_UNIFORM_BINARY (veor_s8, int8x8_t)

/*
** test_veor_u16:
** eor	v0\.8b, (v0\.8b, v1\.8b|v1\.8b, v0\.8b)
** ret
*/
TEST_UNIFORM_BINARY (veor_u16, uint16x4_t)

/*
** test_veor_s16:
** eor	v0\.8b, (v0\.8b, v1\.8b|v1\.8b, v0\.8b)
** ret
*/
TEST_UNIFORM_BINARY (veor_s16, int16x4_t)

/*
** test_veor_u32:
** eor	v0\.8b, (v0\.8b, v1\.8b|v1\.8b, v0\.8b)
** ret
*/
TEST_UNIFORM_BINARY (veor_u32, uint32x2_t)

/*
** test_veor_s32:
** eor	v0\.8b, (v0\.8b, v1\.8b|v1\.8b, v0\.8b)
** ret
*/
TEST_UNIFORM_BINARY (veor_s32, int32x2_t)

/*
** test_veor_u64:
** eor	v0\.8b, (v0\.8b, v1\.8b|v1\.8b, v0\.8b)
** ret
*/
TEST_UNIFORM_BINARY (veor_u64, uint64x1_t)

/*
** test_veor_s64:
** eor	v0\.8b, (v0\.8b, v1\.8b|v1\.8b, v0\.8b)
** ret
*/
TEST_UNIFORM_BINARY (veor_s64, int64x1_t)

/*
** test_veorq_u8:
** eor	v0\.16b, (v0\.16b, v1\.16b|v1\.16b, v0\.16b)
** ret
*/
TEST_UNIFORM_BINARY (veorq_u8, uint8x16_t)

/*
** test_veorq_s8:
** eor	v0\.16b, (v0\.16b, v1\.16b|v1\.16b, v0\.16b)
** ret
*/
TEST_UNIFORM_BINARY (veorq_s8, int8x16_t)

/*
** test_veorq_u16:
** eor	v0\.16b, (v0\.16b, v1\.16b|v1\.16b, v0\.16b)
** ret
*/
TEST_UNIFORM_BINARY (veorq_u16, uint16x8_t)

/*
** test_veorq_s16:
** eor	v0\.16b, (v0\.16b, v1\.16b|v1\.16b, v0\.16b)
** ret
*/
TEST_UNIFORM_BINARY (veorq_s16, int16x8_t)

/*
** test_veorq_u32:
** eor	v0\.16b, (v0\.16b, v1\.16b|v1\.16b, v0\.16b)
** ret
*/
TEST_UNIFORM_BINARY (veorq_u32, uint32x4_t)

/*
** test_veorq_s32:
** eor	v0\.16b, (v0\.16b, v1\.16b|v1\.16b, v0\.16b)
** ret
*/
TEST_UNIFORM_BINARY (veorq_s32, int32x4_t)

/*
** test_veorq_u64:
** eor	v0\.16b, (v0\.16b, v1\.16b|v1\.16b, v0\.16b)
** ret
*/
TEST_UNIFORM_BINARY (veorq_u64, uint64x2_t)

/*
** test_veorq_s64:
** eor	v0\.16b, (v0\.16b, v1\.16b|v1\.16b, v0\.16b)
** ret
*/
TEST_UNIFORM_BINARY (veorq_s64, int64x2_t)
