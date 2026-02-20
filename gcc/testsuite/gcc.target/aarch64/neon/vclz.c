/* { dg-do compile } */
/* { dg-final { check-function-bodies "**" "" } } */

#include "arm_neon_test.h"

/*
** test_vclz_u8:
** clz	v0\.8b, v0\.8b
** ret
*/
TEST_UNARY (vclz_u8, uint8x8_t, uint8x8_t)

/*
** test_vclz_s8:
** clz	v0\.8b, v0\.8b
** ret
*/
TEST_UNARY (vclz_s8, int8x8_t, int8x8_t)

/*
** test_vclz_u16:
** clz	v0\.4h, v0\.4h
** ret
*/
TEST_UNARY (vclz_u16, uint16x4_t, uint16x4_t)

/*
** test_vclz_s16:
** clz	v0\.4h, v0\.4h
** ret
*/
TEST_UNARY (vclz_s16, int16x4_t, int16x4_t)

/*
** test_vclz_u32:
** clz	v0\.2s, v0\.2s
** ret
*/
TEST_UNARY (vclz_u32, uint32x2_t, uint32x2_t)

/*
** test_vclz_s32:
** clz	v0\.2s, v0\.2s
** ret
*/
TEST_UNARY (vclz_s32, int32x2_t, int32x2_t)

/*
** test_vclzq_u8:
** clz	v0\.16b, v0\.16b
** ret
*/
TEST_UNARY (vclzq_u8, uint8x16_t, uint8x16_t)

/*
** test_vclzq_s8:
** clz	v0\.16b, v0\.16b
** ret
*/
TEST_UNARY (vclzq_s8, int8x16_t, int8x16_t)

/*
** test_vclzq_u16:
** clz	v0\.8h, v0\.8h
** ret
*/
TEST_UNARY (vclzq_u16, uint16x8_t, uint16x8_t)

/*
** test_vclzq_s16:
** clz	v0\.8h, v0\.8h
** ret
*/
TEST_UNARY (vclzq_s16, int16x8_t, int16x8_t)

/*
** test_vclzq_u32:
** clz	v0\.4s, v0\.4s
** ret
*/
TEST_UNARY (vclzq_u32, uint32x4_t, uint32x4_t)

/*
** test_vclzq_s32:
** clz	v0\.4s, v0\.4s
** ret
*/
TEST_UNARY (vclzq_s32, int32x4_t, int32x4_t)
