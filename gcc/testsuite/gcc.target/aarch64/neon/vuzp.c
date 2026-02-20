/* { dg-do compile } */
/* { dg-final { check-function-bodies "**" "" } } */

#include "arm_neon_test.h"

/*
** test_vuzp1_u8:
** uzp1	v0\.8b, v0\.8b, v1\.8b
** ret
*/
TEST_UNIFORM_BINARY (vuzp1_u8, uint8x8_t)

/*
** test_vuzp1_s8:
** uzp1	v0\.8b, v0\.8b, v1\.8b
** ret
*/
TEST_UNIFORM_BINARY (vuzp1_s8, int8x8_t)

/*
** test_vuzp1_p8:
** uzp1	v0\.8b, v0\.8b, v1\.8b
** ret
*/
TEST_UNIFORM_BINARY (vuzp1_p8, poly8x8_t)

/*
** test_vuzp1_mf8:
** uzp1	v0\.8b, v0\.8b, v1\.8b
** ret
*/
TEST_UNIFORM_BINARY (vuzp1_mf8, mfloat8x8_t)

/*
** test_vuzp1_u16:
** uzp1	v0\.4h, v0\.4h, v1\.4h
** ret
*/
TEST_UNIFORM_BINARY (vuzp1_u16, uint16x4_t)

/*
** test_vuzp1_s16:
** uzp1	v0\.4h, v0\.4h, v1\.4h
** ret
*/
TEST_UNIFORM_BINARY (vuzp1_s16, int16x4_t)

/*
** test_vuzp1_f16:
** uzp1	v0\.4h, v0\.4h, v1\.4h
** ret
*/
TEST_UNIFORM_BINARY (vuzp1_f16, float16x4_t)

/*
** test_vuzp1_p16:
** uzp1	v0\.4h, v0\.4h, v1\.4h
** ret
*/
TEST_UNIFORM_BINARY (vuzp1_p16, poly16x4_t)

/*
** test_vuzp1_u32:
** (uzp|zip)1	v0\.2s, v0\.2s, v1\.2s
** ret
*/
TEST_UNIFORM_BINARY (vuzp1_u32, uint32x2_t)

/*
** test_vuzp1_s32:
** (uzp|zip)1	v0\.2s, v0\.2s, v1\.2s
** ret
*/
TEST_UNIFORM_BINARY (vuzp1_s32, int32x2_t)

/*
** test_vuzp1_f32:
** (uzp|zip)1	v0\.2s, v0\.2s, v1\.2s
** ret
*/
TEST_UNIFORM_BINARY (vuzp1_f32, float32x2_t)

/*
** test_vuzp1q_u8:
** uzp1	v0\.16b, v0\.16b, v1\.16b
** ret
*/
TEST_UNIFORM_BINARY (vuzp1q_u8, uint8x16_t)

/*
** test_vuzp1q_s8:
** uzp1	v0\.16b, v0\.16b, v1\.16b
** ret
*/
TEST_UNIFORM_BINARY (vuzp1q_s8, int8x16_t)

/*
** test_vuzp1q_p8:
** uzp1	v0\.16b, v0\.16b, v1\.16b
** ret
*/
TEST_UNIFORM_BINARY (vuzp1q_p8, poly8x16_t)

/*
** test_vuzp1q_mf8:
** uzp1	v0\.16b, v0\.16b, v1\.16b
** ret
*/
TEST_UNIFORM_BINARY (vuzp1q_mf8, mfloat8x16_t)

/*
** test_vuzp1q_u16:
** uzp1	v0\.8h, v0\.8h, v1\.8h
** ret
*/
TEST_UNIFORM_BINARY (vuzp1q_u16, uint16x8_t)

/*
** test_vuzp1q_s16:
** uzp1	v0\.8h, v0\.8h, v1\.8h
** ret
*/
TEST_UNIFORM_BINARY (vuzp1q_s16, int16x8_t)

/*
** test_vuzp1q_f16:
** uzp1	v0\.8h, v0\.8h, v1\.8h
** ret
*/
TEST_UNIFORM_BINARY (vuzp1q_f16, float16x8_t)

/*
** test_vuzp1q_p16:
** uzp1	v0\.8h, v0\.8h, v1\.8h
** ret
*/
TEST_UNIFORM_BINARY (vuzp1q_p16, poly16x8_t)

/*
** test_vuzp1q_u32:
** uzp1	v0\.4s, v0\.4s, v1\.4s
** ret
*/
TEST_UNIFORM_BINARY (vuzp1q_u32, uint32x4_t)

/*
** test_vuzp1q_s32:
** uzp1	v0\.4s, v0\.4s, v1\.4s
** ret
*/
TEST_UNIFORM_BINARY (vuzp1q_s32, int32x4_t)

/*
** test_vuzp1q_f32:
** uzp1	v0\.4s, v0\.4s, v1\.4s
** ret
*/
TEST_UNIFORM_BINARY (vuzp1q_f32, float32x4_t)

/*
** test_vuzp1q_u64:
** (uzp|zip)1	v0\.2d, v0\.2d, v1\.2d
** ret
*/
TEST_UNIFORM_BINARY (vuzp1q_u64, uint64x2_t)

/*
** test_vuzp1q_s64:
** (uzp|zip)1	v0\.2d, v0\.2d, v1\.2d
** ret
*/
TEST_UNIFORM_BINARY (vuzp1q_s64, int64x2_t)

/*
** test_vuzp1q_f64:
** (uzp|zip)1	v0\.2d, v0\.2d, v1\.2d
** ret
*/
TEST_UNIFORM_BINARY (vuzp1q_f64, float64x2_t)

/*
** test_vuzp1q_p64:
** (uzp|zip)1	v0\.2d, v0\.2d, v1\.2d
** ret
*/
TEST_UNIFORM_BINARY (vuzp1q_p64, poly64x2_t)

/*
** test_vuzp2_u8:
** uzp2	v0\.8b, v0\.8b, v1\.8b
** ret
*/
TEST_UNIFORM_BINARY (vuzp2_u8, uint8x8_t)

/*
** test_vuzp2_s8:
** uzp2	v0\.8b, v0\.8b, v1\.8b
** ret
*/
TEST_UNIFORM_BINARY (vuzp2_s8, int8x8_t)

/*
** test_vuzp2_p8:
** uzp2	v0\.8b, v0\.8b, v1\.8b
** ret
*/
TEST_UNIFORM_BINARY (vuzp2_p8, poly8x8_t)

/*
** test_vuzp2_mf8:
** uzp2	v0\.8b, v0\.8b, v1\.8b
** ret
*/
TEST_UNIFORM_BINARY (vuzp2_mf8, mfloat8x8_t)

/*
** test_vuzp2_u16:
** uzp2	v0\.4h, v0\.4h, v1\.4h
** ret
*/
TEST_UNIFORM_BINARY (vuzp2_u16, uint16x4_t)

/*
** test_vuzp2_s16:
** uzp2	v0\.4h, v0\.4h, v1\.4h
** ret
*/
TEST_UNIFORM_BINARY (vuzp2_s16, int16x4_t)

/*
** test_vuzp2_f16:
** uzp2	v0\.4h, v0\.4h, v1\.4h
** ret
*/
TEST_UNIFORM_BINARY (vuzp2_f16, float16x4_t)

/*
** test_vuzp2_p16:
** uzp2	v0\.4h, v0\.4h, v1\.4h
** ret
*/
TEST_UNIFORM_BINARY (vuzp2_p16, poly16x4_t)

/*
** test_vuzp2_u32:
** (uzp|zip)2	v0\.2s, v0\.2s, v1\.2s
** ret
*/
TEST_UNIFORM_BINARY (vuzp2_u32, uint32x2_t)

/*
** test_vuzp2_s32:
** (uzp|zip)2	v0\.2s, v0\.2s, v1\.2s
** ret
*/
TEST_UNIFORM_BINARY (vuzp2_s32, int32x2_t)

/*
** test_vuzp2_f32:
** (uzp|zip)2	v0\.2s, v0\.2s, v1\.2s
** ret
*/
TEST_UNIFORM_BINARY (vuzp2_f32, float32x2_t)

/*
** test_vuzp2q_u8:
** uzp2	v0\.16b, v0\.16b, v1\.16b
** ret
*/
TEST_UNIFORM_BINARY (vuzp2q_u8, uint8x16_t)

/*
** test_vuzp2q_s8:
** uzp2	v0\.16b, v0\.16b, v1\.16b
** ret
*/
TEST_UNIFORM_BINARY (vuzp2q_s8, int8x16_t)

/*
** test_vuzp2q_p8:
** uzp2	v0\.16b, v0\.16b, v1\.16b
** ret
*/
TEST_UNIFORM_BINARY (vuzp2q_p8, poly8x16_t)

/*
** test_vuzp2q_mf8:
** uzp2	v0\.16b, v0\.16b, v1\.16b
** ret
*/
TEST_UNIFORM_BINARY (vuzp2q_mf8, mfloat8x16_t)

/*
** test_vuzp2q_u16:
** uzp2	v0\.8h, v0\.8h, v1\.8h
** ret
*/
TEST_UNIFORM_BINARY (vuzp2q_u16, uint16x8_t)

/*
** test_vuzp2q_s16:
** uzp2	v0\.8h, v0\.8h, v1\.8h
** ret
*/
TEST_UNIFORM_BINARY (vuzp2q_s16, int16x8_t)

/*
** test_vuzp2q_f16:
** uzp2	v0\.8h, v0\.8h, v1\.8h
** ret
*/
TEST_UNIFORM_BINARY (vuzp2q_f16, float16x8_t)

/*
** test_vuzp2q_p16:
** uzp2	v0\.8h, v0\.8h, v1\.8h
** ret
*/
TEST_UNIFORM_BINARY (vuzp2q_p16, poly16x8_t)

/*
** test_vuzp2q_u32:
** uzp2	v0\.4s, v0\.4s, v1\.4s
** ret
*/
TEST_UNIFORM_BINARY (vuzp2q_u32, uint32x4_t)

/*
** test_vuzp2q_s32:
** uzp2	v0\.4s, v0\.4s, v1\.4s
** ret
*/
TEST_UNIFORM_BINARY (vuzp2q_s32, int32x4_t)

/*
** test_vuzp2q_f32:
** uzp2	v0\.4s, v0\.4s, v1\.4s
** ret
*/
TEST_UNIFORM_BINARY (vuzp2q_f32, float32x4_t)

/*
** test_vuzp2q_u64:
** (uzp|zip)2	v0\.2d, v0\.2d, v1\.2d
** ret
*/
TEST_UNIFORM_BINARY (vuzp2q_u64, uint64x2_t)

/*
** test_vuzp2q_s64:
** (uzp|zip)2	v0\.2d, v0\.2d, v1\.2d
** ret
*/
TEST_UNIFORM_BINARY (vuzp2q_s64, int64x2_t)

/*
** test_vuzp2q_f64:
** (uzp|zip)2	v0\.2d, v0\.2d, v1\.2d
** ret
*/
TEST_UNIFORM_BINARY (vuzp2q_f64, float64x2_t)

/*
** test_vuzp2q_p64:
** (uzp|zip)2	v0\.2d, v0\.2d, v1\.2d
** ret
*/
TEST_UNIFORM_BINARY (vuzp2q_p64, poly64x2_t)

/*
** test_vuzp_u8:
** ...
** uzp1	v0\.8b, .+
** uzp2	v1\.8b, .+
** ret
*/
TEST_BINARY (vuzp_u8, uint8x8x2_t, uint8x8_t, uint8x8_t)

/*
** test_vuzp_s8:
** ...
** uzp1	v0\.8b, .+
** uzp2	v1\.8b, .+
** ret
*/
TEST_BINARY (vuzp_s8, int8x8x2_t, int8x8_t, int8x8_t)

/*
** test_vuzp_p8:
** ...
** uzp1	v0\.8b, .+
** uzp2	v1\.8b, .+
** ret
*/
TEST_BINARY (vuzp_p8, poly8x8x2_t, poly8x8_t, poly8x8_t)

/*
** test_vuzp_mf8:
** ...
** uzp1	v0\.8b, .+
** uzp2	v1\.8b, .+
** ret
*/
TEST_BINARY (vuzp_mf8, mfloat8x8x2_t, mfloat8x8_t, mfloat8x8_t)

/*
** test_vuzp_u16:
** ...
** uzp1	v0\.4h, .+
** uzp2	v1\.4h, .+
** ret
*/
TEST_BINARY (vuzp_u16, uint16x4x2_t, uint16x4_t, uint16x4_t)

/*
** test_vuzp_s16:
** ...
** uzp1	v0\.4h, .+
** uzp2	v1\.4h, .+
** ret
*/
TEST_BINARY (vuzp_s16, int16x4x2_t, int16x4_t, int16x4_t)

/*
** test_vuzp_f16:
** ...
** uzp1	v0\.4h, .+
** uzp2	v1\.4h, .+
** ret
*/
TEST_BINARY (vuzp_f16, float16x4x2_t, float16x4_t, float16x4_t)

/*
** test_vuzp_p16:
** ...
** uzp1	v0\.4h, .+
** uzp2	v1\.4h, .+
** ret
*/
TEST_BINARY (vuzp_p16, poly16x4x2_t, poly16x4_t, poly16x4_t)

/*
** test_vuzp_u32:
** ...
** (uzp|zip)1	v0\.2s, .+
** (uzp|zip)2	v1\.2s, .+
** ret
*/
TEST_BINARY (vuzp_u32, uint32x2x2_t, uint32x2_t, uint32x2_t)

/*
** test_vuzp_s32:
** ...
** (uzp|zip)1	v0\.2s, .+
** (uzp|zip)2	v1\.2s, .+
** ret
*/
TEST_BINARY (vuzp_s32, int32x2x2_t, int32x2_t, int32x2_t)

/*
** test_vuzp_f32:
** ...
** (uzp|zip)1	v0\.2s, .+
** (uzp|zip)2	v1\.2s, .+
** ret
*/
TEST_BINARY (vuzp_f32, float32x2x2_t, float32x2_t, float32x2_t)

/*
** test_vuzpq_u8:
** ...
** uzp1	v0\.16b, .+
** uzp2	v1\.16b, .+
** ret
*/
TEST_BINARY (vuzpq_u8, uint8x16x2_t, uint8x16_t, uint8x16_t)

/*
** test_vuzpq_s8:
** ...
** uzp1	v0\.16b, .+
** uzp2	v1\.16b, .+
** ret
*/
TEST_BINARY (vuzpq_s8, int8x16x2_t, int8x16_t, int8x16_t)

/*
** test_vuzpq_p8:
** ...
** uzp1	v0\.16b, .+
** uzp2	v1\.16b, .+
** ret
*/
TEST_BINARY (vuzpq_p8, poly8x16x2_t, poly8x16_t, poly8x16_t)

/*
** test_vuzpq_mf8:
** ...
** uzp1	v0\.16b, .+
** uzp2	v1\.16b, .+
** ret
*/
TEST_BINARY (vuzpq_mf8, mfloat8x16x2_t, mfloat8x16_t, mfloat8x16_t)

/*
** test_vuzpq_u16:
** ...
** uzp1	v0\.8h, .+
** uzp2	v1\.8h, .+
** ret
*/
TEST_BINARY (vuzpq_u16, uint16x8x2_t, uint16x8_t, uint16x8_t)

/*
** test_vuzpq_s16:
** ...
** uzp1	v0\.8h, .+
** uzp2	v1\.8h, .+
** ret
*/
TEST_BINARY (vuzpq_s16, int16x8x2_t, int16x8_t, int16x8_t)

/*
** test_vuzpq_f16:
** ...
** uzp1	v0\.8h, .+
** uzp2	v1\.8h, .+
** ret
*/
TEST_BINARY (vuzpq_f16, float16x8x2_t, float16x8_t, float16x8_t)

/*
** test_vuzpq_p16:
** ...
** uzp1	v0\.8h, .+
** uzp2	v1\.8h, .+
** ret
*/
TEST_BINARY (vuzpq_p16, poly16x8x2_t, poly16x8_t, poly16x8_t)

/*
** test_vuzpq_u32:
** ...
** uzp1	v0\.4s, .+
** uzp2	v1\.4s, .+
** ret
*/
TEST_BINARY (vuzpq_u32, uint32x4x2_t, uint32x4_t, uint32x4_t)

/*
** test_vuzpq_s32:
** ...
** uzp1	v0\.4s, .+
** uzp2	v1\.4s, .+
** ret
*/
TEST_BINARY (vuzpq_s32, int32x4x2_t, int32x4_t, int32x4_t)

/*
** test_vuzpq_f32:
** ...
** uzp1	v0\.4s, .+
** uzp2	v1\.4s, .+
** ret
*/
TEST_BINARY (vuzpq_f32, float32x4x2_t, float32x4_t, float32x4_t)
