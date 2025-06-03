/* { dg-do compile } */
/* { dg-options "-O -msve-vector-bits=512 -g" } */
/* { dg-final { check-function-bodies "**" "" "" { target lp64 } } } */

#include <stdint.h>

typedef int8_t svint8_t __attribute__ ((vector_size (64)));
typedef uint8_t svuint8_t __attribute__ ((vector_size (64)));
typedef __mfp8 svmfloat8_t __attribute__ ((vector_size (64)));

typedef int16_t svint16_t __attribute__ ((vector_size (64)));
typedef uint16_t svuint16_t __attribute__ ((vector_size (64)));
typedef __fp16 svfloat16_t __attribute__ ((vector_size (64)));
typedef __bf16 svbfloat16_t __attribute__ ((vector_size (64)));

typedef int32_t svint32_t __attribute__ ((vector_size (64)));
typedef uint32_t svuint32_t __attribute__ ((vector_size (64)));
typedef float svfloat32_t __attribute__ ((vector_size (64)));

typedef int64_t svint64_t __attribute__ ((vector_size (64)));
typedef uint64_t svuint64_t __attribute__ ((vector_size (64)));
typedef double svfloat64_t __attribute__ ((vector_size (64)));

#define CALLEE(SUFFIX, TYPE)			\
  TYPE __attribute__((noipa))			\
  callee_##SUFFIX (TYPE *ptr)			\
  {						\
    return *ptr;				\
  }

/*
** callee_s8:
**	ptrue	(p[0-7])\.b, vl64
**	ld1b	(z[0-9]+)\.b, \1/z, \[x0\]
**	st1b	\2\.b, \1, \[x8\]
**	ret
*/
CALLEE (s8, svint8_t)

/*
** callee_u8:
**	ptrue	(p[0-7])\.b, vl64
**	ld1b	(z[0-9]+)\.b, \1/z, \[x0\]
**	st1b	\2\.b, \1, \[x8\]
**	ret
*/
CALLEE (u8, svuint8_t)

/*
** callee_mf8:
**	ptrue	(p[0-7])\.b, vl64
**	ld1b	(z[0-9]+)\.b, \1/z, \[x0\]
**	st1b	\2\.b, \1, \[x8\]
**	ret
*/
CALLEE (mf8, svmfloat8_t)

/*
** callee_s16:
**	ptrue	(p[0-7])\.b, vl64
**	ld1h	(z[0-9]+)\.h, \1/z, \[x0\]
**	st1h	\2\.h, \1, \[x8\]
**	ret
*/
CALLEE (s16, svint16_t)

/*
** callee_u16:
**	ptrue	(p[0-7])\.b, vl64
**	ld1h	(z[0-9]+)\.h, \1/z, \[x0\]
**	st1h	\2\.h, \1, \[x8\]
**	ret
*/
CALLEE (u16, svuint16_t)

/*
** callee_f16:
**	ptrue	(p[0-7])\.b, vl64
**	ld1h	(z[0-9]+)\.h, \1/z, \[x0\]
**	st1h	\2\.h, \1, \[x8\]
**	ret
*/
CALLEE (f16, svfloat16_t)

/*
** callee_bf16:
**	ptrue	(p[0-7])\.b, vl64
**	ld1h	(z[0-9]+)\.h, \1/z, \[x0\]
**	st1h	\2\.h, \1, \[x8\]
**	ret
*/
CALLEE (bf16, svbfloat16_t)

/*
** callee_s32:
**	ptrue	(p[0-7])\.b, vl64
**	ld1w	(z[0-9]+)\.s, \1/z, \[x0\]
**	st1w	\2\.s, \1, \[x8\]
**	ret
*/
CALLEE (s32, svint32_t)

/*
** callee_u32:
**	ptrue	(p[0-7])\.b, vl64
**	ld1w	(z[0-9]+)\.s, \1/z, \[x0\]
**	st1w	\2\.s, \1, \[x8\]
**	ret
*/
CALLEE (u32, svuint32_t)

/*
** callee_f32:
**	ptrue	(p[0-7])\.b, vl64
**	ld1w	(z[0-9]+)\.s, \1/z, \[x0\]
**	st1w	\2\.s, \1, \[x8\]
**	ret
*/
CALLEE (f32, svfloat32_t)

/*
** callee_s64:
**	ptrue	(p[0-7])\.b, vl64
**	ld1d	(z[0-9]+)\.d, \1/z, \[x0\]
**	st1d	\2\.d, \1, \[x8\]
**	ret
*/
CALLEE (s64, svint64_t)

/*
** callee_u64:
**	ptrue	(p[0-7])\.b, vl64
**	ld1d	(z[0-9]+)\.d, \1/z, \[x0\]
**	st1d	\2\.d, \1, \[x8\]
**	ret
*/
CALLEE (u64, svuint64_t)

/*
** callee_f64:
**	ptrue	(p[0-7])\.b, vl64
**	ld1d	(z[0-9]+)\.d, \1/z, \[x0\]
**	st1d	\2\.d, \1, \[x8\]
**	ret
*/
CALLEE (f64, svfloat64_t)

#define CALLER(SUFFIX, TYPE)			\
  void __attribute__((noipa))			\
  caller_##SUFFIX (TYPE *ptr1, TYPE *ptr2)	\
  {						\
    *ptr2 = callee_##SUFFIX (ptr1);		\
  }

/*
** caller_s8:
**	...
**	bl	callee_s8
**	...
**	ld1b	(z[0-9]+\.b), (p[0-7])/z, \[[^]]*\]
**	st1b	\1, \2, \[[^]]*\]
**	...
**	ret
*/
CALLER (s8, svint8_t)

/*
** caller_u8:
**	...
**	bl	callee_u8
**	...
**	ld1b	(z[0-9]+\.b), (p[0-7])/z, \[[^]]*\]
**	st1b	\1, \2, \[[^]]*\]
**	...
**	ret
*/
CALLER (u8, svuint8_t)

/*
** caller_mf8:
**	...
**	bl	callee_mf8
**	...
**	ld1b	(z[0-9]+\.b), (p[0-7])/z, \[[^]]*\]
**	st1b	\1, \2, \[[^]]*\]
**	...
**	ret
*/
CALLER (mf8, svmfloat8_t)

/*
** caller_s16:
**	...
**	bl	callee_s16
**	...
**	ld1h	(z[0-9]+\.h), (p[0-7])/z, \[[^]]*\]
**	st1h	\1, \2, \[[^]]*\]
**	...
**	ret
*/
CALLER (s16, svint16_t)

/*
** caller_u16:
**	...
**	bl	callee_u16
**	...
**	ld1h	(z[0-9]+\.h), (p[0-7])/z, \[[^]]*\]
**	st1h	\1, \2, \[[^]]*\]
**	...
**	ret
*/
CALLER (u16, svuint16_t)

/*
** caller_f16:
**	...
**	bl	callee_f16
**	...
**	ld1h	(z[0-9]+\.h), (p[0-7])/z, \[[^]]*\]
**	st1h	\1, \2, \[[^]]*\]
**	...
**	ret
*/
CALLER (f16, svfloat16_t)

/*
** caller_bf16:
**	...
**	bl	callee_bf16
**	...
**	ld1h	(z[0-9]+\.h), (p[0-7])/z, \[[^]]*\]
**	st1h	\1, \2, \[[^]]*\]
**	...
**	ret
*/
CALLER (bf16, svbfloat16_t)

/*
** caller_s32:
**	...
**	bl	callee_s32
**	...
**	ld1w	(z[0-9]+\.s), (p[0-7])/z, \[[^]]*\]
**	st1w	\1, \2, \[[^]]*\]
**	...
**	ret
*/
CALLER (s32, svint32_t)

/*
** caller_u32:
**	...
**	bl	callee_u32
**	...
**	ld1w	(z[0-9]+\.s), (p[0-7])/z, \[[^]]*\]
**	st1w	\1, \2, \[[^]]*\]
**	...
**	ret
*/
CALLER (u32, svuint32_t)

/*
** caller_f32:
**	...
**	bl	callee_f32
**	...
**	ld1w	(z[0-9]+\.s), (p[0-7])/z, \[[^]]*\]
**	st1w	\1, \2, \[[^]]*\]
**	...
**	ret
*/
CALLER (f32, svfloat32_t)

/*
** caller_s64:
**	...
**	bl	callee_s64
**	...
**	ld1d	(z[0-9]+\.d), (p[0-7])/z, \[[^]]*\]
**	st1d	\1, \2, \[[^]]*\]
**	...
**	ret
*/
CALLER (s64, svint64_t)

/*
** caller_u64:
**	...
**	bl	callee_u64
**	...
**	ld1d	(z[0-9]+\.d), (p[0-7])/z, \[[^]]*\]
**	st1d	\1, \2, \[[^]]*\]
**	...
**	ret
*/
CALLER (u64, svuint64_t)

/*
** caller_f64:
**	...
**	bl	callee_f64
**	...
**	ld1d	(z[0-9]+\.d), (p[0-7])/z, \[[^]]*\]
**	st1d	\1, \2, \[[^]]*\]
**	...
**	ret
*/
CALLER (f64, svfloat64_t)
