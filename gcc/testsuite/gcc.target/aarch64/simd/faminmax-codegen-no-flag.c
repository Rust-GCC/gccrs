/* { dg-do compile } */
/* { dg-additional-options "-O3 -ffast-math -march=armv9-a" } */
/* { dg-final { check-function-bodies "**" "" } } */

#include "arm_neon.h"

#pragma GCC target "+nosve"

/*
** test_vamax_f16:
**	fabs	v1.4h, v1.4h
**	fabs	v0.4h, v0.4h
**	fmaxnm	v0.4h, v0.4h, v1.4h
**	ret
*/
float16x4_t
test_vamax_f16 (float16x4_t a, float16x4_t b)
{
  int i;
  float16x4_t c;

  for (i = 0; i < 4; ++i) {
    a[i] = __builtin_fabsf16 (a[i]);
    b[i] = __builtin_fabsf16 (b[i]);
    c[i] = __builtin_fmaxf16 (a[i], b[i]);
  }
  return c;
}

/*
** test_vamaxq_f16:
**	fabs	v1.8h, v1.8h
**	fabs	v0.8h, v0.8h
**	fmaxnm	v0.8h, v0.8h, v1.8h
**	ret
*/
float16x8_t
test_vamaxq_f16 (float16x8_t a, float16x8_t b)
{
  int i;
  float16x8_t c;

  for (i = 0; i < 8; ++i) {
    a[i] = __builtin_fabsf16 (a[i]);
    b[i] = __builtin_fabsf16 (b[i]);
    c[i] = __builtin_fmaxf16 (a[i], b[i]);
  }
  return c;
}

/*
** test_vamax_f32:
**	fabs	v1.2s, v1.2s
**	fabs	v0.2s, v0.2s
**	fmaxnm	v0.2s, v0.2s, v1.2s
**	ret
*/
float32x2_t
test_vamax_f32 (float32x2_t a, float32x2_t b)
{
  int i;
  float32x2_t c;

  for (i = 0; i < 2; ++i) {
    a[i] = __builtin_fabsf32 (a[i]);
    b[i] = __builtin_fabsf32 (b[i]);
    c[i] = __builtin_fmaxf32 (a[i], b[i]);
  }
  return c;
}

/*
** test_vamaxq_f32:
**	fabs	v1.4s, v1.4s
**	fabs	v0.4s, v0.4s
**	fmaxnm	v0.4s, v0.4s, v1.4s
**	ret
*/
float32x4_t
test_vamaxq_f32 (float32x4_t a, float32x4_t b)
{
  int i;
  float32x4_t c;

  for (i = 0; i < 4; ++i) {
    a[i] = __builtin_fabsf32 (a[i]);
    b[i] = __builtin_fabsf32 (b[i]);
    c[i] = __builtin_fmaxf32 (a[i], b[i]);
  }
  return c;
}

/*
** test_vamaxq_f64:
**	fabs	v1.2d, v1.2d
**	fabs	v0.2d, v0.2d
**	fmaxnm	v0.2d, v0.2d, v1.2d
**	ret
*/
float64x2_t
test_vamaxq_f64 (float64x2_t a, float64x2_t b)
{
  int i;
  float64x2_t c;

  for (i = 0; i < 2; ++i) {
    a[i] = __builtin_fabsf64 (a[i]);
    b[i] = __builtin_fabsf64 (b[i]);
    c[i] = __builtin_fmaxf64 (a[i], b[i]);
  }
  return c;
}

/*
** test_vamin_f16:
**	fabs	v1.4h, v1.4h
**	fabs	v0.4h, v0.4h
**	fminnm	v0.4h, v0.4h, v1.4h
**	ret
*/
float16x4_t
test_vamin_f16 (float16x4_t a, float16x4_t b)
{
  int i;
  float16x4_t c;

  for (i = 0; i < 4; ++i) {
    a[i] = __builtin_fabsf16 (a[i]);
    b[i] = __builtin_fabsf16 (b[i]);
    c[i] = __builtin_fminf16 (a[i], b[i]);
  }
  return c;
}

/*
** test_vaminq_f16:
**	fabs	v1.8h, v1.8h
**	fabs	v0.8h, v0.8h
**	fminnm	v0.8h, v0.8h, v1.8h
**	ret
*/
float16x8_t
test_vaminq_f16 (float16x8_t a, float16x8_t b)
{
  int i;
  float16x8_t c;

  for (i = 0; i < 8; ++i) {
    a[i] = __builtin_fabsf16 (a[i]);
    b[i] = __builtin_fabsf16 (b[i]);
    c[i] = __builtin_fminf16 (a[i], b[i]);
  }
  return c;
}

/*
** test_vamin_f32:
**	fabs	v1.2s, v1.2s
**	fabs	v0.2s, v0.2s
**	fminnm	v0.2s, v0.2s, v1.2s
**	ret
*/
float32x2_t
test_vamin_f32 (float32x2_t a, float32x2_t b)
{
  int i;
  float32x2_t c;

  for (i = 0; i < 2; ++i) {
    a[i] = __builtin_fabsf32 (a[i]);
    b[i] = __builtin_fabsf32 (b[i]);
    c[i] = __builtin_fminf32 (a[i], b[i]);
  }
  return c;
}

/*
** test_vaminq_f32:
**	fabs	v1.4s, v1.4s
**	fabs	v0.4s, v0.4s
**	fminnm	v0.4s, v0.4s, v1.4s
**	ret
*/
float32x4_t
test_vaminq_f32 (float32x4_t a, float32x4_t b)
{
  int i;
  float32x4_t c;

  for (i = 0; i < 4; ++i) {
    a[i] = __builtin_fabsf32 (a[i]);
    b[i] = __builtin_fabsf32 (b[i]);
    c[i] = __builtin_fminf32 (a[i], b[i]);
  }
  return c;
}

/*
** test_vaminq_f64:
**	fabs	v1.2d, v1.2d
**	fabs	v0.2d, v0.2d
**	fminnm	v0.2d, v0.2d, v1.2d
**	ret
*/
float64x2_t
test_vaminq_f64 (float64x2_t a, float64x2_t b)
{
  int i;
  float64x2_t c;

  for (i = 0; i < 2; ++i) {
    a[i] = __builtin_fabsf64 (a[i]);
    b[i] = __builtin_fabsf64 (b[i]);
    c[i] = __builtin_fminf64 (a[i], b[i]);
  }
  return c;
}
