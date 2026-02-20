/* { dg-do compile } */
/* { dg-final { check-function-bodies "**" "" } } */

#include "arm_neon_test.h"

/*
** test_vbic_u8:
** bic	v0\.8b, v0\.8b, v1\.8b
** ret
*/
TEST_UNIFORM_BINARY (vbic_u8, uint8x8_t)

/*
** test_vbic_s8:
** bic	v0\.8b, v0\.8b, v1\.8b
** ret
*/
TEST_UNIFORM_BINARY (vbic_s8, int8x8_t)

/*
** test_vbic_u16:
** bic	v0\.8b, v0\.8b, v1\.8b
** ret
*/
TEST_UNIFORM_BINARY (vbic_u16, uint16x4_t)

/*
** test_vbic_s16:
** bic	v0\.8b, v0\.8b, v1\.8b
** ret
*/
TEST_UNIFORM_BINARY (vbic_s16, int16x4_t)

/*
** test_vbic_u32:
** bic	v0\.8b, v0\.8b, v1\.8b
** ret
*/
TEST_UNIFORM_BINARY (vbic_u32, uint32x2_t)

/*
** test_vbic_s32:
** bic	v0\.8b, v0\.8b, v1\.8b
** ret
*/
TEST_UNIFORM_BINARY (vbic_s32, int32x2_t)

/*
** test_vbic_u64:
** bic	v0\.8b, v0\.8b, v1\.8b
** ret
*/
TEST_UNIFORM_BINARY (vbic_u64, uint64x1_t)

/*
** test_vbic_s64:
** bic	v0\.8b, v0\.8b, v1\.8b
** ret
*/
TEST_UNIFORM_BINARY (vbic_s64, int64x1_t)

/*
** test_vbicq_u8:
** bic	v0\.16b, v0\.16b, v1\.16b
** ret
*/
TEST_UNIFORM_BINARY (vbicq_u8, uint8x16_t)

/*
** test_vbicq_s8:
** bic	v0\.16b, v0\.16b, v1\.16b
** ret
*/
TEST_UNIFORM_BINARY (vbicq_s8, int8x16_t)

/*
** test_vbicq_u16:
** bic	v0\.16b, v0\.16b, v1\.16b
** ret
*/
TEST_UNIFORM_BINARY (vbicq_u16, uint16x8_t)

/*
** test_vbicq_s16:
** bic	v0\.16b, v0\.16b, v1\.16b
** ret
*/
TEST_UNIFORM_BINARY (vbicq_s16, int16x8_t)

/*
** test_vbicq_u32:
** bic	v0\.16b, v0\.16b, v1\.16b
** ret
*/
TEST_UNIFORM_BINARY (vbicq_u32, uint32x4_t)

/*
** test_vbicq_s32:
** bic	v0\.16b, v0\.16b, v1\.16b
** ret
*/
TEST_UNIFORM_BINARY (vbicq_s32, int32x4_t)

/*
** test_vbicq_u64:
** bic	v0\.16b, v0\.16b, v1\.16b
** ret
*/
TEST_UNIFORM_BINARY (vbicq_u64, uint64x2_t)

/*
** test_vbicq_s64:
** bic	v0\.16b, v0\.16b, v1\.16b
** ret
*/
TEST_UNIFORM_BINARY (vbicq_s64, int64x2_t)
