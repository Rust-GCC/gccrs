/* { dg-do assemble } */

#include <arm_neon.h>

#define VEC_LEN(VEC) (sizeof (VEC) / sizeof ((VEC)[0]))

#define TEST_VCOPY_LANE(NAME, ARG1_TYPE, ARG2_TYPE) \
  ARG1_TYPE test_##NAME (ARG1_TYPE arg1, ARG2_TYPE arg2) { return NAME (arg1, VEC_LEN (arg1), arg2, VEC_LEN (arg2)); }

TEST_VCOPY_LANE (vcopy_lane_u8, uint8x8_t, uint8x8_t)         /* { dg-error {passing [0-9]+ to argument [0-9]+ of 'vcopy_lane_[a-z0-9]+', [^\n]+} } */
TEST_VCOPY_LANE (vcopy_lane_s8, int8x8_t, int8x8_t)           /* { dg-error {passing [0-9]+ to argument [0-9]+ of 'vcopy_lane_[a-z0-9]+', [^\n]+} } */
TEST_VCOPY_LANE (vcopy_lane_p8, poly8x8_t, poly8x8_t)         /* { dg-error {passing [0-9]+ to argument [0-9]+ of 'vcopy_lane_[a-z0-9]+', [^\n]+} } */
TEST_VCOPY_LANE (vcopy_lane_u16, uint16x4_t, uint16x4_t)      /* { dg-error {passing [0-9]+ to argument [0-9]+ of 'vcopy_lane_[a-z0-9]+', [^\n]+} } */
TEST_VCOPY_LANE (vcopy_lane_s16, int16x4_t, int16x4_t)        /* { dg-error {passing [0-9]+ to argument [0-9]+ of 'vcopy_lane_[a-z0-9]+', [^\n]+} } */
TEST_VCOPY_LANE (vcopy_lane_p16, poly16x4_t, poly16x4_t)      /* { dg-error {passing [0-9]+ to argument [0-9]+ of 'vcopy_lane_[a-z0-9]+', [^\n]+} } */
TEST_VCOPY_LANE (vcopy_lane_bf16, bfloat16x4_t, bfloat16x4_t) /* { dg-error {passing [0-9]+ to argument [0-9]+ of 'vcopy_lane_[a-z0-9]+', [^\n]+} } */
TEST_VCOPY_LANE (vcopy_lane_u32, uint32x2_t, uint32x2_t)      /* { dg-error {passing [0-9]+ to argument [0-9]+ of 'vcopy_lane_[a-z0-9]+', [^\n]+} } */
TEST_VCOPY_LANE (vcopy_lane_s32, int32x2_t, int32x2_t)        /* { dg-error {passing [0-9]+ to argument [0-9]+ of 'vcopy_lane_[a-z0-9]+', [^\n]+} } */
TEST_VCOPY_LANE (vcopy_lane_f32, float32x2_t, float32x2_t)    /* { dg-error {passing [0-9]+ to argument [0-9]+ of 'vcopy_lane_[a-z0-9]+', [^\n]+} } */
TEST_VCOPY_LANE (vcopy_lane_u64, uint64x1_t, uint64x1_t)      /* { dg-error {passing [0-9]+ to argument [0-9]+ of 'vcopy_lane_[a-z0-9]+', [^\n]+} } */
TEST_VCOPY_LANE (vcopy_lane_s64, int64x1_t, int64x1_t)        /* { dg-error {passing [0-9]+ to argument [0-9]+ of 'vcopy_lane_[a-z0-9]+', [^\n]+} } */
TEST_VCOPY_LANE (vcopy_lane_f64, float64x1_t, float64x1_t)    /* { dg-error {passing [0-9]+ to argument [0-9]+ of 'vcopy_lane_[a-z0-9]+', [^\n]+} } */
TEST_VCOPY_LANE (vcopy_lane_p64, poly64x1_t, poly64x1_t)      /* { dg-error {passing [0-9]+ to argument [0-9]+ of 'vcopy_lane_[a-z0-9]+', [^\n]+} } */

TEST_VCOPY_LANE (vcopy_laneq_u8, uint8x8_t, uint8x16_t)        /* { dg-error {passing [0-9]+ to argument [0-9]+ of 'vcopy_laneq_[a-z0-9]+', [^\n]+} } */
TEST_VCOPY_LANE (vcopy_laneq_s8, int8x8_t, int8x16_t)          /* { dg-error {passing [0-9]+ to argument [0-9]+ of 'vcopy_laneq_[a-z0-9]+', [^\n]+} } */
TEST_VCOPY_LANE (vcopy_laneq_p8, poly8x8_t, poly8x16_t)        /* { dg-error {passing [0-9]+ to argument [0-9]+ of 'vcopy_laneq_[a-z0-9]+', [^\n]+} } */
TEST_VCOPY_LANE (vcopy_laneq_u16, uint16x4_t, uint16x8_t)      /* { dg-error {passing [0-9]+ to argument [0-9]+ of 'vcopy_laneq_[a-z0-9]+', [^\n]+} } */
TEST_VCOPY_LANE (vcopy_laneq_s16, int16x4_t, int16x8_t)        /* { dg-error {passing [0-9]+ to argument [0-9]+ of 'vcopy_laneq_[a-z0-9]+', [^\n]+} } */
TEST_VCOPY_LANE (vcopy_laneq_p16, poly16x4_t, poly16x8_t)      /* { dg-error {passing [0-9]+ to argument [0-9]+ of 'vcopy_laneq_[a-z0-9]+', [^\n]+} } */
TEST_VCOPY_LANE (vcopy_laneq_bf16, bfloat16x4_t, bfloat16x8_t) /* { dg-error {passing [0-9]+ to argument [0-9]+ of 'vcopy_laneq_[a-z0-9]+', [^\n]+} } */
TEST_VCOPY_LANE (vcopy_laneq_u32, uint32x2_t, uint32x4_t)      /* { dg-error {passing [0-9]+ to argument [0-9]+ of 'vcopy_laneq_[a-z0-9]+', [^\n]+} } */
TEST_VCOPY_LANE (vcopy_laneq_s32, int32x2_t, int32x4_t)        /* { dg-error {passing [0-9]+ to argument [0-9]+ of 'vcopy_laneq_[a-z0-9]+', [^\n]+} } */
TEST_VCOPY_LANE (vcopy_laneq_f32, float32x2_t, float32x4_t)    /* { dg-error {passing [0-9]+ to argument [0-9]+ of 'vcopy_laneq_[a-z0-9]+', [^\n]+} } */
TEST_VCOPY_LANE (vcopy_laneq_u64, uint64x1_t, uint64x2_t)      /* { dg-error {passing [0-9]+ to argument [0-9]+ of 'vcopy_laneq_[a-z0-9]+', [^\n]+} } */
TEST_VCOPY_LANE (vcopy_laneq_s64, int64x1_t, int64x2_t)        /* { dg-error {passing [0-9]+ to argument [0-9]+ of 'vcopy_laneq_[a-z0-9]+', [^\n]+} } */
TEST_VCOPY_LANE (vcopy_laneq_f64, float64x1_t, float64x2_t)    /* { dg-error {passing [0-9]+ to argument [0-9]+ of 'vcopy_laneq_[a-z0-9]+', [^\n]+} } */
TEST_VCOPY_LANE (vcopy_laneq_p64, poly64x1_t, poly64x2_t)      /* { dg-error {passing [0-9]+ to argument [0-9]+ of 'vcopy_laneq_[a-z0-9]+', [^\n]+} } */

TEST_VCOPY_LANE (vcopyq_lane_u8, uint8x16_t, uint8x8_t)        /* { dg-error {passing [0-9]+ to argument [0-9]+ of 'vcopyq_lane_[a-z0-9]+', [^\n]+} } */
TEST_VCOPY_LANE (vcopyq_lane_s8, int8x16_t, int8x8_t)          /* { dg-error {passing [0-9]+ to argument [0-9]+ of 'vcopyq_lane_[a-z0-9]+', [^\n]+} } */
TEST_VCOPY_LANE (vcopyq_lane_p8, poly8x16_t, poly8x8_t)        /* { dg-error {passing [0-9]+ to argument [0-9]+ of 'vcopyq_lane_[a-z0-9]+', [^\n]+} } */
TEST_VCOPY_LANE (vcopyq_lane_u16, uint16x8_t, uint16x4_t)      /* { dg-error {passing [0-9]+ to argument [0-9]+ of 'vcopyq_lane_[a-z0-9]+', [^\n]+} } */
TEST_VCOPY_LANE (vcopyq_lane_s16, int16x8_t, int16x4_t)        /* { dg-error {passing [0-9]+ to argument [0-9]+ of 'vcopyq_lane_[a-z0-9]+', [^\n]+} } */
TEST_VCOPY_LANE (vcopyq_lane_p16, poly16x8_t, poly16x4_t)      /* { dg-error {passing [0-9]+ to argument [0-9]+ of 'vcopyq_lane_[a-z0-9]+', [^\n]+} } */
TEST_VCOPY_LANE (vcopyq_lane_bf16, bfloat16x8_t, bfloat16x4_t) /* { dg-error {passing [0-9]+ to argument [0-9]+ of 'vcopyq_lane_[a-z0-9]+', [^\n]+} } */
TEST_VCOPY_LANE (vcopyq_lane_u32, uint32x4_t, uint32x2_t)      /* { dg-error {passing [0-9]+ to argument [0-9]+ of 'vcopyq_lane_[a-z0-9]+', [^\n]+} } */
TEST_VCOPY_LANE (vcopyq_lane_s32, int32x4_t, int32x2_t)        /* { dg-error {passing [0-9]+ to argument [0-9]+ of 'vcopyq_lane_[a-z0-9]+', [^\n]+} } */
TEST_VCOPY_LANE (vcopyq_lane_f32, float32x4_t, float32x2_t)    /* { dg-error {passing [0-9]+ to argument [0-9]+ of 'vcopyq_lane_[a-z0-9]+', [^\n]+} } */
TEST_VCOPY_LANE (vcopyq_lane_u64, uint64x2_t, uint64x1_t)      /* { dg-error {passing [0-9]+ to argument [0-9]+ of 'vcopyq_lane_[a-z0-9]+', [^\n]+} } */
TEST_VCOPY_LANE (vcopyq_lane_s64, int64x2_t, int64x1_t)        /* { dg-error {passing [0-9]+ to argument [0-9]+ of 'vcopyq_lane_[a-z0-9]+', [^\n]+} } */
TEST_VCOPY_LANE (vcopyq_lane_f64, float64x2_t, float64x1_t)    /* { dg-error {passing [0-9]+ to argument [0-9]+ of 'vcopyq_lane_[a-z0-9]+', [^\n]+} } */
TEST_VCOPY_LANE (vcopyq_lane_p64, poly64x2_t, poly64x1_t)      /* { dg-error {passing [0-9]+ to argument [0-9]+ of 'vcopyq_lane_[a-z0-9]+', [^\n]+} } */

TEST_VCOPY_LANE (vcopyq_laneq_u8, uint8x16_t, uint8x16_t)       /* { dg-error {passing [0-9]+ to argument [0-9]+ of 'vcopyq_laneq_[a-z0-9]+', [^\n]+} } */
TEST_VCOPY_LANE (vcopyq_laneq_s8, int8x16_t, int8x16_t)         /* { dg-error {passing [0-9]+ to argument [0-9]+ of 'vcopyq_laneq_[a-z0-9]+', [^\n]+} } */
TEST_VCOPY_LANE (vcopyq_laneq_p8, poly8x16_t, poly8x16_t)       /* { dg-error {passing [0-9]+ to argument [0-9]+ of 'vcopyq_laneq_[a-z0-9]+', [^\n]+} } */
TEST_VCOPY_LANE (vcopyq_laneq_u16, uint16x8_t, uint16x8_t)      /* { dg-error {passing [0-9]+ to argument [0-9]+ of 'vcopyq_laneq_[a-z0-9]+', [^\n]+} } */
TEST_VCOPY_LANE (vcopyq_laneq_s16, int16x8_t, int16x8_t)        /* { dg-error {passing [0-9]+ to argument [0-9]+ of 'vcopyq_laneq_[a-z0-9]+', [^\n]+} } */
TEST_VCOPY_LANE (vcopyq_laneq_p16, poly16x8_t, poly16x8_t)      /* { dg-error {passing [0-9]+ to argument [0-9]+ of 'vcopyq_laneq_[a-z0-9]+', [^\n]+} } */
TEST_VCOPY_LANE (vcopyq_laneq_bf16, bfloat16x8_t, bfloat16x8_t) /* { dg-error {passing [0-9]+ to argument [0-9]+ of 'vcopyq_laneq_[a-z0-9]+', [^\n]+} } */
TEST_VCOPY_LANE (vcopyq_laneq_u32, uint32x4_t, uint32x4_t)      /* { dg-error {passing [0-9]+ to argument [0-9]+ of 'vcopyq_laneq_[a-z0-9]+', [^\n]+} } */
TEST_VCOPY_LANE (vcopyq_laneq_s32, int32x4_t, int32x4_t)        /* { dg-error {passing [0-9]+ to argument [0-9]+ of 'vcopyq_laneq_[a-z0-9]+', [^\n]+} } */
TEST_VCOPY_LANE (vcopyq_laneq_f32, float32x4_t, float32x4_t)    /* { dg-error {passing [0-9]+ to argument [0-9]+ of 'vcopyq_laneq_[a-z0-9]+', [^\n]+} } */
TEST_VCOPY_LANE (vcopyq_laneq_u64, uint64x2_t, uint64x2_t)      /* { dg-error {passing [0-9]+ to argument [0-9]+ of 'vcopyq_laneq_[a-z0-9]+', [^\n]+} } */
TEST_VCOPY_LANE (vcopyq_laneq_s64, int64x2_t, int64x2_t)        /* { dg-error {passing [0-9]+ to argument [0-9]+ of 'vcopyq_laneq_[a-z0-9]+', [^\n]+} } */
TEST_VCOPY_LANE (vcopyq_laneq_f64, float64x2_t, float64x2_t)    /* { dg-error {passing [0-9]+ to argument [0-9]+ of 'vcopyq_laneq_[a-z0-9]+', [^\n]+} } */
TEST_VCOPY_LANE (vcopyq_laneq_p64, poly64x2_t, poly64x2_t)      /* { dg-error {passing [0-9]+ to argument [0-9]+ of 'vcopyq_laneq_[a-z0-9]+', [^\n]+} } */
