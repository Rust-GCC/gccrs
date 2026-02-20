/* { dg-do compile } */
/* { dg-final { check-function-bodies "**" "" } } */

#include "arm_neon_test.h"

/*
** test_veor3q_u8:
** eor3	v0\.16b, v0\.16b, v1\.16b, v2\.16b
** ret
*/
TEST_UNIFORM_TERNARY (veor3q_u8, uint8x16_t)

/*
** test_veor3q_u16:
** eor3	v0\.16b, v0\.16b, v1\.16b, v2\.16b
** ret
*/
TEST_UNIFORM_TERNARY (veor3q_u16, uint16x8_t)

/*
** test_veor3q_u32:
** eor3	v0\.16b, v0\.16b, v1\.16b, v2\.16b
** ret
*/
TEST_UNIFORM_TERNARY (veor3q_u32, uint32x4_t)

/*
** test_veor3q_u64:
** eor3	v0\.16b, v0\.16b, v1\.16b, v2\.16b
** ret
*/
TEST_UNIFORM_TERNARY (veor3q_u64, uint64x2_t)

/*
** test_veor3q_s8:
** eor3	v0\.16b, v0\.16b, v1\.16b, v2\.16b
** ret
*/
TEST_UNIFORM_TERNARY (veor3q_s8, int8x16_t)

/*
** test_veor3q_s16:
** eor3	v0\.16b, v0\.16b, v1\.16b, v2\.16b
** ret
*/
TEST_UNIFORM_TERNARY (veor3q_s16, int16x8_t)

/*
** test_veor3q_s32:
** eor3	v0\.16b, v0\.16b, v1\.16b, v2\.16b
** ret
*/
TEST_UNIFORM_TERNARY (veor3q_s32, int32x4_t)

/*
** test_veor3q_s64:
** eor3	v0\.16b, v0\.16b, v1\.16b, v2\.16b
** ret
*/
TEST_UNIFORM_TERNARY (veor3q_s64, int64x2_t)
