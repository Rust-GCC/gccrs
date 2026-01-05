/* { dg-do compile } */
/* { dg-final { check-function-bodies "**" "" "" } } */

#include <arm_acle.h>
#include <arm_neon.h>

#define CONVERT_INTRINSIC(T_FROM, T_TO, INTRINSIC)                             \
  void convert_##INTRINSIC (T_FROM *from, T_TO *to)                            \
  {                                                                            \
    T_FROM from_val = *from;                                                   \
    T_TO to_val = INTRINSIC (from_val);                                        \
    *to = to_val;                                                              \
  }

CONVERT_INTRINSIC (float32_t, int64_t, vcvts_s64_f32)
/*
** convert_vcvts_s64_f32:
**...
**	fcvtzs	x[0-9]+, s[0-9]+
**...
*/

CONVERT_INTRINSIC (float32_t, uint64_t, vcvts_u64_f32)
/*
** convert_vcvts_u64_f32:
**...
**	fcvtzu	x[0-9]+, s[0-9]+
**...
*/

CONVERT_INTRINSIC (float32_t, int64_t, vcvtns_s64_f32)
/*
** convert_vcvtns_s64_f32:
**...
**	fcvtns	x[0-9]+, s[0-9]+
**...
*/

CONVERT_INTRINSIC (float32_t, uint64_t, vcvtns_u64_f32)
/*
** convert_vcvtns_u64_f32:
**...
**	fcvtnu	x[0-9]+, s[0-9]+
**...
*/

CONVERT_INTRINSIC (float32_t, int64_t, vcvtms_s64_f32)
/*
** convert_vcvtms_s64_f32:
**...
**	fcvtms	x[0-9]+, s[0-9]+
**...
*/

CONVERT_INTRINSIC (float32_t, uint64_t, vcvtms_u64_f32)
/*
** convert_vcvtms_u64_f32:
**...
**	fcvtmu	x[0-9]+, s[0-9]+
**...
*/

CONVERT_INTRINSIC (float32_t, int64_t, vcvtps_s64_f32)
/*
** convert_vcvtps_s64_f32:
**...
**	fcvtps	x[0-9]+, s[0-9]+
**...
*/

CONVERT_INTRINSIC (float32_t, uint64_t, vcvtps_u64_f32)
/*
** convert_vcvtps_u64_f32:
**...
**	fcvtpu	x[0-9]+, s[0-9]+
**...
*/

CONVERT_INTRINSIC (float32_t, int64_t, vcvtas_s64_f32)
/*
** convert_vcvtas_s64_f32:
**...
**	fcvtas	x[0-9]+, s[0-9]+
**...
*/

CONVERT_INTRINSIC (float32_t, uint64_t, vcvtas_u64_f32)
/*
** convert_vcvtas_u64_f32:
**...
**	fcvtau	x[0-9]+, s[0-9]+
**...
*/

CONVERT_INTRINSIC (float64_t, int32_t, vcvtd_s32_f64)
/*
** convert_vcvtd_s32_f64:
**...
**	fcvtzs	w[0-9]+, d[0-9]+
**...
*/

CONVERT_INTRINSIC (float64_t, uint32_t, vcvtd_u32_f64)
/*
** convert_vcvtd_u32_f64:
**...
**	fcvtzu	w[0-9]+, d[0-9]+
**...
*/

CONVERT_INTRINSIC (float64_t, int32_t, vcvtnd_s32_f64)
/*
** convert_vcvtnd_s32_f64:
**...
**	fcvtns	w[0-9]+, d[0-9]+
**...
*/

CONVERT_INTRINSIC (float64_t, uint32_t, vcvtnd_u32_f64)
/*
** convert_vcvtnd_u32_f64:
**...
**	fcvtnu	w[0-9]+, d[0-9]+
**...
*/

CONVERT_INTRINSIC (float64_t, int32_t, vcvtmd_s32_f64)
/*
** convert_vcvtmd_s32_f64:
**...
**	fcvtms	w[0-9]+, d[0-9]+
**...
*/

CONVERT_INTRINSIC (float64_t, uint32_t, vcvtmd_u32_f64)
/*
** convert_vcvtmd_u32_f64:
**...
**	fcvtmu	w[0-9]+, d[0-9]+
**...
*/

CONVERT_INTRINSIC (float64_t, int32_t, vcvtpd_s32_f64)
/*
** convert_vcvtpd_s32_f64:
**...
**	fcvtps	w[0-9]+, d[0-9]+
**...
*/

CONVERT_INTRINSIC (float64_t, uint32_t, vcvtpd_u32_f64)
/*
** convert_vcvtpd_u32_f64:
**...
**	fcvtpu	w[0-9]+, d[0-9]+
**...
*/

CONVERT_INTRINSIC (float64_t, int32_t, vcvtad_s32_f64)
/*
** convert_vcvtad_s32_f64:
**...
**	fcvtas	w[0-9]+, d[0-9]+
**...
*/

