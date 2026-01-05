/* { dg-do compile } */
/* { dg-options "-save-temps -fno-inline -O1 -march=armv9-a+fprcvt" } */
/* { dg-final { check-function-bodies "**" "" "" } } */

#include <stdint.h>
#include <arm_fp16.h>
#include <arm_acle.h>
#include <arm_neon.h>

#define FORCE_FP_REG(a) asm volatile("fmov %d0, %d1" : "=w"(a) : "w"(a) :)
#define CONVERT_INTRINSIC(T_FROM, T_TO, INTRINSIC)                             \
  void convert_##INTRINSIC (T_FROM *from, T_TO *to)        \
  {                                                                            \
    T_FROM from_val = *from;                                                   \
    FORCE_FP_REG (from_val);                                                   \
    T_TO to_val = INTRINSIC (from_val);                                        \
    FORCE_FP_REG (to_val);                                                     \
    *to = to_val;                                                              \
  }

CONVERT_INTRINSIC (float32_t, int32_t, vcvts_s32_f32)
/*
** convert_vcvts_s32_f32:
**...
**	fcvtzs	s[0-9]+, s[0-9]+
**...
*/

CONVERT_INTRINSIC (float32_t, int64_t, vcvts_s64_f32)
/*
** convert_vcvts_s64_f32:
**...
**	fcvtzs	d[0-9]+, s[0-9]+
**...
*/

CONVERT_INTRINSIC (float32_t, uint32_t, vcvts_u32_f32)
/*
** convert_vcvts_u32_f32:
**...
**	fcvtzu	s[0-9]+, s[0-9]+
**...
*/

CONVERT_INTRINSIC (float32_t, uint64_t, vcvts_u64_f32)
/*
** convert_vcvts_u64_f32:
**...
**	fcvtzu	d[0-9]+, s[0-9]+
**...
*/

CONVERT_INTRINSIC (float32_t, int32_t, vcvtns_s32_f32)
/*
** convert_vcvtns_s32_f32:
**...
**	fcvtns	s[0-9]+, s[0-9]+
**...
*/

CONVERT_INTRINSIC (float32_t, int64_t, vcvtns_s64_f32)
/*
** convert_vcvtns_s64_f32:
**...
**	fcvtns	d[0-9]+, s[0-9]+
**...
*/

CONVERT_INTRINSIC (float32_t, uint32_t, vcvtns_u32_f32)
/*
** convert_vcvtns_u32_f32:
**...
**	fcvtnu	s[0-9]+, s[0-9]+
**...
*/

CONVERT_INTRINSIC (float32_t, uint64_t, vcvtns_u64_f32)
/*
** convert_vcvtns_u64_f32:
**...
**	fcvtnu	d[0-9]+, s[0-9]+
**...
*/

CONVERT_INTRINSIC (float32_t, int32_t, vcvtms_s32_f32)
/*
** convert_vcvtms_s32_f32:
**...
**	fcvtms	s[0-9]+, s[0-9]+
**...
*/

CONVERT_INTRINSIC (float32_t, int64_t, vcvtms_s64_f32)
/*
** convert_vcvtms_s64_f32:
**...
**	fcvtms	d[0-9]+, s[0-9]+
**...
*/

CONVERT_INTRINSIC (float32_t, uint32_t, vcvtms_u32_f32)
/*
** convert_vcvtms_u32_f32:
**...
**	fcvtmu	s[0-9]+, s[0-9]+
**...
*/

CONVERT_INTRINSIC (float32_t, uint64_t, vcvtms_u64_f32)
/*
** convert_vcvtms_u64_f32:
**...
**	fcvtmu	d[0-9]+, s[0-9]+
**...
*/

CONVERT_INTRINSIC (float32_t, int32_t, vcvtps_s32_f32)
/*
** convert_vcvtps_s32_f32:
**...
**	fcvtps	s[0-9]+, s[0-9]+
**...
*/

CONVERT_INTRINSIC (float32_t, int64_t, vcvtps_s64_f32)
/*
** convert_vcvtps_s64_f32:
**...
**	fcvtps	d[0-9]+, s[0-9]+
**...
*/

CONVERT_INTRINSIC (float32_t, uint32_t, vcvtps_u32_f32)
/*
** convert_vcvtps_u32_f32:
**...
**	fcvtpu	s[0-9]+, s[0-9]+
**...
*/

CONVERT_INTRINSIC (float32_t, uint64_t, vcvtps_u64_f32)
/*
** convert_vcvtps_u64_f32:
**...
**	fcvtpu	d[0-9]+, s[0-9]+
**...
*/

CONVERT_INTRINSIC (float32_t, int32_t, vcvtas_s32_f32)
/*
** convert_vcvtas_s32_f32:
**...
**	fcvtas	s[0-9]+, s[0-9]+
**...
*/

CONVERT_INTRINSIC (float32_t, int64_t, vcvtas_s64_f32)
/*
** convert_vcvtas_s64_f32:
**...
**	fcvtas	d[0-9]+, s[0-9]+
**...
*/

CONVERT_INTRINSIC (float32_t, uint32_t, vcvtas_u32_f32)
/*
** convert_vcvtas_u32_f32:
**...
**	fcvtau	s[0-9]+, s[0-9]+
**...
*/

CONVERT_INTRINSIC (float32_t, uint64_t, vcvtas_u64_f32)
/*
** convert_vcvtas_u64_f32:
**...
**	fcvtau	d[0-9]+, s[0-9]+
**...
*/

CONVERT_INTRINSIC (float64_t, int32_t, vcvtd_s32_f64)
/*
** convert_vcvtd_s32_f64:
**...
**	fcvtzs	s[0-9]+, d[0-9]+
**...
*/

CONVERT_INTRINSIC (float64_t, int64_t, vcvtd_s64_f64)
/*
** convert_vcvtd_s64_f64:
**...
**	fcvtzs	d[0-9]+, d[0-9]+
**...
*/

CONVERT_INTRINSIC (float64_t, uint32_t, vcvtd_u32_f64)
/*
** convert_vcvtd_u32_f64:
**...
**	fcvtzu	s[0-9]+, d[0-9]+
**...
*/

CONVERT_INTRINSIC (float64_t, uint64_t, vcvtd_u64_f64)
/*
** convert_vcvtd_u64_f64:
**...
**	fcvtzu	d[0-9]+, d[0-9]+
**...
*/

CONVERT_INTRINSIC (float64_t, int32_t, vcvtnd_s32_f64)
/*
** convert_vcvtnd_s32_f64:
**...
**	fcvtns	s[0-9]+, d[0-9]+
**...
*/

CONVERT_INTRINSIC (float64_t, int64_t, vcvtnd_s64_f64)
/*
** convert_vcvtnd_s64_f64:
**...
**	fcvtns	d[0-9]+, d[0-9]+
**...
*/

CONVERT_INTRINSIC (float64_t, uint32_t, vcvtnd_u32_f64)
/*
** convert_vcvtnd_u32_f64:
**...
**	fcvtnu	s[0-9]+, d[0-9]+
**...
*/

CONVERT_INTRINSIC (float64_t, uint64_t, vcvtnd_u64_f64)
/*
** convert_vcvtnd_u64_f64:
**...
**	fcvtnu	d[0-9]+, d[0-9]+
**...
*/

CONVERT_INTRINSIC (float64_t, int32_t, vcvtmd_s32_f64)
/*
** convert_vcvtmd_s32_f64:
**...
**	fcvtms	s[0-9]+, d[0-9]+
**...
*/

CONVERT_INTRINSIC (float64_t, int64_t, vcvtmd_s64_f64)
/*
** convert_vcvtmd_s64_f64:
**...
**	fcvtms	d[0-9]+, d[0-9]+
**...
*/

CONVERT_INTRINSIC (float64_t, uint32_t, vcvtmd_u32_f64)
/*
** convert_vcvtmd_u32_f64:
**...
**	fcvtmu	s[0-9]+, d[0-9]+
**...
*/

CONVERT_INTRINSIC (float64_t, uint64_t, vcvtmd_u64_f64)
/*
** convert_vcvtmd_u64_f64:
**...
**	fcvtmu	d[0-9]+, d[0-9]+
**...
*/

CONVERT_INTRINSIC (float64_t, int32_t, vcvtpd_s32_f64)
/*
** convert_vcvtpd_s32_f64:
**...
**	fcvtps	s[0-9]+, d[0-9]+
**...
*/

CONVERT_INTRINSIC (float64_t, int64_t, vcvtpd_s64_f64)
/*
** convert_vcvtpd_s64_f64:
**...
**	fcvtps	d[0-9]+, d[0-9]+
**...
*/

CONVERT_INTRINSIC (float64_t, uint32_t, vcvtpd_u32_f64)
/*
** convert_vcvtpd_u32_f64:
**...
**	fcvtpu	s[0-9]+, d[0-9]+
**...
*/

CONVERT_INTRINSIC (float64_t, uint64_t, vcvtpd_u64_f64)
/*
** convert_vcvtpd_u64_f64:
**...
**	fcvtpu	d[0-9]+, d[0-9]+
**...
*/

CONVERT_INTRINSIC (float64_t, int32_t, vcvtad_s32_f64)
/*
** convert_vcvtad_s32_f64:
**...
**	fcvtas	s[0-9]+, d[0-9]+
**...
*/

CONVERT_INTRINSIC (float64_t, int64_t, vcvtad_s64_f64)
/*
** convert_vcvtad_s64_f64:
**...
**	fcvtas	d[0-9]+, d[0-9]+
**...
*/

CONVERT_INTRINSIC (float64_t, uint32_t, vcvtad_u32_f64)
/*
** convert_vcvtad_u32_f64:
**...
**	fcvtau	s[0-9]+, d[0-9]+
**...
*/

CONVERT_INTRINSIC (float64_t, uint64_t, vcvtad_u64_f64)
/*
** convert_vcvtad_u64_f64:
**...
**	fcvtau	d[0-9]+, d[0-9]+
**...
*/

CONVERT_INTRINSIC (float16_t, int16_t, vcvth_s16_f16)
/*
** convert_vcvth_s16_f16:
**...
**	fcvtzs	h[0-9]+, h[0-9]+
**...
*/

CONVERT_INTRINSIC (float16_t, int32_t, vcvth_s32_f16)
/*
** convert_vcvth_s32_f16:
**...
**	fcvtzs	s[0-9]+, h[0-9]+
**...
*/

CONVERT_INTRINSIC (float16_t, int64_t, vcvth_s64_f16)
/*
** convert_vcvth_s64_f16:
**...
**	fcvtzs	d[0-9]+, h[0-9]+
**...
*/

CONVERT_INTRINSIC (float16_t, uint16_t, vcvth_u16_f16)
/*
** convert_vcvth_u16_f16:
**...
**	fcvtzu	h[0-9]+, h[0-9]+
**...
*/

CONVERT_INTRINSIC (float16_t, uint32_t, vcvth_u32_f16)
/*
** convert_vcvth_u32_f16:
**...
**	fcvtzu	s[0-9]+, h[0-9]+
**...
*/

CONVERT_INTRINSIC (float16_t, uint64_t, vcvth_u64_f16)
/*
** convert_vcvth_u64_f16:
**...
**	fcvtzu	d[0-9]+, h[0-9]+
**...
*/

CONVERT_INTRINSIC (float16_t, int16_t, vcvtah_s16_f16)
/*
** convert_vcvtah_s16_f16:
**...
**	fcvtas	h[0-9]+, h[0-9]+
**...
*/

CONVERT_INTRINSIC (float16_t, int32_t, vcvtah_s32_f16)
/*
** convert_vcvtah_s32_f16:
**...
**	fcvtas	s[0-9]+, h[0-9]+
**...
*/

CONVERT_INTRINSIC (float16_t, int64_t, vcvtah_s64_f16)
/*
** convert_vcvtah_s64_f16:
**...
**	fcvtas	d[0-9]+, h[0-9]+
**...
*/

CONVERT_INTRINSIC (float16_t, uint16_t, vcvtah_u16_f16)
/*
** convert_vcvtah_u16_f16:
**...
**	fcvtau	h[0-9]+, h[0-9]+
**...
*/

CONVERT_INTRINSIC (float16_t, uint32_t, vcvtah_u32_f16)
/*
** convert_vcvtah_u32_f16:
**...
**	fcvtau	s[0-9]+, h[0-9]+
**...
*/

CONVERT_INTRINSIC (float16_t, uint64_t, vcvtah_u64_f16)
/*
** convert_vcvtah_u64_f16:
**...
**	fcvtau	d[0-9]+, h[0-9]+
**...
*/

CONVERT_INTRINSIC (float16_t, int16_t, vcvtmh_s16_f16)
/*
** convert_vcvtmh_s16_f16:
**...
**	fcvtms	h[0-9]+, h[0-9]+
**...
*/

CONVERT_INTRINSIC (float16_t, int32_t, vcvtmh_s32_f16)
/*
** convert_vcvtmh_s32_f16:
**...
**	fcvtms	s[0-9]+, h[0-9]+
**...
*/

CONVERT_INTRINSIC (float16_t, int64_t, vcvtmh_s64_f16)
/*
** convert_vcvtmh_s64_f16:
**...
**	fcvtms	d[0-9]+, h[0-9]+
**...
*/

CONVERT_INTRINSIC (float16_t, uint16_t, vcvtmh_u16_f16)
/*
** convert_vcvtmh_u16_f16:
**...
**	fcvtmu	h[0-9]+, h[0-9]+
**...
*/

CONVERT_INTRINSIC (float16_t, uint32_t, vcvtmh_u32_f16)
/*
** convert_vcvtmh_u32_f16:
**...
**	fcvtmu	s[0-9]+, h[0-9]+
**...
*/

CONVERT_INTRINSIC (float16_t, uint64_t, vcvtmh_u64_f16)
/*
** convert_vcvtmh_u64_f16:
**...
**	fcvtmu	d[0-9]+, h[0-9]+
**...
*/

CONVERT_INTRINSIC (float16_t, int16_t, vcvtnh_s16_f16)
/*
** convert_vcvtnh_s16_f16:
**...
**	fcvtns	h[0-9]+, h[0-9]+
**...
*/

CONVERT_INTRINSIC (float16_t, int32_t, vcvtnh_s32_f16)
/*
** convert_vcvtnh_s32_f16:
**...
**	fcvtns	s[0-9]+, h[0-9]+
**...
*/

CONVERT_INTRINSIC (float16_t, int64_t, vcvtnh_s64_f16)
/*
** convert_vcvtnh_s64_f16:
**...
**	fcvtns	d[0-9]+, h[0-9]+
**...
*/

CONVERT_INTRINSIC (float16_t, uint16_t, vcvtnh_u16_f16)
/*
** convert_vcvtnh_u16_f16:
**...
**	fcvtnu	h[0-9]+, h[0-9]+
**...
*/

CONVERT_INTRINSIC (float16_t, uint32_t, vcvtnh_u32_f16)
/*
** convert_vcvtnh_u32_f16:
**...
**	fcvtnu	s[0-9]+, h[0-9]+
**...
*/

CONVERT_INTRINSIC (float16_t, uint64_t, vcvtnh_u64_f16)
/*
** convert_vcvtnh_u64_f16:
**...
**	fcvtnu	d[0-9]+, h[0-9]+
**...
*/

CONVERT_INTRINSIC (float16_t, int16_t, vcvtph_s16_f16)
/*
** convert_vcvtph_s16_f16:
**...
**	fcvtps	h[0-9]+, h[0-9]+
**...
*/

CONVERT_INTRINSIC (float16_t, int32_t, vcvtph_s32_f16)
/*
** convert_vcvtph_s32_f16:
**...
**	fcvtps	s[0-9]+, h[0-9]+
**...
*/

CONVERT_INTRINSIC (float16_t, int64_t, vcvtph_s64_f16)
/*
** convert_vcvtph_s64_f16:
**...
**	fcvtps	d[0-9]+, h[0-9]+
**...
*/

CONVERT_INTRINSIC (float16_t, uint16_t, vcvtph_u16_f16)
/*
** convert_vcvtph_u16_f16:
**...
**	fcvtpu	h[0-9]+, h[0-9]+
**...
*/

CONVERT_INTRINSIC (float16_t, uint32_t, vcvtph_u32_f16)
/*
** convert_vcvtph_u32_f16:
**...
**	fcvtpu	s[0-9]+, h[0-9]+
**...
*/

CONVERT_INTRINSIC (float16_t, uint64_t, vcvtph_u64_f16)
/*
** convert_vcvtph_u64_f16:
**...
**	fcvtpu	d[0-9]+, h[0-9]+
**...
*/


CONVERT_INTRINSIC (int32_t, float32_t, vcvts_f32_s32)
/*
** convert_vcvts_f32_s32:
**...
**	scvtf	s[0-9]+, s[0-9]+
**...
*/
CONVERT_INTRINSIC (uint32_t, float32_t, vcvts_f32_u32)
/*
** convert_vcvts_f32_u32:
**...
**	ucvtf	s[0-9]+, s[0-9]+
**...
*/
CONVERT_INTRINSIC (int64_t, float64_t, vcvtd_f64_s64)
/*
** convert_vcvtd_f64_s64:
**...
**	scvtf	d[0-9]+, d[0-9]+
**...
*/
CONVERT_INTRINSIC (uint64_t, float64_t, vcvtd_f64_u64)
/*
** convert_vcvtd_f64_u64:
**...
**	ucvtf	d[0-9]+, d[0-9]+
**...
*/
CONVERT_INTRINSIC (int16_t, float16_t, vcvth_f16_s16)
/*
** convert_vcvth_f16_s16:
**...
**	scvtf	h[0-9]+, h[0-9]+
**...
*/
CONVERT_INTRINSIC (int32_t, float16_t, vcvth_f16_s32)
/*
** convert_vcvth_f16_s32:
**...
**	scvtf	h[0-9]+, s[0-9]+
**...
*/
CONVERT_INTRINSIC (int64_t, float16_t, vcvth_f16_s64)
/*
** convert_vcvth_f16_s64:
**...
**	scvtf	h[0-9]+, d[0-9]+
**...
*/
CONVERT_INTRINSIC (uint16_t, float16_t, vcvth_f16_u16)
/*
** convert_vcvth_f16_u16:
**...
**	ucvtf	h[0-9]+, h[0-9]+
**...
*/
CONVERT_INTRINSIC (uint32_t, float16_t, vcvth_f16_u32)
/*
** convert_vcvth_f16_u32:
**...
**	ucvtf	h[0-9]+, s[0-9]+
**...
*/
CONVERT_INTRINSIC (uint64_t, float16_t, vcvth_f16_u64)
/*
** convert_vcvth_f16_u64:
**...
**	ucvtf	h[0-9]+, d[0-9]+
**...
*/
