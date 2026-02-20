/* { dg-do compile } */
/* { dg-final { check-function-bodies "**" "" } } */

#include "arm_neon_test.h"

/*
** test_vzip1_u8:
** zip1	v0\.8b, v0\.8b, v1\.8b
** ret
*/
TEST_UNIFORM_BINARY (vzip1_u8, uint8x8_t)

/*
** test_vzip1_s8:
** zip1	v0\.8b, v0\.8b, v1\.8b
** ret
*/
TEST_UNIFORM_BINARY (vzip1_s8, int8x8_t)

/*
** test_vzip1_p8:
** zip1	v0\.8b, v0\.8b, v1\.8b
** ret
*/
TEST_UNIFORM_BINARY (vzip1_p8, poly8x8_t)

/*
** test_vzip1_mf8:
** zip1	v0\.8b, v0\.8b, v1\.8b
** ret
*/
TEST_UNIFORM_BINARY (vzip1_mf8, mfloat8x8_t)

/*
** test_vzip1_u16:
** zip1	v0\.4h, v0\.4h, v1\.4h
** ret
*/
TEST_UNIFORM_BINARY (vzip1_u16, uint16x4_t)

/*
** test_vzip1_s16:
** zip1	v0\.4h, v0\.4h, v1\.4h
** ret
*/
TEST_UNIFORM_BINARY (vzip1_s16, int16x4_t)

/*
** test_vzip1_f16:
** zip1	v0\.4h, v0\.4h, v1\.4h
** ret
*/
TEST_UNIFORM_BINARY (vzip1_f16, float16x4_t)

/*
** test_vzip1_p16:
** zip1	v0\.4h, v0\.4h, v1\.4h
** ret
*/
TEST_UNIFORM_BINARY (vzip1_p16, poly16x4_t)

/*
** test_vzip1_u32:
** zip1	v0\.2s, v0\.2s, v1\.2s
** ret
*/
TEST_UNIFORM_BINARY (vzip1_u32, uint32x2_t)

/*
** test_vzip1_s32:
** zip1	v0\.2s, v0\.2s, v1\.2s
** ret
*/
TEST_UNIFORM_BINARY (vzip1_s32, int32x2_t)

/*
** test_vzip1_f32:
** zip1	v0\.2s, v0\.2s, v1\.2s
** ret
*/
TEST_UNIFORM_BINARY (vzip1_f32, float32x2_t)

/*
** test_vzip1q_u8:
** zip1	v0\.16b, v0\.16b, v1\.16b
** ret
*/
TEST_UNIFORM_BINARY (vzip1q_u8, uint8x16_t)

/*
** test_vzip1q_s8:
** zip1	v0\.16b, v0\.16b, v1\.16b
** ret
*/
TEST_UNIFORM_BINARY (vzip1q_s8, int8x16_t)

/*
** test_vzip1q_p8:
** zip1	v0\.16b, v0\.16b, v1\.16b
** ret
*/
TEST_UNIFORM_BINARY (vzip1q_p8, poly8x16_t)

/*
** test_vzip1q_mf8:
** zip1	v0\.16b, v0\.16b, v1\.16b
** ret
*/
TEST_UNIFORM_BINARY (vzip1q_mf8, mfloat8x16_t)

/*
** test_vzip1q_u16:
** zip1	v0\.8h, v0\.8h, v1\.8h
** ret
*/
TEST_UNIFORM_BINARY (vzip1q_u16, uint16x8_t)

/*
** test_vzip1q_s16:
** zip1	v0\.8h, v0\.8h, v1\.8h
** ret
*/
TEST_UNIFORM_BINARY (vzip1q_s16, int16x8_t)

/*
** test_vzip1q_f16:
** zip1	v0\.8h, v0\.8h, v1\.8h
** ret
*/
TEST_UNIFORM_BINARY (vzip1q_f16, float16x8_t)

/*
** test_vzip1q_p16:
** zip1	v0\.8h, v0\.8h, v1\.8h
** ret
*/
TEST_UNIFORM_BINARY (vzip1q_p16, poly16x8_t)

/*
** test_vzip1q_u32:
** zip1	v0\.4s, v0\.4s, v1\.4s
** ret
*/
TEST_UNIFORM_BINARY (vzip1q_u32, uint32x4_t)

/*
** test_vzip1q_s32:
** zip1	v0\.4s, v0\.4s, v1\.4s
** ret
*/
TEST_UNIFORM_BINARY (vzip1q_s32, int32x4_t)

/*
** test_vzip1q_f32:
** zip1	v0\.4s, v0\.4s, v1\.4s
** ret
*/
TEST_UNIFORM_BINARY (vzip1q_f32, float32x4_t)

/*
** test_vzip1q_u64:
** zip1	v0\.2d, v0\.2d, v1\.2d
** ret
*/
TEST_UNIFORM_BINARY (vzip1q_u64, uint64x2_t)

/*
** test_vzip1q_s64:
** zip1	v0\.2d, v0\.2d, v1\.2d
** ret
*/
TEST_UNIFORM_BINARY (vzip1q_s64, int64x2_t)

/*
** test_vzip1q_f64:
** zip1	v0\.2d, v0\.2d, v1\.2d
** ret
*/
TEST_UNIFORM_BINARY (vzip1q_f64, float64x2_t)

/*
** test_vzip1q_p64:
** zip1	v0\.2d, v0\.2d, v1\.2d
** ret
*/
TEST_UNIFORM_BINARY (vzip1q_p64, poly64x2_t)

/*
** test_vzip2_u8:
** zip2	v0\.8b, v0\.8b, v1\.8b
** ret
*/
TEST_UNIFORM_BINARY (vzip2_u8, uint8x8_t)

/*
** test_vzip2_s8:
** zip2	v0\.8b, v0\.8b, v1\.8b
** ret
*/
TEST_UNIFORM_BINARY (vzip2_s8, int8x8_t)

/*
** test_vzip2_p8:
** zip2	v0\.8b, v0\.8b, v1\.8b
** ret
*/
TEST_UNIFORM_BINARY (vzip2_p8, poly8x8_t)

/*
** test_vzip2_mf8:
** zip2	v0\.8b, v0\.8b, v1\.8b
** ret
*/
TEST_UNIFORM_BINARY (vzip2_mf8, mfloat8x8_t)

/*
** test_vzip2_u16:
** zip2	v0\.4h, v0\.4h, v1\.4h
** ret
*/
TEST_UNIFORM_BINARY (vzip2_u16, uint16x4_t)

/*
** test_vzip2_s16:
** zip2	v0\.4h, v0\.4h, v1\.4h
** ret
*/
TEST_UNIFORM_BINARY (vzip2_s16, int16x4_t)

/*
** test_vzip2_f16:
** zip2	v0\.4h, v0\.4h, v1\.4h
** ret
*/
TEST_UNIFORM_BINARY (vzip2_f16, float16x4_t)

/*
** test_vzip2_p16:
** zip2	v0\.4h, v0\.4h, v1\.4h
** ret
*/
TEST_UNIFORM_BINARY (vzip2_p16, poly16x4_t)

/*
** test_vzip2_u32:
** zip2	v0\.2s, v0\.2s, v1\.2s
** ret
*/
TEST_UNIFORM_BINARY (vzip2_u32, uint32x2_t)

/*
** test_vzip2_s32:
** zip2	v0\.2s, v0\.2s, v1\.2s
** ret
*/
TEST_UNIFORM_BINARY (vzip2_s32, int32x2_t)

/*
** test_vzip2_f32:
** zip2	v0\.2s, v0\.2s, v1\.2s
** ret
*/
TEST_UNIFORM_BINARY (vzip2_f32, float32x2_t)

/*
** test_vzip2q_u8:
** zip2	v0\.16b, v0\.16b, v1\.16b
** ret
*/
TEST_UNIFORM_BINARY (vzip2q_u8, uint8x16_t)

/*
** test_vzip2q_s8:
** zip2	v0\.16b, v0\.16b, v1\.16b
** ret
*/
TEST_UNIFORM_BINARY (vzip2q_s8, int8x16_t)

/*
** test_vzip2q_mf8:
** zip2	v0\.16b, v0\.16b, v1\.16b
** ret
*/
TEST_UNIFORM_BINARY (vzip2q_mf8, mfloat8x16_t)

/*
** test_vzip2q_u16:
** zip2	v0\.8h, v0\.8h, v1\.8h
** ret
*/
TEST_UNIFORM_BINARY (vzip2q_u16, uint16x8_t)

/*
** test_vzip2q_s16:
** zip2	v0\.8h, v0\.8h, v1\.8h
** ret
*/
TEST_UNIFORM_BINARY (vzip2q_s16, int16x8_t)

/*
** test_vzip2q_f16:
** zip2	v0\.8h, v0\.8h, v1\.8h
** ret
*/
TEST_UNIFORM_BINARY (vzip2q_f16, float16x8_t)

/*
** test_vzip2q_p16:
** zip2	v0\.8h, v0\.8h, v1\.8h
** ret
*/
TEST_UNIFORM_BINARY (vzip2q_p16, poly16x8_t)

/*
** test_vzip2q_u32:
** zip2	v0\.4s, v0\.4s, v1\.4s
** ret
*/
TEST_UNIFORM_BINARY (vzip2q_u32, uint32x4_t)

/*
** test_vzip2q_s32:
** zip2	v0\.4s, v0\.4s, v1\.4s
** ret
*/
TEST_UNIFORM_BINARY (vzip2q_s32, int32x4_t)

/*
** test_vzip2q_f32:
** zip2	v0\.4s, v0\.4s, v1\.4s
** ret
*/
TEST_UNIFORM_BINARY (vzip2q_f32, float32x4_t)

/*
** test_vzip2q_u64:
** zip2	v0\.2d, v0\.2d, v1\.2d
** ret
*/
TEST_UNIFORM_BINARY (vzip2q_u64, uint64x2_t)

/*
** test_vzip2q_s64:
** zip2	v0\.2d, v0\.2d, v1\.2d
** ret
*/
TEST_UNIFORM_BINARY (vzip2q_s64, int64x2_t)

/*
** test_vzip2q_f64:
** zip2	v0\.2d, v0\.2d, v1\.2d
** ret
*/
TEST_UNIFORM_BINARY (vzip2q_f64, float64x2_t)

/*
** test_vzip2q_p64:
** zip2	v0\.2d, v0\.2d, v1\.2d
** ret
*/
TEST_UNIFORM_BINARY (vzip2q_p64, poly64x2_t)

/*
** test_vzip_u8:
** ...
** zip1	v0\.8b, .+
** zip2	v1\.8b, .+
** ret
*/
TEST_BINARY (vzip_u8, uint8x8x2_t, uint8x8_t, uint8x8_t)

/*
** test_vzip_s8:
** ...
** zip1	v0\.8b, .+
** zip2	v1\.8b, .+
** ret
*/
TEST_BINARY (vzip_s8, int8x8x2_t, int8x8_t, int8x8_t)

/*
** test_vzip_p8:
** ...
** zip1	v0\.8b, .+
** zip2	v1\.8b, .+
** ret
*/
TEST_BINARY (vzip_p8, poly8x8x2_t, poly8x8_t, poly8x8_t)

/*
** test_vzip_mf8:
** ...
** zip1	v0\.8b, .+
** zip2	v1\.8b, .+
** ret
*/
TEST_BINARY (vzip_mf8, mfloat8x8x2_t, mfloat8x8_t, mfloat8x8_t)

/*
** test_vzip_u16:
** ...
** zip1	v0\.4h, .+
** zip2	v1\.4h, .+
** ret
*/
TEST_BINARY (vzip_u16, uint16x4x2_t, uint16x4_t, uint16x4_t)

/*
** test_vzip_s16:
** ...
** zip1	v0\.4h, .+
** zip2	v1\.4h, .+
** ret
*/
TEST_BINARY (vzip_s16, int16x4x2_t, int16x4_t, int16x4_t)

/*
** test_vzip_f16:
** ...
** zip1	v0\.4h, .+
** zip2	v1\.4h, .+
** ret
*/
TEST_BINARY (vzip_f16, float16x4x2_t, float16x4_t, float16x4_t)

/*
** test_vzip_p16:
** ...
** zip1	v0\.4h, .+
** zip2	v1\.4h, .+
** ret
*/
TEST_BINARY (vzip_p16, poly16x4x2_t, poly16x4_t, poly16x4_t)

/*
** test_vzip_u32:
** ...
** zip1	v0\.2s, .+
** zip2	v1\.2s, .+
** ret
*/
TEST_BINARY (vzip_u32, uint32x2x2_t, uint32x2_t, uint32x2_t)

/*
** test_vzip_s32:
** ...
** zip1	v0\.2s, .+
** zip2	v1\.2s, .+
** ret
*/
TEST_BINARY (vzip_s32, int32x2x2_t, int32x2_t, int32x2_t)

/*
** test_vzip_f32:
** ...
** zip1	v0\.2s, .+
** zip2	v1\.2s, .+
** ret
*/
TEST_BINARY (vzip_f32, float32x2x2_t, float32x2_t, float32x2_t)

/*
** test_vzipq_u8:
** ...
** zip1	v0\.16b, .+
** zip2	v1\.16b, .+
** ret
*/
TEST_BINARY (vzipq_u8, uint8x16x2_t, uint8x16_t, uint8x16_t)

/*
** test_vzipq_s8:
** ...
** zip1	v0\.16b, .+
** zip2	v1\.16b, .+
** ret
*/
TEST_BINARY (vzipq_s8, int8x16x2_t, int8x16_t, int8x16_t)

/*
** test_vzipq_p8:
** ...
** zip1	v0\.16b, .+
** zip2	v1\.16b, .+
** ret
*/
TEST_BINARY (vzipq_p8, poly8x16x2_t, poly8x16_t, poly8x16_t)

/*
** test_vzipq_mf8:
** ...
** zip1	v0\.16b, .+
** zip2	v1\.16b, .+
** ret
*/
TEST_BINARY (vzipq_mf8, mfloat8x16x2_t, mfloat8x16_t, mfloat8x16_t)

/*
** test_vzipq_u16:
** ...
** zip1	v0\.8h, .+
** zip2	v1\.8h, .+
** ret
*/
TEST_BINARY (vzipq_u16, uint16x8x2_t, uint16x8_t, uint16x8_t)

/*
** test_vzipq_s16:
** ...
** zip1	v0\.8h, .+
** zip2	v1\.8h, .+
** ret
*/
TEST_BINARY (vzipq_s16, int16x8x2_t, int16x8_t, int16x8_t)

/*
** test_vzipq_f16:
** ...
** zip1	v0\.8h, .+
** zip2	v1\.8h, .+
** ret
*/
TEST_BINARY (vzipq_f16, float16x8x2_t, float16x8_t, float16x8_t)

/*
** test_vzipq_p16:
** ...
** zip1	v0\.8h, .+
** zip2	v1\.8h, .+
** ret
*/
TEST_BINARY (vzipq_p16, poly16x8x2_t, poly16x8_t, poly16x8_t)

/*
** test_vzipq_u32:
** ...
** zip1	v0\.4s, .+
** zip2	v1\.4s, .+
** ret
*/
TEST_BINARY (vzipq_u32, uint32x4x2_t, uint32x4_t, uint32x4_t)

/*
** test_vzipq_s32:
** ...
** zip1	v0\.4s, .+
** zip2	v1\.4s, .+
** ret
*/
TEST_BINARY (vzipq_s32, int32x4x2_t, int32x4_t, int32x4_t)

/*
** test_vzipq_f32:
** ...
** zip1	v0\.4s, .+
** zip2	v1\.4s, .+
** ret
*/
TEST_BINARY (vzipq_f32, float32x4x2_t, float32x4_t, float32x4_t)
