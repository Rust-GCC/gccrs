/* { dg-do compile } */
/* { dg-final { check-function-bodies "**" "" } } */

#include "arm_neon_test.h"

/*
** test_vmvn_u8:
** not	v0\.8b, v0\.8b
** ret
*/
TEST_UNIFORM_UNARY (vmvn_u8, uint8x8_t)

/*
** test_vmvn_s8:
** not	v0\.8b, v0\.8b
** ret
*/
TEST_UNIFORM_UNARY (vmvn_s8, int8x8_t)

/*
** test_vmvn_p8:
** not	v0\.8b, v0\.8b
** ret
*/
TEST_UNIFORM_UNARY (vmvn_p8, poly8x8_t)

/*
** test_vmvn_u16:
** not	v0\.8b, v0\.8b
** ret
*/
TEST_UNIFORM_UNARY (vmvn_u16, uint16x4_t)

/*
** test_vmvn_s16:
** not	v0\.8b, v0\.8b
** ret
*/
TEST_UNIFORM_UNARY (vmvn_s16, int16x4_t)

/*
** test_vmvn_u32:
** not	v0\.8b, v0\.8b
** ret
*/
TEST_UNIFORM_UNARY (vmvn_u32, uint32x2_t)

/*
** test_vmvn_s32:
** not	v0\.8b, v0\.8b
** ret
*/
TEST_UNIFORM_UNARY (vmvn_s32, int32x2_t)

/*
** test_vmvnq_u8:
** not	v0\.16b, v0\.16b
** ret
*/
TEST_UNIFORM_UNARY (vmvnq_u8, uint8x16_t)

/*
** test_vmvnq_s8:
** not	v0\.16b, v0\.16b
** ret
*/
TEST_UNIFORM_UNARY (vmvnq_s8, int8x16_t)

/*
** test_vmvnq_p8:
** not	v0\.16b, v0\.16b
** ret
*/
TEST_UNIFORM_UNARY (vmvnq_p8, poly8x16_t)

/*
** test_vmvnq_u16:
** not	v0\.16b, v0\.16b
** ret
*/
TEST_UNIFORM_UNARY (vmvnq_u16, uint16x8_t)

/*
** test_vmvnq_s16:
** not	v0\.16b, v0\.16b
** ret
*/
TEST_UNIFORM_UNARY (vmvnq_s16, int16x8_t)

/*
** test_vmvnq_u32:
** not	v0\.16b, v0\.16b
** ret
*/
TEST_UNIFORM_UNARY (vmvnq_u32, uint32x4_t)

/*
** test_vmvnq_s32:
** not	v0\.16b, v0\.16b
** ret
*/
TEST_UNIFORM_UNARY (vmvnq_s32, int32x4_t)
