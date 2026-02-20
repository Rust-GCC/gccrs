/* { dg-do compile } */
/* { dg-final { check-function-bodies "**" "" } } */

#include "arm_neon_test.h"

/*
** test_vcls_u8:
** cls	v0\.8b, v0\.8b
** ret
*/
TEST_UNARY (vcls_u8, int8x8_t, uint8x8_t)

/*
** test_vcls_s8:
** cls	v0\.8b, v0\.8b
** ret
*/
TEST_UNARY (vcls_s8, int8x8_t, int8x8_t)

/*
** test_vcls_u16:
** cls	v0\.4h, v0\.4h
** ret
*/
TEST_UNARY (vcls_u16, int16x4_t, uint16x4_t)

/*
** test_vcls_s16:
** cls	v0\.4h, v0\.4h
** ret
*/
TEST_UNARY (vcls_s16, int16x4_t, int16x4_t)

/*
** test_vcls_u32:
** cls	v0\.2s, v0\.2s
** ret
*/
TEST_UNARY (vcls_u32, int32x2_t, uint32x2_t)

/*
** test_vcls_s32:
** cls	v0\.2s, v0\.2s
** ret
*/
TEST_UNARY (vcls_s32, int32x2_t, int32x2_t)

/*
** test_vclsq_u8:
** cls	v0\.16b, v0\.16b
** ret
*/
TEST_UNARY (vclsq_u8, int8x16_t, uint8x16_t)

/*
** test_vclsq_s8:
** cls	v0\.16b, v0\.16b
** ret
*/
TEST_UNARY (vclsq_s8, int8x16_t, int8x16_t)

/*
** test_vclsq_u16:
** cls	v0\.8h, v0\.8h
** ret
*/
TEST_UNARY (vclsq_u16, int16x8_t, uint16x8_t)

/*
** test_vclsq_s16:
** cls	v0\.8h, v0\.8h
** ret
*/
TEST_UNARY (vclsq_s16, int16x8_t, int16x8_t)

/*
** test_vclsq_u32:
** cls	v0\.4s, v0\.4s
** ret
*/
TEST_UNARY (vclsq_u32, int32x4_t, uint32x4_t)

/*
** test_vclsq_s32:
** cls	v0\.4s, v0\.4s
** ret
*/
TEST_UNARY (vclsq_s32, int32x4_t, int32x4_t)
