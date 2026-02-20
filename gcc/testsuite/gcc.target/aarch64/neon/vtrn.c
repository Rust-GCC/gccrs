/* { dg-do compile } */
/* { dg-final { check-function-bodies "**" "" } } */

#include "arm_neon_test.h"

/*
** test_vtrn1_u8:
** trn1	v0\.8b, v0\.8b, v1\.8b
** ret
*/
TEST_UNIFORM_BINARY (vtrn1_u8, uint8x8_t)

/*
** test_vtrn1_s8:
** trn1	v0\.8b, v0\.8b, v1\.8b
** ret
*/
TEST_UNIFORM_BINARY (vtrn1_s8, int8x8_t)

/*
** test_vtrn1_p8:
** trn1	v0\.8b, v0\.8b, v1\.8b
** ret
*/
TEST_UNIFORM_BINARY (vtrn1_p8, poly8x8_t)

/*
** test_vtrn1_mf8:
** trn1	v0\.8b, v0\.8b, v1\.8b
** ret
*/
TEST_UNIFORM_BINARY (vtrn1_mf8, mfloat8x8_t)

/*
** test_vtrn1_u16:
** trn1	v0\.4h, v0\.4h, v1\.4h
** ret
*/
TEST_UNIFORM_BINARY (vtrn1_u16, uint16x4_t)

/*
** test_vtrn1_s16:
** trn1	v0\.4h, v0\.4h, v1\.4h
** ret
*/
TEST_UNIFORM_BINARY (vtrn1_s16, int16x4_t)

/*
** test_vtrn1_f16:
** trn1	v0\.4h, v0\.4h, v1\.4h
** ret
*/
TEST_UNIFORM_BINARY (vtrn1_f16, float16x4_t)

/*
** test_vtrn1_p16:
** trn1	v0\.4h, v0\.4h, v1\.4h
** ret
*/
TEST_UNIFORM_BINARY (vtrn1_p16, poly16x4_t)

/*
** test_vtrn1_u32:
** (trn|zip)1	v0\.2s, v0\.2s, v1\.2s
** ret
*/
TEST_UNIFORM_BINARY (vtrn1_u32, uint32x2_t)

/*
** test_vtrn1_s32:
** (trn|zip)1	v0\.2s, v0\.2s, v1\.2s
** ret
*/
TEST_UNIFORM_BINARY (vtrn1_s32, int32x2_t)

/*
** test_vtrn1_f32:
** (trn|zip)1	v0\.2s, v0\.2s, v1\.2s
** ret
*/
TEST_UNIFORM_BINARY (vtrn1_f32, float32x2_t)

/*
** test_vtrn1q_u8:
** trn1	v0\.16b, v0\.16b, v1\.16b
** ret
*/
TEST_UNIFORM_BINARY (vtrn1q_u8, uint8x16_t)

/*
** test_vtrn1q_s8:
** trn1	v0\.16b, v0\.16b, v1\.16b
** ret
*/
TEST_UNIFORM_BINARY (vtrn1q_s8, int8x16_t)

/*
** test_vtrn1q_p8:
** trn1	v0\.16b, v0\.16b, v1\.16b
** ret
*/
TEST_UNIFORM_BINARY (vtrn1q_p8, poly8x16_t)

/*
** test_vtrn1q_mf8:
** trn1	v0\.16b, v0\.16b, v1\.16b
** ret
*/
TEST_UNIFORM_BINARY (vtrn1q_mf8, mfloat8x16_t)

/*
** test_vtrn1q_u16:
** trn1	v0\.8h, v0\.8h, v1\.8h
** ret
*/
TEST_UNIFORM_BINARY (vtrn1q_u16, uint16x8_t)

/*
** test_vtrn1q_s16:
** trn1	v0\.8h, v0\.8h, v1\.8h
** ret
*/
TEST_UNIFORM_BINARY (vtrn1q_s16, int16x8_t)

/*
** test_vtrn1q_f16:
** trn1	v0\.8h, v0\.8h, v1\.8h
** ret
*/
TEST_UNIFORM_BINARY (vtrn1q_f16, float16x8_t)

/*
** test_vtrn1q_p16:
** trn1	v0\.8h, v0\.8h, v1\.8h
** ret
*/
TEST_UNIFORM_BINARY (vtrn1q_p16, poly16x8_t)

/*
** test_vtrn1q_u32:
** trn1	v0\.4s, v0\.4s, v1\.4s
** ret
*/
TEST_UNIFORM_BINARY (vtrn1q_u32, uint32x4_t)

/*
** test_vtrn1q_s32:
** trn1	v0\.4s, v0\.4s, v1\.4s
** ret
*/
TEST_UNIFORM_BINARY (vtrn1q_s32, int32x4_t)

/*
** test_vtrn1q_f32:
** trn1	v0\.4s, v0\.4s, v1\.4s
** ret
*/
TEST_UNIFORM_BINARY (vtrn1q_f32, float32x4_t)

/*
** test_vtrn1q_u64:
** (trn|zip)1	v0\.2d, v0\.2d, v1\.2d
** ret
*/
TEST_UNIFORM_BINARY (vtrn1q_u64, uint64x2_t)

/*
** test_vtrn1q_s64:
** (trn|zip)1	v0\.2d, v0\.2d, v1\.2d
** ret
*/
TEST_UNIFORM_BINARY (vtrn1q_s64, int64x2_t)

/*
** test_vtrn1q_f64:
** (trn|zip)1	v0\.2d, v0\.2d, v1\.2d
** ret
*/
TEST_UNIFORM_BINARY (vtrn1q_f64, float64x2_t)

/*
** test_vtrn1q_p64:
** (trn|zip)1	v0\.2d, v0\.2d, v1\.2d
** ret
*/
TEST_UNIFORM_BINARY (vtrn1q_p64, poly64x2_t)

/*
** test_vtrn2_u8:
** trn2	v0\.8b, v0\.8b, v1\.8b
** ret
*/
TEST_UNIFORM_BINARY (vtrn2_u8, uint8x8_t)

/*
** test_vtrn2_s8:
** trn2	v0\.8b, v0\.8b, v1\.8b
** ret
*/
TEST_UNIFORM_BINARY (vtrn2_s8, int8x8_t)

/*
** test_vtrn2_p8:
** trn2	v0\.8b, v0\.8b, v1\.8b
** ret
*/
TEST_UNIFORM_BINARY (vtrn2_p8, poly8x8_t)

/*
** test_vtrn2_mf8:
** trn2	v0\.8b, v0\.8b, v1\.8b
** ret
*/
TEST_UNIFORM_BINARY (vtrn2_mf8, mfloat8x8_t)

/*
** test_vtrn2_u16:
** trn2	v0\.4h, v0\.4h, v1\.4h
** ret
*/
TEST_UNIFORM_BINARY (vtrn2_u16, uint16x4_t)

/*
** test_vtrn2_s16:
** trn2	v0\.4h, v0\.4h, v1\.4h
** ret
*/
TEST_UNIFORM_BINARY (vtrn2_s16, int16x4_t)

/*
** test_vtrn2_f16:
** trn2	v0\.4h, v0\.4h, v1\.4h
** ret
*/
TEST_UNIFORM_BINARY (vtrn2_f16, float16x4_t)

/*
** test_vtrn2_p16:
** trn2	v0\.4h, v0\.4h, v1\.4h
** ret
*/
TEST_UNIFORM_BINARY (vtrn2_p16, poly16x4_t)

/*
** test_vtrn2_u32:
** (trn|zip)2	v0\.2s, v0\.2s, v1\.2s
** ret
*/
TEST_UNIFORM_BINARY (vtrn2_u32, uint32x2_t)

/*
** test_vtrn2_s32:
** (trn|zip)2	v0\.2s, v0\.2s, v1\.2s
** ret
*/
TEST_UNIFORM_BINARY (vtrn2_s32, int32x2_t)

/*
** test_vtrn2_f32:
** (trn|zip)2	v0\.2s, v0\.2s, v1\.2s
** ret
*/
TEST_UNIFORM_BINARY (vtrn2_f32, float32x2_t)

/*
** test_vtrn2q_u8:
** trn2	v0\.16b, v0\.16b, v1\.16b
** ret
*/
TEST_UNIFORM_BINARY (vtrn2q_u8, uint8x16_t)

/*
** test_vtrn2q_s8:
** trn2	v0\.16b, v0\.16b, v1\.16b
** ret
*/
TEST_UNIFORM_BINARY (vtrn2q_s8, int8x16_t)

/*
** test_vtrn2q_p8:
** trn2	v0\.16b, v0\.16b, v1\.16b
** ret
*/
TEST_UNIFORM_BINARY (vtrn2q_p8, poly8x16_t)

/*
** test_vtrn2q_mf8:
** trn2	v0\.16b, v0\.16b, v1\.16b
** ret
*/
TEST_UNIFORM_BINARY (vtrn2q_mf8, mfloat8x16_t)

/*
** test_vtrn2q_u16:
** trn2	v0\.8h, v0\.8h, v1\.8h
** ret
*/
TEST_UNIFORM_BINARY (vtrn2q_u16, uint16x8_t)

/*
** test_vtrn2q_s16:
** trn2	v0\.8h, v0\.8h, v1\.8h
** ret
*/
TEST_UNIFORM_BINARY (vtrn2q_s16, int16x8_t)

/*
** test_vtrn2q_f16:
** trn2	v0\.8h, v0\.8h, v1\.8h
** ret
*/
TEST_UNIFORM_BINARY (vtrn2q_f16, float16x8_t)

/*
** test_vtrn2q_p16:
** trn2	v0\.8h, v0\.8h, v1\.8h
** ret
*/
TEST_UNIFORM_BINARY (vtrn2q_p16, poly16x8_t)

/*
** test_vtrn2q_u32:
** trn2	v0\.4s, v0\.4s, v1\.4s
** ret
*/
TEST_UNIFORM_BINARY (vtrn2q_u32, uint32x4_t)

/*
** test_vtrn2q_s32:
** trn2	v0\.4s, v0\.4s, v1\.4s
** ret
*/
TEST_UNIFORM_BINARY (vtrn2q_s32, int32x4_t)

/*
** test_vtrn2q_f32:
** trn2	v0\.4s, v0\.4s, v1\.4s
** ret
*/
TEST_UNIFORM_BINARY (vtrn2q_f32, float32x4_t)

/*
** test_vtrn2q_u64:
** (trn|zip)2	v0\.2d, v0\.2d, v1\.2d
** ret
*/
TEST_UNIFORM_BINARY (vtrn2q_u64, uint64x2_t)

/*
** test_vtrn2q_s64:
** (trn|zip)2	v0\.2d, v0\.2d, v1\.2d
** ret
*/
TEST_UNIFORM_BINARY (vtrn2q_s64, int64x2_t)

/*
** test_vtrn2q_f64:
** (trn|zip)2	v0\.2d, v0\.2d, v1\.2d
** ret
*/
TEST_UNIFORM_BINARY (vtrn2q_f64, float64x2_t)

/*
** test_vtrn2q_p64:
** (trn|zip)2	v0\.2d, v0\.2d, v1\.2d
** ret
*/
TEST_UNIFORM_BINARY (vtrn2q_p64, poly64x2_t)

/*
** test_vtrn_u8:
** ...
** trn1	v0\.8b, .+
** trn2	v1\.8b, .+
** ret
*/
TEST_BINARY (vtrn_u8, uint8x8x2_t, uint8x8_t, uint8x8_t)

/*
** test_vtrn_s8:
** ...
** trn1	v0\.8b, .+
** trn2	v1\.8b, .+
** ret
*/
TEST_BINARY (vtrn_s8, int8x8x2_t, int8x8_t, int8x8_t)

/*
** test_vtrn_p8:
** ...
** trn1	v0\.8b, .+
** trn2	v1\.8b, .+
** ret
*/
TEST_BINARY (vtrn_p8, poly8x8x2_t, poly8x8_t, poly8x8_t)

/*
** test_vtrn_mf8:
** ...
** trn1	v0\.8b, .+
** trn2	v1\.8b, .+
** ret
*/
TEST_BINARY (vtrn_mf8, mfloat8x8x2_t, mfloat8x8_t, mfloat8x8_t)

/*
** test_vtrn_u16:
** ...
** trn1	v0\.4h, .+
** trn2	v1\.4h, .+
** ret
*/
TEST_BINARY (vtrn_u16, uint16x4x2_t, uint16x4_t, uint16x4_t)

/*
** test_vtrn_s16:
** ...
** trn1	v0\.4h, .+
** trn2	v1\.4h, .+
** ret
*/
TEST_BINARY (vtrn_s16, int16x4x2_t, int16x4_t, int16x4_t)

/*
** test_vtrn_f16:
** ...
** trn1	v0\.4h, .+
** trn2	v1\.4h, .+
** ret
*/
TEST_BINARY (vtrn_f16, float16x4x2_t, float16x4_t, float16x4_t)

/*
** test_vtrn_p16:
** ...
** trn1	v0\.4h, .+
** trn2	v1\.4h, .+
** ret
*/
TEST_BINARY (vtrn_p16, poly16x4x2_t, poly16x4_t, poly16x4_t)

/*
** test_vtrn_u32:
** ...
** (trn|zip)1	v0\.2s, .+
** (trn|zip)2	v1\.2s, .+
** ret
*/
TEST_BINARY (vtrn_u32, uint32x2x2_t, uint32x2_t, uint32x2_t)

/*
** test_vtrn_s32:
** ...
** (trn|zip)1	v0\.2s, .+
** (trn|zip)2	v1\.2s, .+
** ret
*/
TEST_BINARY (vtrn_s32, int32x2x2_t, int32x2_t, int32x2_t)

/*
** test_vtrn_f32:
** ...
** (trn|zip)1	v0\.2s, .+
** (trn|zip)2	v1\.2s, .+
** ret
*/
TEST_BINARY (vtrn_f32, float32x2x2_t, float32x2_t, float32x2_t)

/*
** test_vtrnq_u8:
** ...
** trn1	v0\.16b, .+
** trn2	v1\.16b, .+
** ret
*/
TEST_BINARY (vtrnq_u8, uint8x16x2_t, uint8x16_t, uint8x16_t)

/*
** test_vtrnq_s8:
** ...
** trn1	v0\.16b, .+
** trn2	v1\.16b, .+
** ret
*/
TEST_BINARY (vtrnq_s8, int8x16x2_t, int8x16_t, int8x16_t)

/*
** test_vtrnq_p8:
** ...
** trn1	v0\.16b, .+
** trn2	v1\.16b, .+
** ret
*/
TEST_BINARY (vtrnq_p8, poly8x16x2_t, poly8x16_t, poly8x16_t)

/*
** test_vtrnq_mf8:
** ...
** trn1	v0\.16b, .+
** trn2	v1\.16b, .+
** ret
*/
TEST_BINARY (vtrnq_mf8, mfloat8x16x2_t, mfloat8x16_t, mfloat8x16_t)

/*
** test_vtrnq_u16:
** ...
** trn1	v0\.8h, .+
** trn2	v1\.8h, .+
** ret
*/
TEST_BINARY (vtrnq_u16, uint16x8x2_t, uint16x8_t, uint16x8_t)

/*
** test_vtrnq_s16:
** ...
** trn1	v0\.8h, .+
** trn2	v1\.8h, .+
** ret
*/
TEST_BINARY (vtrnq_s16, int16x8x2_t, int16x8_t, int16x8_t)

/*
** test_vtrnq_f16:
** ...
** trn1	v0\.8h, .+
** trn2	v1\.8h, .+
** ret
*/
TEST_BINARY (vtrnq_f16, float16x8x2_t, float16x8_t, float16x8_t)

/*
** test_vtrnq_p16:
** ...
** trn1	v0\.8h, .+
** trn2	v1\.8h, .+
** ret
*/
TEST_BINARY (vtrnq_p16, poly16x8x2_t, poly16x8_t, poly16x8_t)

/*
** test_vtrnq_u32:
** ...
** trn1	v0\.4s, .+
** trn2	v1\.4s, .+
** ret
*/
TEST_BINARY (vtrnq_u32, uint32x4x2_t, uint32x4_t, uint32x4_t)

/*
** test_vtrnq_s32:
** ...
** trn1	v0\.4s, .+
** trn2	v1\.4s, .+
** ret
*/
TEST_BINARY (vtrnq_s32, int32x4x2_t, int32x4_t, int32x4_t)

/*
** test_vtrnq_f32:
** ...
** trn1	v0\.4s, .+
** trn2	v1\.4s, .+
** ret
*/
TEST_BINARY (vtrnq_f32, float32x4x2_t, float32x4_t, float32x4_t)
