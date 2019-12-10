/* { dg-do compile } */
/* { dg-options "-O -msve-vector-bits=512 -g" } */
/* { dg-final { check-function-bodies "**" "" } } */

#include <arm_sve.h>

#define CALLEE(SUFFIX, TYPE)			\
  TYPE __attribute__((noipa))			\
  callee_##SUFFIX (TYPE *ptr)			\
  {						\
    return *ptr;				\
  }

/*
** callee_s8:
**	ptrue	(p[0-7])\.b, vl64
**	ld1b	z0\.b, \1/z, \[x0\]
**	ret
*/
CALLEE (s8, svint8_t)

/*
** callee_u8:
**	ptrue	(p[0-7])\.b, vl64
**	ld1b	z0\.b, \1/z, \[x0\]
**	ret
*/
CALLEE (u8, svuint8_t)

/*
** callee_s16:
**	ptrue	(p[0-7])\.b, vl64
**	ld1h	z0\.h, \1/z, \[x0\]
**	ret
*/
CALLEE (s16, svint16_t)

/*
** callee_u16:
**	ptrue	(p[0-7])\.b, vl64
**	ld1h	z0\.h, \1/z, \[x0\]
**	ret
*/
CALLEE (u16, svuint16_t)

/*
** callee_f16:
**	ptrue	(p[0-7])\.b, vl64
**	ld1h	z0\.h, \1/z, \[x0\]
**	ret
*/
CALLEE (f16, svfloat16_t)

/*
** callee_s32:
**	ptrue	(p[0-7])\.b, vl64
**	ld1w	z0\.s, \1/z, \[x0\]
**	ret
*/
CALLEE (s32, svint32_t)

/*
** callee_u32:
**	ptrue	(p[0-7])\.b, vl64
**	ld1w	z0\.s, \1/z, \[x0\]
**	ret
*/
CALLEE (u32, svuint32_t)

/*
** callee_f32:
**	ptrue	(p[0-7])\.b, vl64
**	ld1w	z0\.s, \1/z, \[x0\]
**	ret
*/
CALLEE (f32, svfloat32_t)

/*
** callee_s64:
**	ptrue	(p[0-7])\.b, vl64
**	ld1d	z0\.d, \1/z, \[x0\]
**	ret
*/
CALLEE (s64, svint64_t)

/*
** callee_u64:
**	ptrue	(p[0-7])\.b, vl64
**	ld1d	z0\.d, \1/z, \[x0\]
**	ret
*/
CALLEE (u64, svuint64_t)

/*
** callee_f64:
**	ptrue	(p[0-7])\.b, vl64
**	ld1d	z0\.d, \1/z, \[x0\]
**	ret
*/
CALLEE (f64, svfloat64_t)

#define CALLER(SUFFIX, TYPE)					\
  typeof (svaddv (svptrue_b8 (), *(TYPE *) 0))			\
  __attribute__((noipa))					\
  caller_##SUFFIX (TYPE *ptr1)					\
  {								\
    return svaddv (svptrue_b8 (), callee_##SUFFIX (ptr1));	\
  }

/*
** caller_s8:
**	...
**	bl	callee_s8
**	ptrue	(p[0-7])\.b, vl64
**	saddv	(d[0-9]+), \1, z0\.b
**	fmov	x0, \2
**	ldp	x29, x30, \[sp\], 16
**	ret
*/
CALLER (s8, svint8_t)

/*
** caller_u8:
**	...
**	bl	callee_u8
**	ptrue	(p[0-7])\.b, vl64
**	uaddv	(d[0-9]+), \1, z0\.b
**	fmov	x0, \2
**	ldp	x29, x30, \[sp\], 16
**	ret
*/
CALLER (u8, svuint8_t)

/*
** caller_s16:
**	...
**	bl	callee_s16
**	ptrue	(p[0-7])\.b, vl64
**	saddv	(d[0-9]+), \1, z0\.h
**	fmov	x0, \2
**	ldp	x29, x30, \[sp\], 16
**	ret
*/
CALLER (s16, svint16_t)

/*
** caller_u16:
**	...
**	bl	callee_u16
**	ptrue	(p[0-7])\.b, vl64
**	uaddv	(d[0-9]+), \1, z0\.h
**	fmov	x0, \2
**	ldp	x29, x30, \[sp\], 16
**	ret
*/
CALLER (u16, svuint16_t)

/*
** caller_f16:
**	...
**	bl	callee_f16
**	ptrue	(p[0-7])\.b, vl64
**	faddv	h0, \1, z0\.h
**	ldp	x29, x30, \[sp\], 16
**	ret
*/
CALLER (f16, svfloat16_t)

/*
** caller_s32:
**	...
**	bl	callee_s32
**	ptrue	(p[0-7])\.b, vl64
**	saddv	(d[0-9]+), \1, z0\.s
**	fmov	x0, \2
**	ldp	x29, x30, \[sp\], 16
**	ret
*/
CALLER (s32, svint32_t)

/*
** caller_u32:
**	...
**	bl	callee_u32
**	ptrue	(p[0-7])\.b, vl64
**	uaddv	(d[0-9]+), \1, z0\.s
**	fmov	x0, \2
**	ldp	x29, x30, \[sp\], 16
**	ret
*/
CALLER (u32, svuint32_t)

/*
** caller_f32:
**	...
**	bl	callee_f32
**	ptrue	(p[0-7])\.b, vl64
**	faddv	s0, \1, z0\.s
**	ldp	x29, x30, \[sp\], 16
**	ret
*/
CALLER (f32, svfloat32_t)

/*
** caller_s64:
**	...
**	bl	callee_s64
**	ptrue	(p[0-7])\.b, vl64
**	uaddv	(d[0-9]+), \1, z0\.d
**	fmov	x0, \2
**	ldp	x29, x30, \[sp\], 16
**	ret
*/
CALLER (s64, svint64_t)

/*
** caller_u64:
**	...
**	bl	callee_u64
**	ptrue	(p[0-7])\.b, vl64
**	uaddv	(d[0-9]+), \1, z0\.d
**	fmov	x0, \2
**	ldp	x29, x30, \[sp\], 16
**	ret
*/
CALLER (u64, svuint64_t)

/*
** caller_f64:
**	...
**	bl	callee_f64
**	ptrue	(p[0-7])\.b, vl64
**	faddv	d0, \1, z0\.d
**	ldp	x29, x30, \[sp\], 16
**	ret
*/
CALLER (f64, svfloat64_t)
