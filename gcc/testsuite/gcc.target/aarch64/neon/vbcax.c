/* { dg-do compile } */
/* { dg-final { check-function-bodies "**" "" } } */

#include "arm_neon_test.h"

/*
** test_vbcaxq_u8:
** bcax	v0\.16b, v0\.16b, v1\.16b, v2\.16b
** ret
*/
TEST_UNIFORM_TERNARY (vbcaxq_u8, uint8x16_t)

/*
** test_vbcaxq_u16:
** bcax	v0\.16b, v0\.16b, v1\.16b, v2\.16b
** ret
*/
TEST_UNIFORM_TERNARY (vbcaxq_u16, uint16x8_t)

/*
** test_vbcaxq_u32:
** bcax	v0\.16b, v0\.16b, v1\.16b, v2\.16b
** ret
*/
TEST_UNIFORM_TERNARY (vbcaxq_u32, uint32x4_t)

/*
** test_vbcaxq_u64:
** bcax	v0\.16b, v0\.16b, v1\.16b, v2\.16b
** ret
*/
TEST_UNIFORM_TERNARY (vbcaxq_u64, uint64x2_t)

/*
** test_vbcaxq_s8:
** bcax	v0\.16b, v0\.16b, v1\.16b, v2\.16b
** ret
*/
TEST_UNIFORM_TERNARY (vbcaxq_s8, int8x16_t)

/*
** test_vbcaxq_s16:
** bcax	v0\.16b, v0\.16b, v1\.16b, v2\.16b
** ret
*/
TEST_UNIFORM_TERNARY (vbcaxq_s16, int16x8_t)

/*
** test_vbcaxq_s32:
** bcax	v0\.16b, v0\.16b, v1\.16b, v2\.16b
** ret
*/
TEST_UNIFORM_TERNARY (vbcaxq_s32, int32x4_t)

/*
** test_vbcaxq_s64:
** bcax	v0\.16b, v0\.16b, v1\.16b, v2\.16b
** ret
*/
TEST_UNIFORM_TERNARY (vbcaxq_s64, int64x2_t)
