/* { dg-do compile } */
/* { dg-final { check-function-bodies "**" "" } } */

#include "arm_neon_test.h"

#define TEST_COPY_LANE(NAME, TYPE1, TYPE2)                                     \
  TYPE1 test_##NAME (TYPE1 dst, TYPE2 src)                                     \
  {                                                                            \
    return NAME (dst, VEC_LEN (dst) - 1, src, VEC_LEN (src) - 1);              \
  }

/*
** test_vcopy_lane_p8:
** ins	v0\.b\[7\], v1\.b\[7\]
** ret
*/
TEST_COPY_LANE (vcopy_lane_p8, poly8x8_t, poly8x8_t)

/*
** test_vcopy_lane_mf8:
** ins	v0\.b\[7\], v1\.b\[7\]
** ret
*/
TEST_COPY_LANE (vcopy_lane_mf8, mfloat8x8_t, mfloat8x8_t)

/*
** test_vcopy_lane_s8:
** ins	v0\.b\[7\], v1\.b\[7\]
** ret
*/
TEST_COPY_LANE (vcopy_lane_s8, int8x8_t, int8x8_t)

/*
** test_vcopy_lane_u8:
** ins	v0\.b\[7\], v1\.b\[7\]
** ret
*/
TEST_COPY_LANE (vcopy_lane_u8, uint8x8_t, uint8x8_t)

/*
** test_vcopy_laneq_p8:
** ins	v0\.b\[7\], v1\.b\[15\]
** ret
*/
TEST_COPY_LANE (vcopy_laneq_p8, poly8x8_t, poly8x16_t)

/*
** test_vcopy_laneq_mf8:
** ins	v0\.b\[7\], v1\.b\[15\]
** ret
*/
TEST_COPY_LANE (vcopy_laneq_mf8, mfloat8x8_t, mfloat8x16_t)

/*
** test_vcopy_laneq_s8:
** ins	v0\.b\[7\], v1\.b\[15\]
** ret
*/
TEST_COPY_LANE (vcopy_laneq_s8, int8x8_t, int8x16_t)

/*
** test_vcopy_laneq_u8:
** ins	v0\.b\[7\], v1\.b\[15\]
** ret
*/
TEST_COPY_LANE (vcopy_laneq_u8, uint8x8_t, uint8x16_t)

/*
** test_vcopyq_lane_p8:
** ins	v0\.b\[15\], v1\.b\[7\]
** ret
*/
TEST_COPY_LANE (vcopyq_lane_p8, poly8x16_t, poly8x8_t)

/*
** test_vcopyq_lane_mf8:
** ins	v0\.b\[15\], v1\.b\[7\]
** ret
*/
TEST_COPY_LANE (vcopyq_lane_mf8, mfloat8x16_t, mfloat8x8_t)

/*
** test_vcopyq_lane_s8:
** ins	v0\.b\[15\], v1\.b\[7\]
** ret
*/
TEST_COPY_LANE (vcopyq_lane_s8, int8x16_t, int8x8_t)

/*
** test_vcopyq_lane_u8:
** ins	v0\.b\[15\], v1\.b\[7\]
** ret
*/
TEST_COPY_LANE (vcopyq_lane_u8, uint8x16_t, uint8x8_t)

/*
** test_vcopyq_laneq_p8:
** ins	v0\.b\[15\], v1\.b\[15\]
** ret
*/
TEST_COPY_LANE (vcopyq_laneq_p8, poly8x16_t, poly8x16_t)

/*
** test_vcopyq_laneq_mf8:
** ins	v0\.b\[15\], v1\.b\[15\]
** ret
*/
TEST_COPY_LANE (vcopyq_laneq_mf8, mfloat8x16_t, mfloat8x16_t)

/*
** test_vcopyq_laneq_s8:
** ins	v0\.b\[15\], v1\.b\[15\]
** ret
*/
TEST_COPY_LANE (vcopyq_laneq_s8, int8x16_t, int8x16_t)

/*
** test_vcopyq_laneq_u8:
** ins	v0\.b\[15\], v1\.b\[15\]
** ret
*/
TEST_COPY_LANE (vcopyq_laneq_u8, uint8x16_t, uint8x16_t)

/*
** test_vcopy_lane_bf16:
** ins	v0\.h\[3\], v1\.h\[3\]
** ret
*/
TEST_COPY_LANE (vcopy_lane_bf16, bfloat16x4_t, bfloat16x4_t)

/*
** test_vcopy_lane_p16:
** ins	v0\.h\[3\], v1\.h\[3\]
** ret
*/
TEST_COPY_LANE (vcopy_lane_p16, poly16x4_t, poly16x4_t)

/*
** test_vcopy_lane_s16:
** ins	v0\.h\[3\], v1\.h\[3\]
** ret
*/
TEST_COPY_LANE (vcopy_lane_s16, int16x4_t, int16x4_t)

/*
** test_vcopy_lane_u16:
** ins	v0\.h\[3\], v1\.h\[3\]
** ret
*/
TEST_COPY_LANE (vcopy_lane_u16, uint16x4_t, uint16x4_t)

/*
** test_vcopy_laneq_bf16:
** ins	v0\.h\[3\], v1\.h\[7\]
** ret
*/
TEST_COPY_LANE (vcopy_laneq_bf16, bfloat16x4_t, bfloat16x8_t)

/*
** test_vcopy_laneq_p16:
** ins	v0\.h\[3\], v1\.h\[7\]
** ret
*/
TEST_COPY_LANE (vcopy_laneq_p16, poly16x4_t, poly16x8_t)
/*
** test_vcopy_laneq_s16:
** ins	v0\.h\[3\], v1\.h\[7\]
** ret
*/
TEST_COPY_LANE (vcopy_laneq_s16, int16x4_t, int16x8_t)

/*
** test_vcopy_laneq_u16:
** ins	v0\.h\[3\], v1\.h\[7\]
** ret
*/
TEST_COPY_LANE (vcopy_laneq_u16, uint16x4_t, uint16x8_t)

/*
** test_vcopyq_lane_bf16:
** ins	v0\.h\[7\], v1\.h\[3\]
** ret
*/
TEST_COPY_LANE (vcopyq_lane_bf16, bfloat16x8_t, bfloat16x4_t)

/*
** test_vcopyq_lane_p16:
** ins	v0\.h\[7\], v1\.h\[3\]
** ret
*/
TEST_COPY_LANE (vcopyq_lane_p16, poly16x8_t, poly16x4_t)

/*
** test_vcopyq_lane_s16:
** ins	v0\.h\[7\], v1\.h\[3\]
** ret
*/
TEST_COPY_LANE (vcopyq_lane_s16, int16x8_t, int16x4_t)

/*
** test_vcopyq_lane_u16:
** ins	v0\.h\[7\], v1\.h\[3\]
** ret
*/
TEST_COPY_LANE (vcopyq_lane_u16, uint16x8_t, uint16x4_t)

/*
** test_vcopyq_laneq_bf16:
** ins	v0\.h\[7\], v1\.h\[7\]
** ret
*/
TEST_COPY_LANE (vcopyq_laneq_bf16, bfloat16x8_t, bfloat16x8_t)

/*
** test_vcopyq_laneq_p16:
** ins	v0\.h\[7\], v1\.h\[7\]
** ret
*/
TEST_COPY_LANE (vcopyq_laneq_p16, poly16x8_t, poly16x8_t)

/*
** test_vcopyq_laneq_s16:
** ins	v0\.h\[7\], v1\.h\[7\]
** ret
*/
TEST_COPY_LANE (vcopyq_laneq_s16, int16x8_t, int16x8_t)

/*
** test_vcopyq_laneq_u16:
** ins	v0\.h\[7\], v1\.h\[7\]
** ret
*/
TEST_COPY_LANE (vcopyq_laneq_u16, uint16x8_t, uint16x8_t)

// FIXME: these functions generate suboptimal code on big-endian:
// ins v1.s[0], v0.s[0]
// mov v0.16b, v1.16b
// ret

/*
** test_vcopy_lane_f32: { xfail { aarch64_big_endian || { any-opts "-mbig-endian" } } }
** ins	v0\.s\[1\], v1\.s\[1\]
** ret
*/
TEST_COPY_LANE (vcopy_lane_f32, float32x2_t, float32x2_t)

/*
** test_vcopy_lane_s32: { xfail { aarch64_big_endian || { any-opts "-mbig-endian" } } }
** ins	v0\.s\[1\], v1\.s\[1\]
** ret
*/
TEST_COPY_LANE (vcopy_lane_s32, int32x2_t, int32x2_t)

/*
** test_vcopy_lane_u32: { xfail { aarch64_big_endian || { any-opts "-mbig-endian" } } }
** ins	v0\.s\[1\], v1\.s\[1\]
** ret
*/
TEST_COPY_LANE (vcopy_lane_u32, uint32x2_t, uint32x2_t)

/*
** test_vcopy_laneq_f32:
** ins	v0\.s\[1\], v1\.s\[3\]
** ret
*/
TEST_COPY_LANE (vcopy_laneq_f32, float32x2_t, float32x4_t)

/*
** test_vcopy_laneq_s32:
** ins	v0\.s\[1\], v1\.s\[3\]
** ret
*/
TEST_COPY_LANE (vcopy_laneq_s32, int32x2_t, int32x4_t)

/*
** test_vcopy_laneq_u32:
** ins	v0\.s\[1\], v1\.s\[3\]
** ret
*/
TEST_COPY_LANE (vcopy_laneq_u32, uint32x2_t, uint32x4_t)

/*
** test_vcopyq_lane_f32:
** ins	v0\.s\[3\], v1\.s\[1\]
** ret
*/
TEST_COPY_LANE (vcopyq_lane_f32, float32x4_t, float32x2_t)

/*
** test_vcopyq_lane_s32:
** ins	v0\.s\[3\], v1\.s\[1\]
** ret
*/
TEST_COPY_LANE (vcopyq_lane_s32, int32x4_t, int32x2_t)

/*
** test_vcopyq_lane_u32:
** ins	v0\.s\[3\], v1\.s\[1\]
** ret
*/
TEST_COPY_LANE (vcopyq_lane_u32, uint32x4_t, uint32x2_t)

/*
** test_vcopyq_laneq_f32:
** ins	v0\.s\[3\], v1\.s\[3\]
** ret
*/
TEST_COPY_LANE (vcopyq_laneq_f32, float32x4_t, float32x4_t)

/*
** test_vcopyq_laneq_s32:
** ins	v0\.s\[3\], v1\.s\[3\]
** ret
*/
TEST_COPY_LANE (vcopyq_laneq_s32, int32x4_t, int32x4_t)

/*
** test_vcopyq_laneq_u32:
** ins	v0\.s\[3\], v1\.s\[3\]
** ret
*/
TEST_COPY_LANE (vcopyq_laneq_u32, uint32x4_t, uint32x4_t)

/*
** test_vcopy_lane_f64:
** fmov	d0, d1
** ret
*/
TEST_COPY_LANE (vcopy_lane_f64, float64x1_t, float64x1_t)

/*
** test_vcopy_lane_p64:
** fmov	d0, d1
** ret
*/
TEST_COPY_LANE (vcopy_lane_p64, poly64x1_t, poly64x1_t)

/*
** test_vcopy_lane_s64:
** fmov	d0, d1
** ret
*/
TEST_COPY_LANE (vcopy_lane_s64, int64x1_t, int64x1_t)

/*
** test_vcopy_lane_u64:
** fmov	d0, d1
** ret
*/
TEST_COPY_LANE (vcopy_lane_u64, uint64x1_t, uint64x1_t)

/*
** test_vcopy_laneq_f64:
** dup	d0, v1\.d\[1\]
** ret
*/
TEST_COPY_LANE (vcopy_laneq_f64, float64x1_t, float64x2_t)

/*
** test_vcopy_laneq_p64:
** dup	d0, v1\.d\[1\]
** ret
*/
TEST_COPY_LANE (vcopy_laneq_p64, poly64x1_t, poly64x2_t)
/*
** test_vcopy_laneq_s64:
** dup	d0, v1\.d\[1\]
** ret
*/
TEST_COPY_LANE (vcopy_laneq_s64, int64x1_t, int64x2_t)

/*
** test_vcopy_laneq_u64:
** dup	d0, v1\.d\[1\]
** ret
*/
TEST_COPY_LANE (vcopy_laneq_u64, uint64x1_t, uint64x2_t)

/*
** test_vcopyq_lane_f64:
** ins	v0\.d\[1\], v1\.d\[0\]
** ret
*/
TEST_COPY_LANE (vcopyq_lane_f64, float64x2_t, float64x1_t)

/*
** test_vcopyq_lane_u64:
** ins	v0\.d\[1\], v1\.d\[0\]
** ret
*/
TEST_COPY_LANE (vcopyq_lane_p64, poly64x2_t, poly64x1_t)

/*
** test_vcopyq_lane_u64:
** ins	v0\.d\[1\], v1\.d\[0\]
** ret
*/
TEST_COPY_LANE (vcopyq_lane_s64, int64x2_t, int64x1_t)

/*
** test_vcopyq_lane_u64:
** ins	v0\.d\[1\], v1\.d\[0\]
** ret
*/
TEST_COPY_LANE (vcopyq_lane_u64, uint64x2_t, uint64x1_t)

// FIXME: these functions generate suboptimal code on big-endian:
// ins v1.d[0], v0.d[0]
// mov v0.16b, v1.16b
// ret

/*
** test_vcopyq_laneq_f64: { xfail { aarch64_big_endian || { any-opts "-mbig-endian" } } }
** ins	v0\.d\[1\], v1\.d\[1\]
** ret
*/
TEST_COPY_LANE (vcopyq_laneq_f64, float64x2_t, float64x2_t)

/*
** test_vcopyq_laneq_p64: { xfail { aarch64_big_endian || { any-opts "-mbig-endian" } } }
** ins	v0\.d\[1\], v1\.d\[1\]
** ret
*/
TEST_COPY_LANE (vcopyq_laneq_p64, poly64x2_t, poly64x2_t)

/*
** test_vcopyq_laneq_s64: { xfail { aarch64_big_endian || { any-opts "-mbig-endian" } } }
** ins	v0\.d\[1\], v1\.d\[1\]
** ret
*/
TEST_COPY_LANE (vcopyq_laneq_s64, int64x2_t, int64x2_t)

/*
** test_vcopyq_laneq_u64: { xfail { aarch64_big_endian || { any-opts "-mbig-endian" } } }
** ins	v0\.d\[1\], v1\.d\[1\]
** ret
*/
TEST_COPY_LANE (vcopyq_laneq_u64, uint64x2_t, uint64x2_t)
