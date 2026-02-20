/* { dg-do compile } */
/* { dg-final { check-function-bodies "**" "" } } */

#include "arm_neon_test.h"

/*
** test_vrev16_u8:
** rev16	v0\.8b, v0\.8b
** ret
*/
TEST_UNIFORM_UNARY (vrev16_u8, uint8x8_t)

/*
** test_vrev16_s8:
** rev16	v0\.8b, v0\.8b
** ret
*/
TEST_UNIFORM_UNARY (vrev16_s8, int8x8_t)

/*
** test_vrev16_p8:
** rev16	v0\.8b, v0\.8b
** ret
*/
TEST_UNIFORM_UNARY (vrev16_p8, poly8x8_t)

/*
** test_vrev16_mf8:
** rev16	v0\.8b, v0\.8b
** ret
*/
TEST_UNIFORM_UNARY (vrev16_mf8, mfloat8x8_t)

/*
** test_vrev16q_u8:
** rev16	v0\.16b, v0\.16b
** ret
*/
TEST_UNIFORM_UNARY (vrev16q_u8, uint8x16_t)

/*
** test_vrev16q_s8:
** rev16	v0\.16b, v0\.16b
** ret
*/
TEST_UNIFORM_UNARY (vrev16q_s8, int8x16_t)

/*
** test_vrev16q_p8:
** rev16	v0\.16b, v0\.16b
** ret
*/
TEST_UNIFORM_UNARY (vrev16q_p8, poly8x16_t)

/*
** test_vrev16q_mf8:
** rev16	v0\.16b, v0\.16b
** ret
*/
TEST_UNIFORM_UNARY (vrev16q_mf8, mfloat8x16_t)

/*
** test_vrev32_u8:
** rev32	v0\.8b, v0\.8b
** ret
*/
TEST_UNIFORM_UNARY (vrev32_u8, uint8x8_t)

/*
** test_vrev32_s8:
** rev32	v0\.8b, v0\.8b
** ret
*/
TEST_UNIFORM_UNARY (vrev32_s8, int8x8_t)

/*
** test_vrev32_p8:
** rev32	v0\.8b, v0\.8b
** ret
*/
TEST_UNIFORM_UNARY (vrev32_p8, poly8x8_t)

/*
** test_vrev32_mf8:
** rev32	v0\.8b, v0\.8b
** ret
*/
TEST_UNIFORM_UNARY (vrev32_mf8, mfloat8x8_t)

/*
** test_vrev32_u16:
** rev32	v0\.4h, v0\.4h
** ret
*/
TEST_UNIFORM_UNARY (vrev32_u16, uint16x4_t)

/*
** test_vrev32_s16:
** rev32	v0\.4h, v0\.4h
** ret
*/
TEST_UNIFORM_UNARY (vrev32_s16, int16x4_t)

/*
** test_vrev32_p16:
** rev32	v0\.4h, v0\.4h
** ret
*/
TEST_UNIFORM_UNARY (vrev32_p16, poly16x4_t)

/*
** test_vrev32q_u8:
** rev32	v0\.16b, v0\.16b
** ret
*/
TEST_UNIFORM_UNARY (vrev32q_u8, uint8x16_t)

/*
** test_vrev32q_s8:
** rev32	v0\.16b, v0\.16b
** ret
*/
TEST_UNIFORM_UNARY (vrev32q_s8, int8x16_t)

/*
** test_vrev32q_p8:
** rev32	v0\.16b, v0\.16b
** ret
*/
TEST_UNIFORM_UNARY (vrev32q_p8, poly8x16_t)

/*
** test_vrev32q_mf8:
** rev32	v0\.16b, v0\.16b
** ret
*/
TEST_UNIFORM_UNARY (vrev32q_mf8, mfloat8x16_t)

/*
** test_vrev32q_u16:
** rev32	v0\.8h, v0\.8h
** ret
*/
TEST_UNIFORM_UNARY (vrev32q_u16, uint16x8_t)

/*
** test_vrev32q_s16:
** rev32	v0\.8h, v0\.8h
** ret
*/
TEST_UNIFORM_UNARY (vrev32q_s16, int16x8_t)

/*
** test_vrev32q_p16:
** rev32	v0\.8h, v0\.8h
** ret
*/
TEST_UNIFORM_UNARY (vrev32q_p16, poly16x8_t)

/*
** test_vrev64_u8:
** rev64	v0\.8b, v0\.8b
** ret
*/
TEST_UNIFORM_UNARY (vrev64_u8, uint8x8_t)

/*
** test_vrev64_s8:
** rev64	v0\.8b, v0\.8b
** ret
*/
TEST_UNIFORM_UNARY (vrev64_s8, int8x8_t)

/*
** test_vrev64_p8:
** rev64	v0\.8b, v0\.8b
** ret
*/
TEST_UNIFORM_UNARY (vrev64_p8, poly8x8_t)

/*
** test_vrev64_mf8:
** rev64	v0\.8b, v0\.8b
** ret
*/
TEST_UNIFORM_UNARY (vrev64_mf8, mfloat8x8_t)

/*
** test_vrev64q_u8:
** rev64	v0\.16b, v0\.16b
** ret
*/
TEST_UNIFORM_UNARY (vrev64q_u8, uint8x16_t)

/*
** test_vrev64q_s8:
** rev64	v0\.16b, v0\.16b
** ret
*/
TEST_UNIFORM_UNARY (vrev64q_s8, int8x16_t)

/*
** test_vrev64q_p8:
** rev64	v0\.16b, v0\.16b
** ret
*/
TEST_UNIFORM_UNARY (vrev64q_p8, poly8x16_t)

/*
** test_vrev64q_mf8:
** rev64	v0\.16b, v0\.16b
** ret
*/
TEST_UNIFORM_UNARY (vrev64q_mf8, mfloat8x16_t)

/*
** test_vrev64_u16:
** rev64	v0\.4h, v0\.4h
** ret
*/
TEST_UNIFORM_UNARY (vrev64_u16, uint16x4_t)

/*
** test_vrev64_s16:
** rev64	v0\.4h, v0\.4h
** ret
*/
TEST_UNIFORM_UNARY (vrev64_s16, int16x4_t)

/*
** test_vrev64_p16:
** rev64	v0\.4h, v0\.4h
** ret
*/
TEST_UNIFORM_UNARY (vrev64_p16, poly16x4_t)

/*
** test_vrev64_f16:
** rev64	v0\.4h, v0\.4h
** ret
*/
TEST_UNIFORM_UNARY (vrev64_f16, float16x4_t)

/*
** test_vrev64_f32:
** rev64	v0\.2s, v0\.2s
** ret
*/
TEST_UNIFORM_UNARY (vrev64_f32, float32x2_t)

/*
** test_vrev64q_u16:
** rev64	v0\.8h, v0\.8h
** ret
*/
TEST_UNIFORM_UNARY (vrev64q_u16, uint16x8_t)
/*
** test_vrev64q_s16:
** rev64	v0\.8h, v0\.8h
** ret
*/
TEST_UNIFORM_UNARY (vrev64q_s16, int16x8_t)

/*
** test_vrev64q_p16:
** rev64	v0\.8h, v0\.8h
** ret
*/
TEST_UNIFORM_UNARY (vrev64q_p16, poly16x8_t)

/*
** test_vrev64q_f16:
** rev64	v0\.8h, v0\.8h
** ret
*/
TEST_UNIFORM_UNARY (vrev64q_f16, float16x8_t)

/*
** test_vrev64_u32:
** rev64	v0\.2s, v0\.2s
** ret
*/
TEST_UNIFORM_UNARY (vrev64_u32, uint32x2_t)

/*
** test_vrev64_s32:
** rev64	v0\.2s, v0\.2s
** ret
*/
TEST_UNIFORM_UNARY (vrev64_s32, int32x2_t)

/*
** test_vrev64q_u32:
** rev64	v0\.4s, v0\.4s
** ret
*/
TEST_UNIFORM_UNARY (vrev64q_u32, uint32x4_t)

/*
** test_vrev64q_s32:
** rev64	v0\.4s, v0\.4s
** ret
*/
TEST_UNIFORM_UNARY (vrev64q_s32, int32x4_t)

/*
** test_vrev64q_f32:
** rev64	v0\.4s, v0\.4s
** ret
*/
TEST_UNIFORM_UNARY (vrev64q_f32, float32x4_t)
