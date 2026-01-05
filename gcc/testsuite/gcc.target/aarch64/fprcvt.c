/* { dg-do compile } */
/* { dg-options "-save-temps -fno-inline -O2 -march=armv9-a+fprcvt" } */
/* { dg-final { check-function-bodies "**" "" "" } } */

#include <stdint.h>
#include <arm_fp16.h>
#include <arm_acle.h>
#include <arm_neon.h>

#define FORCE_FP_REG(a) asm volatile("fmov %d0, %d1" : "=w"(a) : "w"(a) :)
#define CONVERT(T_FROM, T_TO)                                                  \
  void convert_##T_FROM##_to_##T_TO (T_FROM *from, T_TO *to)                      \
  {                                                                            \
    T_FROM from_val = *from;                                                   \
    FORCE_FP_REG (from_val);                                                       \
    T_TO to_val = (T_TO) from_val;                                             \
    FORCE_FP_REG (to_val);                                                         \
    *to = to_val;                                                              \
  }

/*
** convert_float16_t_to_int32_t:
**...
**	fcvtzs	s[0-9]+, h[0-9]+
**...
*/
CONVERT (float16_t, int32_t)

/*
** convert_float64_t_to_int32_t:
**...
**	fcvtzs	s[0-9]+, d[0-9]+
**...
*/
CONVERT (float64_t, int32_t)

/*
** convert_float16_t_to_int64_t:
**...
**	fcvtzs	d[0-9]+, h[0-9]+
**...
*/
CONVERT (float16_t, int64_t)

/*
** convert_float32_t_to_int64_t:
**...
**	fcvtzs	d[0-9]+, s[0-9]+
**...
*/
CONVERT (float32_t, int64_t)

/*
** convert_float16_t_to_uint32_t:
**...
**	fcvtzu	s[0-9]+, h[0-9]+
**...
*/
CONVERT (float16_t, uint32_t)

/*
** convert_float64_t_to_uint32_t:
**...
**	fcvtzu	s[0-9]+, d[0-9]+
**...
*/
CONVERT (float64_t, uint32_t)

/*
** convert_float16_t_to_uint64_t:
**...
**	fcvtzu	d[0-9]+, h[0-9]+
**...
*/
CONVERT (float16_t, uint64_t)

/*
** convert_float32_t_to_uint64_t:
**...
**	fcvtzu	d[0-9]+, s[0-9]+
**...
*/
CONVERT (float32_t, uint64_t)

/*
** convert_int32_t_to_float16_t:
**...
**	scvtf	h[0-9]+, s[0-9]+
**...
*/
CONVERT (int32_t, float16_t)

/*
** convert_int32_t_to_float64_t:
**...
**	scvtf	d[0-9]+, s[0-9]+
**...
*/
CONVERT (int32_t, float64_t)

/*
** convert_int64_t_to_float16_t:
**...
**	scvtf	h[0-9]+, d[0-9]+
**...
*/
CONVERT (int64_t, float16_t)

/*
** convert_int64_t_to_float32_t:
**...
**	scvtf	s[0-9]+, d[0-9]+
**...
*/
CONVERT (int64_t, float32_t)

/*
** convert_uint32_t_to_float16_t:
**...
**	ucvtf	h[0-9]+, s[0-9]+
**...
*/
CONVERT (uint32_t, float16_t)

/*
** convert_uint32_t_to_float64_t:
**...
**	ucvtf	d[0-9]+, s[0-9]+
**...
*/
CONVERT (uint32_t, float64_t)

/*
** convert_uint64_t_to_float16_t:
**...
**	ucvtf	h[0-9]+, d[0-9]+
**...
*/
CONVERT (uint64_t, float16_t)

/*
** convert_uint64_t_to_float32_t:
**...
**	ucvtf	s[0-9]+, d[0-9]+
**...
*/
CONVERT (uint64_t, float32_t)
