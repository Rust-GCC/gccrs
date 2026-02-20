/* { dg-do compile } */
/* { dg-final { check-function-bodies "**" "" } } */

#include "arm_neon_test.h"

#define TEST_EXT(NAME, TYPE, INDEX) \
  TYPE test_##NAME##_##INDEX (TYPE a, TYPE b) { return NAME (a, b, INDEX); }

/*
** test_vext_u8_1:
** ext	v0\.8b, v0\.8b, v1\.8b, #?1
** ret
*/
TEST_EXT (vext_u8, uint8x8_t, 1)

/*
** test_vext_s8_1:
** ext	v0\.8b, v0\.8b, v1\.8b, #?1
** ret
*/
TEST_EXT (vext_s8, int8x8_t, 1)

/*
** test_vext_p8_1:
** ext	v0\.8b, v0\.8b, v1\.8b, #?1
** ret
*/
TEST_EXT (vext_p8, poly8x8_t, 1)

/*
** test_vext_mf8_1:
** ext	v0\.8b, v0\.8b, v1\.8b, #?1
** ret
*/
TEST_EXT (vext_mf8, mfloat8x8_t, 1)

/*
** test_vext_u16_1:
** ext	v0\.8b, v0\.8b, v1\.8b, #?2
** ret
*/
TEST_EXT (vext_u16, uint16x4_t, 1)

/*
** test_vext_s16_1:
** ext	v0\.8b, v0\.8b, v1\.8b, #?2
** ret
*/
TEST_EXT (vext_s16, int16x4_t, 1)

/*
** test_vext_f16_1:
** ext	v0\.8b, v0\.8b, v1\.8b, #?2
** ret
*/
TEST_EXT (vext_f16, float16x4_t, 1)

/*
** test_vext_p16_1:
** ext	v0\.8b, v0\.8b, v1\.8b, #?2
** ret
*/
TEST_EXT (vext_p16, poly16x4_t, 1)

/*
** test_vext_u32_1:
** ext	v0\.8b, v0\.8b, v1\.8b, #?4
** ret
*/
TEST_EXT (vext_u32, uint32x2_t, 1)

/*
** test_vext_s32_1:
** ext	v0\.8b, v0\.8b, v1\.8b, #?4
** ret
*/
TEST_EXT (vext_s32, int32x2_t, 1)

/*
** test_vext_f32_1:
** ext	v0\.8b, v0\.8b, v1\.8b, #?4
** ret
*/
TEST_EXT (vext_f32, float32x2_t, 1)

/*
** test_vext_u64_0:
** ret
*/
TEST_EXT (vext_u64, uint64x1_t, 0)

/* The only allowable index for vext_{u64,s64,f64} is 0, which just returns the
   whole `a` vector. So no instructions are emitted.  */

/*
** test_vext_s64_0:
** ret
*/
TEST_EXT (vext_s64, int64x1_t, 0)

/*
** test_vext_f64_0:
** ret
*/
TEST_EXT (vext_f64, float64x1_t, 0)

/*
** test_vext_p64_0:
** ret
*/
TEST_EXT (vext_p64, poly64x1_t, 0)

/*
** test_vextq_u8_1:
** ext	v0\.16b, v0\.16b, v1\.16b, #?1
** ret
*/
TEST_EXT (vextq_u8, uint8x16_t, 1)

/*
** test_vextq_s8_1:
** ext	v0\.16b, v0\.16b, v1\.16b, #?1
** ret
*/
TEST_EXT (vextq_s8, int8x16_t, 1)

/*
** test_vextq_p8_1:
** ext	v0\.16b, v0\.16b, v1\.16b, #?1
** ret
*/
TEST_EXT (vextq_p8, poly8x16_t, 1)

/*
** test_vextq_mf8_1:
** ext	v0\.16b, v0\.16b, v1\.16b, #?1
** ret
*/
TEST_EXT (vextq_mf8, mfloat8x16_t, 1)

/*
** test_vextq_u16_1:
** ext	v0\.16b, v0\.16b, v1\.16b, #?2
** ret
*/
TEST_EXT (vextq_u16, uint16x8_t, 1)

/*
** test_vextq_s16_1:
** ext	v0\.16b, v0\.16b, v1\.16b, #?2
** ret
*/
TEST_EXT (vextq_s16, int16x8_t, 1)

/*
** test_vextq_f16_1:
** ext	v0\.16b, v0\.16b, v1\.16b, #?2
** ret
*/
TEST_EXT (vextq_f16, float16x8_t, 1)

/*
** test_vextq_p16_1:
** ext	v0\.16b, v0\.16b, v1\.16b, #?2
** ret
*/
TEST_EXT (vextq_p16, poly16x8_t, 1)

/*
** test_vextq_u32_1:
** ext	v0\.16b, v0\.16b, v1\.16b, #?4
** ret
*/
TEST_EXT (vextq_u32, uint32x4_t, 1)

/*
** test_vextq_s32_1:
** ext	v0\.16b, v0\.16b, v1\.16b, #?4
** ret
*/
TEST_EXT (vextq_s32, int32x4_t, 1)

/*
** test_vextq_f32_1:
** ext	v0\.16b, v0\.16b, v1\.16b, #?4
** ret
*/
TEST_EXT (vextq_f32, float32x4_t, 1)

/*
** test_vextq_u64_1:
** ext	v0\.16b, v0\.16b, v1\.16b, #?8
** ret
*/
TEST_EXT (vextq_u64, uint64x2_t, 1)

/*
** test_vextq_s64_1:
** ext	v0\.16b, v0\.16b, v1\.16b, #?8
** ret
*/
TEST_EXT (vextq_s64, int64x2_t, 1)

/*
** test_vextq_f64_1:
** ext	v0\.16b, v0\.16b, v1\.16b, #?8
** ret
*/
TEST_EXT (vextq_f64, float64x2_t, 1)

/*
** test_vextq_p64_1:
** ext	v0\.16b, v0\.16b, v1\.16b, #?8
** ret
*/
TEST_EXT (vextq_p64, poly64x2_t, 1)
