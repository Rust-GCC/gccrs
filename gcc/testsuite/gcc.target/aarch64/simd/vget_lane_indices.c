/* { dg-do assemble } */

#include <arm_neon.h>

#define VEC_LEN(VEC) (sizeof (VEC) / sizeof ((VEC)[0]))

#define TEST_VGET_LANE(NAME, RET_TYPE, ARG_TYPE)                               \
  RET_TYPE test_##NAME (ARG_TYPE arg) { return NAME (arg, VEC_LEN (arg)); }

TEST_VGET_LANE (vget_lane_u8, uint8_t, uint8x8_t) /* { dg-error {passing [0-9]+ to argument [0-9]+ of 'vget_lane_[a-z0-9]+', [^\n]+} } */
TEST_VGET_LANE (vget_lane_s8, int8_t, int8x8_t)   /* { dg-error {passing [0-9]+ to argument [0-9]+ of 'vget_lane_[a-z0-9]+', [^\n]+} } */
TEST_VGET_LANE (vget_lane_p8, poly8_t, poly8x8_t) /* { dg-error {passing [0-9]+ to argument [0-9]+ of 'vget_lane_[a-z0-9]+', [^\n]+} } */

TEST_VGET_LANE (vget_lane_u16, uint16_t, uint16x4_t)      /* { dg-error {passing [0-9]+ to argument [0-9]+ of 'vget_lane_[a-z0-9]+', [^\n]+} } */
TEST_VGET_LANE (vget_lane_s16, int16_t, int16x4_t)        /* { dg-error {passing [0-9]+ to argument [0-9]+ of 'vget_lane_[a-z0-9]+', [^\n]+} } */
TEST_VGET_LANE (vget_lane_p16, poly16_t, poly16x4_t)      /* { dg-error {passing [0-9]+ to argument [0-9]+ of 'vget_lane_[a-z0-9]+', [^\n]+} } */
TEST_VGET_LANE (vget_lane_f16, float16_t, float16x4_t)    /* { dg-error {passing [0-9]+ to argument [0-9]+ of 'vget_lane_[a-z0-9]+', [^\n]+} } */
TEST_VGET_LANE (vget_lane_bf16, bfloat16_t, bfloat16x4_t) /* { dg-error {passing [0-9]+ to argument [0-9]+ of 'vget_lane_[a-z0-9]+', [^\n]+} } */

TEST_VGET_LANE (vget_lane_u32, uint32_t, uint32x2_t)   /* { dg-error {passing [0-9]+ to argument [0-9]+ of 'vget_lane_[a-z0-9]+', [^\n]+} } */
TEST_VGET_LANE (vget_lane_s32, int32_t, int32x2_t)     /* { dg-error {passing [0-9]+ to argument [0-9]+ of 'vget_lane_[a-z0-9]+', [^\n]+} } */
TEST_VGET_LANE (vget_lane_f32, float32_t, float32x2_t) /* { dg-error {passing [0-9]+ to argument [0-9]+ of 'vget_lane_[a-z0-9]+', [^\n]+} } */

TEST_VGET_LANE (vget_lane_u64, uint64_t, uint64x1_t)   /* { dg-error {passing [0-9]+ to argument [0-9]+ of 'vget_lane_[a-z0-9]+', [^\n]+} } */
TEST_VGET_LANE (vget_lane_s64, int64_t, int64x1_t)     /* { dg-error {passing [0-9]+ to argument [0-9]+ of 'vget_lane_[a-z0-9]+', [^\n]+} } */
TEST_VGET_LANE (vget_lane_f64, float64_t, float64x1_t) /* { dg-error {passing [0-9]+ to argument [0-9]+ of 'vget_lane_[a-z0-9]+', [^\n]+} } */
TEST_VGET_LANE (vget_lane_p64, poly64_t, poly64x1_t)   /* { dg-error {passing [0-9]+ to argument [0-9]+ of 'vget_lane_[a-z0-9]+', [^\n]+} } */

TEST_VGET_LANE (vgetq_lane_u8, uint8_t, uint8x16_t) /* { dg-error {passing [0-9]+ to argument [0-9]+ of 'vgetq_lane_[a-z0-9]+', [^\n]+} } */
TEST_VGET_LANE (vgetq_lane_s8, int8_t, int8x16_t)   /* { dg-error {passing [0-9]+ to argument [0-9]+ of 'vgetq_lane_[a-z0-9]+', [^\n]+} } */
TEST_VGET_LANE (vgetq_lane_p8, poly8_t, poly8x16_t) /* { dg-error {passing [0-9]+ to argument [0-9]+ of 'vgetq_lane_[a-z0-9]+', [^\n]+} } */

TEST_VGET_LANE (vgetq_lane_u16, uint16_t, uint16x8_t)      /* { dg-error {passing [0-9]+ to argument [0-9]+ of 'vgetq_lane_[a-z0-9]+', [^\n]+} } */
TEST_VGET_LANE (vgetq_lane_s16, int16_t, int16x8_t)        /* { dg-error {passing [0-9]+ to argument [0-9]+ of 'vgetq_lane_[a-z0-9]+', [^\n]+} } */
TEST_VGET_LANE (vgetq_lane_p16, poly16_t, poly16x8_t)      /* { dg-error {passing [0-9]+ to argument [0-9]+ of 'vgetq_lane_[a-z0-9]+', [^\n]+} } */
TEST_VGET_LANE (vgetq_lane_f16, float16_t, float16x8_t)    /* { dg-error {passing [0-9]+ to argument [0-9]+ of 'vgetq_lane_[a-z0-9]+', [^\n]+} } */
TEST_VGET_LANE (vgetq_lane_bf16, bfloat16_t, bfloat16x8_t) /* { dg-error {passing [0-9]+ to argument [0-9]+ of 'vgetq_lane_[a-z0-9]+', [^\n]+} } */

TEST_VGET_LANE (vgetq_lane_u32, uint32_t, uint32x4_t)   /* { dg-error {passing [0-9]+ to argument [0-9]+ of 'vgetq_lane_[a-z0-9]+', [^\n]+} } */
TEST_VGET_LANE (vgetq_lane_s32, int32_t, int32x4_t)     /* { dg-error {passing [0-9]+ to argument [0-9]+ of 'vgetq_lane_[a-z0-9]+', [^\n]+} } */
TEST_VGET_LANE (vgetq_lane_f32, float32_t, float32x4_t) /* { dg-error {passing [0-9]+ to argument [0-9]+ of 'vgetq_lane_[a-z0-9]+', [^\n]+} } */

TEST_VGET_LANE (vgetq_lane_u64, uint64_t, uint64x2_t)   /* { dg-error {passing [0-9]+ to argument [0-9]+ of 'vgetq_lane_[a-z0-9]+', [^\n]+} } */
TEST_VGET_LANE (vgetq_lane_s64, int64_t, int64x2_t)     /* { dg-error {passing [0-9]+ to argument [0-9]+ of 'vgetq_lane_[a-z0-9]+', [^\n]+} } */
TEST_VGET_LANE (vgetq_lane_f64, float64_t, float64x2_t) /* { dg-error {passing [0-9]+ to argument [0-9]+ of 'vgetq_lane_[a-z0-9]+', [^\n]+} } */
TEST_VGET_LANE (vgetq_lane_p64, poly64_t, poly64x2_t)   /* { dg-error {passing [0-9]+ to argument [0-9]+ of 'vgetq_lane_[a-z0-9]+', [^\n]+} } */