/* { dg-do compile } */
/* { dg-options "-O2 -march=armv8.2-a+fp16" } */
/* { dg-final { check-function-bodies "**" "" "" } } */

#include <arm_neon.h>

/* Check vector initialization with a repeating sequence of elements.  */

#ifndef TESTCASE
#define TESTCASE(TYPE, ETYPE, T, SZ, NUM, MULT, ...)\
  TYPE##SZ##MULT##_t test_##TYPE##SZ##_##NUM (ETYPE x0, ETYPE x1, ETYPE x2, ETYPE x3,\
					   ETYPE x4, ETYPE x5, ETYPE x6, ETYPE x7)\
  {\
    return (TYPE##SZ##MULT##_t) {__VA_ARGS__};\
  }
#endif

#define TEST_8(TYPE, ETYPE, T)\
    TESTCASE (TYPE, ETYPE, T, 8, 1, x16, x0, x0, x0, x0, x0, x0, x0, x0,\
			       x0, x0, x0, x0, x0, x0, x0, x0)\
    TESTCASE (TYPE, ETYPE, T, 8, 2, x16, x0, x1, x0, x1, x0, x1, x0, x1,\
			       x0, x1, x0, x1, x0, x1, x0, x1)\
    TESTCASE (TYPE, ETYPE, T, 8, 3, x16, x0, x1, x2, x3, x0, x1, x2, x3,\
			       x0, x1, x2, x3, x0, x1, x2, x3)\
    TESTCASE (TYPE, ETYPE, T, 8, 4, x16, x0, x1, x2, x3, x4, x5, x6, x7,\
			       x0, x1, x2, x3, x4, x5, x6, x7)\
    TESTCASE (TYPE, ETYPE, T, 8, 5, x16, x0, 0, x0, 0, x0, 0, x0, 0,\
			       x0, 0, x0, 0, x0, 0, x0, 0)\
    TESTCASE (TYPE, ETYPE, T, 8, 6, x16, 0, x0, 0, x0, 0, x0, 0, x0,\
			       0, x0, 0, x0, 0, x0, 0, x0)\
    TESTCASE (TYPE, ETYPE, T, 8, 7, x16, x0, x1, 0, 1, x0, x1, 0, 1,\
			       x0, x1, 0, 1, x0, x1, 0, 1)\
    TESTCASE (TYPE, ETYPE, T, 8, 8, x16, 0, 1, x0, x1, 0, 1, x0, x1,\
			   0, 1, x0, x1, 0, 1, x0, x1)\
    TESTCASE (TYPE, ETYPE, T, 8, 9, x16, x0, 0, x1, 1, x0, 0, x1, 1,\
			       x0, 0, x1, 1, x0, 0, x1, 1)\
    TESTCASE (TYPE, ETYPE, T, 8, 10, x16, x0, 0, x1, 1, x2, 2, x3, 3,\
			       x0, 0, x1, 1, x2, 2, x3, 3)\
    TESTCASE (TYPE, ETYPE, T, 8, 11, x16, 0, x0, 1, x1, 2, x2, 3, x3,\
			       0, x0, 1, x1, 2, x2, 3, x3)\
    TESTCASE (TYPE, ETYPE, T, 8, 12, x16, x0, x1, 0, 1, x2, x3, 2, 3,\
			       x0, x1, 0, 1, x2, x3, 2, 3)\
    TESTCASE (TYPE, ETYPE, T, 8, 13, x16, 0, 1, x0, x1, 2, 3, x2, x3,\
			       0, 1, x0, x1, 2, 3, x2, x3) \
    TESTCASE (TYPE, ETYPE, T, 8, 14, x16, x0, x1, x1, x2, x0, x1, x1, x2,\
			       x0, x1, x1, x2, x0, x1, x1, x2) \
    TESTCASE (TYPE, ETYPE, T, 8, 15, x16, x0, x1, x2, x1, x0, x1, x2, x1,\
			       x0, x1, x2, x1, x0, x1, x2, x1)

#define TEST_16(TYPE, ETYPE, T)\
    TESTCASE (TYPE, ETYPE, T, 16, 1, x8, x0, x0, x0, x0, x0, x0, x0, x0)\
    TESTCASE (TYPE, ETYPE, T, 16, 2, x8, x0, x1, x0, x1, x0, x1, x0, x1)\
    TESTCASE (TYPE, ETYPE, T, 16, 3, x8, x0, x1, x2, x3, x0, x1, x2, x3)\
    TESTCASE (TYPE, ETYPE, T, 16, 4, x8, x0, 0, x0, 0, x0, 0, x0, 0)\
    TESTCASE (TYPE, ETYPE, T, 16, 5, x8, 0, x0, 0, x0, 0, x0, 0, x0)\
    TESTCASE (TYPE, ETYPE, T, 16, 6, x8, x0, x1, 0, 1, x0, x1, 0, 1)\
    TESTCASE (TYPE, ETYPE, T, 16, 7, x8, 0, 1, x0, x1, 0, 1, x0, x1)\
    TESTCASE (TYPE, ETYPE, T, 16, 8, x8, 0, x0, 1, x1, 0, x0, 1, x1)\
    TESTCASE (TYPE, ETYPE, T, 16, 9, x8, x0, x1, x1, x2, x0, x1, x1, x2)\
    TESTCASE (TYPE, ETYPE, T, 16, 10, x8, x0, x1, x2, x1, x0, x1, x2, x1)

#define TEST_32(TYPE, ETYPE, T)\
    TESTCASE (TYPE, ETYPE, T, 32, 1, x4, x0, x0, x0, x0)\
    TESTCASE (TYPE, ETYPE, T, 32, 2, x4, x0, x1, x0, x1)\
    TESTCASE (TYPE, ETYPE, T, 32, 3, x4, x0, 0, x0, 0)\
    TESTCASE (TYPE, ETYPE, T, 32, 4, x4, 0, x0, 0, x0)

#define TEST_64(TYPE, ETYPE, T)\
    TESTCASE (TYPE, ETYPE, T, 64, 1, x2, x0, x0)

TEST_8(int, int8_t, s)

TEST_16(float, float, f)
TEST_16(int, int16_t, s)

TEST_32(float, float, f)
TEST_32(int, int32_t, s)

TEST_64(float, double, f)
TEST_64(int, int64_t, s)

/*
** test_int8_1:
**	dup	v0\.16b, w0
**	ret
*/
	
/*
** test_int8_2:
**	bfi	w0, w1, 8, 8
**	dup	v0\.8h, w0
**	ret
*/

/*
** test_int8_3:
**	bfi	w0, w1, 8, 8
**	bfi	w0, w2, 16, 8
**	bfi	w0, w3, 24, 8
**	dup	v0\.4s, w0
**	ret
*/

/*
** test_int8_4:
**	bfi	w0, w2, 8, 8
**	bfi	w1, w3, 8, 8
**	bfi	w0, w4, 16, 8
**	bfi	w1, w5, 16, 8
**	bfi	w0, w6, 24, 8
**	bfi	w1, w7, 24, 8
**	dup	v31\.2s, w0
**	dup	v0\.2s, w1
**	zip1	v0\.16b, v31\.16b, v0\.16b
**	ret
*/

/*
** test_int8_5:
**	uxtb	w0, w0
**	dup	v0\.8h, w0
**	ret
*/

/*
** test_int8_6:
**	mov	w1, 0
**	bfi	w1, w0, 8, 8
**	dup	v0\.8h, w1
**	ret
*/

/*
** test_int8_7:
**	mov	w2, 16777472
**	bfi	w2, w0, 0, 8
**	bfi	w2, w1, 8, 8
**	dup	v0\.4s, w2
**	ret
*/

/*
** test_int8_8:
**	mov	w2, 16777472
**	bfi	w2, w0, 16, 8
**	bfi	w2, w1, 24, 8
**	dup	v0\.4s, w2
**	ret
*/

/*
** test_int8_9:
**	mov	w2, 16777216
**	bfi	w2, w0, 0, 8
**	bfi	w2, w1, 16, 8
**	dup	v0\.4s, w2
**	ret
*/

/*
** test_int8_10:
**	bfi	w0, w1, 8, 8
**	bfi	w0, w2, 16, 8
**	bfi	w0, w3, 24, 8
**	dup	v31\.2s, w0
**	adrp	x0, .LANCHOR[0-9]+
**	ldr	d0, \[x0, #:lo12:.LANCHOR[0-9]+\]
**	zip1	v0\.16b, v31\.16b, v0\.16b
**	ret
*/

/*
** test_int8_11:
**	bfi	w0, w1, 8, 8
**	adrp	x4, .LANCHOR[0-9]+
**	bfi	w0, w2, 16, 8
**	ldr	d0, \[x4, #:lo12:\.LANCHOR[0-9]+\]
**	bfi	w0, w3, 24, 8
**	dup	v31\.2s, w0
**	zip1	v0\.16b, v0\.16b, v31\.16b
**	ret
*/

/*
** test_int8_12:
**	mov	w4, 33685504
**	bfi	w4, w0, 0, 8
**	mov	w0, 257
**	movk	w0, 0x303, lsl 16
**	bfi	w0, w1, 0, 8
**	bfi	w4, w2, 16, 8
**	bfi	w0, w3, 16, 8
**	dup	v31\.2s, w4
**	dup	v0\.2s, w0
**	zip1	v0\.16b, v31\.16b, v0\.16b
**	ret
*/

/*
** test_int8_13:
**	mov	w4, 33685504
**	bfi	w4, w0, 8, 8
**	mov	w0, 257
**	movk	w0, 0x303, lsl 16
**	bfi	w0, w1, 8, 8
**	bfi	w4, w2, 24, 8
**	bfi	w0, w3, 24, 8
**	dup	v31\.2s, w4
**	dup	v0\.2s, w0
**	zip1	v0\.16b, v31\.16b, v0\.16b
**	ret
*/

/*
** test_int8_14:
**	dup	v31\.8b, w1
**	fmov	w1, s31
**	bfi	w1, w0, 0, 8
**	bfi	w1, w2, 24, 8
**	dup	v0\.4s, w1
**	ret
*/

/*
** test_int8_15:
**	dup	v31\.8b, w1
**	fmov	w1, s31
**	bfi	w1, w0, 0, 8
**	bfi	w1, w2, 16, 8
**	dup	v0\.4s, w1
**	ret
*/

/*
** test_float16_1:
**	fcvt	h0, s0
**	dup	v0\.8h, v0\.h\[0\]
**	ret
*/

/*
** test_float16_2:
**	fcvt	h1, s1
**	fcvt	h0, s0
**	zip1	v0\.4h, v0\.4h, v1\.4h
**	dup	v0\.4s, v0\.s\[0\]
**	ret
*/

/*
** test_float16_3:
**	zip1	v2\.2s, v0\.2s, v2\.2s
**	zip1	v3\.2s, v1\.2s, v3\.2s
**	zip1	v3\.4s, v2\.4s, v3\.4s
**	fcvtn	v3\.4h, v3\.4s
**	dup	v0\.2d, v3\.d\[0\]
**	ret
*/

/*
** test_float16_4:
**	fcvt	h0, s0
**	fmov	h0, h0
**	dup	v0\.4s, v0\.s\[0\]
**	ret
*/

/*
** test_float16_5:
**	movi	v31\.4h, #0
**	fcvt	h0, s0
**	zip1	v0\.4h, v31\.4h, v0\.4h
**	dup	v0\.4s, v0\.s\[0\]
**	ret
*/

/*
** test_float16_6:
**	fcvt	h0, s0
**	fcvt	h1, s1
**	fmov	h31, 1.0e\+0
**	fmov	h0, h0
**	zip1	v1\.4h, v1\.4h, v31\.4h
**	dup	v0\.2s, v0\.s\[0\]
**	dup	v1\.2s, v1\.s\[0\]
**	zip1	v0\.8h, v0\.8h, v1\.8h
**	ret
*/

/*
** test_float16_7:
**	fcvt	h0, s0
**	movi	v31\.4h, #0
**	fcvt	h1, s1
**	zip1	v31\.4h, v31\.4h, v0\.4h
**	fmov	h0, 1.0e\+0
**	zip1	v0\.4h, v0\.4h, v1\.4h
**	dup	v31\.2s, v31\.s\[0\]
**	dup	v0\.2s, v0\.s\[0\]
**	zip1	v0\.8h, v31\.8h, v0\.8h
**	ret
*/

/*
** test_float16_8:
**	fcvt	h1, s1
**	fcvt	h0, s0
**	movi	v31\.2s, 0x3c, lsl 24
**	zip1	v0\.4h, v0\.4h, v1\.4h
**	dup	v0\.2s, v0\.s\[0\]
**	zip1	v0\.8h, v31\.8h, v0\.8h
**	ret
*/

/*
** test_float16_9:
**	fcvt	h1, s1
**	fcvt	h2, s2
**	fcvt	h0, s0
**	zip1	v0\.4h, v0\.4h, v1\.4h
**	zip1	v1\.4h, v1\.4h, v2\.4h
**	dup	v0\.2s, v0\.s\[0\]
**	dup	v1\.2s, v1\.s\[0\]
**	zip1	v0\.8h, v0\.8h, v1\.8h
**	ret
*/

/*
** test_float16_10:
**	fcvt	h2, s2
**	fcvt	h0, s0
**	fcvt	h1, s1
**	zip1	v0\.4h, v0\.4h, v2\.4h
**	dup	v1\.4h, v1\.h\[0\]
**	dup	v0\.2s, v0\.s\[0\]
**	zip1	v0\.8h, v0\.8h, v1\.8h
**	ret
*/

/*
** test_int16_1:
**	dup	v0\.8h, w0
**	ret
*/

/*
** test_int16_2:
**	bfi	w0, w1, 16, 16
**	dup	v0\.4s, w0
**	ret
*/

/*
** test_int16_3:
**	bfi	w0, w2, 16, 16
**	bfi	w1, w3, 16, 16
**	dup	v31\.2s, w0
**	dup	v0\.2s, w1
**	zip1	v0\.8h, v31\.8h, v0\.8h
**	ret
*/

/*
** test_int16_4:
**	uxth	w0, w0
**	dup	v0\.4s, w0
**	ret
*/

/*
** test_int16_5:
**	mov	w1, 0
**	bfi	w1, w0, 16, 16
**	dup	v0\.4s, w1
**	ret
*/

/*
** test_int16_6:
**	uxth	w0, w0
**	dup	v31\.2s, w0
**	mov	w0, 1
**	bfi	w1, w0, 16, 16
**	dup	v0\.2s, w1
**	zip1	v0\.8h, v31\.8h, v0\.8h
**	ret
*/

/*
** test_int16_7:
**	mov	w2, 0
**	bfi	w2, w0, 16, 16
**	mov	w0, 65537
**	bfi	w0, w1, 16, 16
**	dup	v31\.2s, w2
**	dup	v0\.2s, w0
**	zip1	v0\.8h, v31\.8h, v0\.8h
**	ret
*/

/*
** test_int16_8:
**	bfi	w0, w1, 16, 16
**	movi	v0\.2s, 0x1, lsl 16
**	dup	v31\.2s, w0
**	zip1	v0\.8h, v0\.8h, v31\.8h
**	ret
*/

/*
** test_int16_9:
**	bfi	w0, w1, 16, 16
**	bfi	w1, w2, 16, 16
**	dup	v31\.2s, w0
**	dup	v0\.2s, w1
**	zip1	v0\.8h, v31\.8h, v0\.8h
**	ret
*/

/*
** test_int16_10:
**	bfi	w0, w2, 16, 16
**	dup	v0\.4h, w1
**	dup	v31\.2s, w0
**	zip1	v0\.8h, v31\.8h, v0\.8h
**	ret
*/

/*
** test_float32_1:
**	dup	v0\.4s, v0\.s\[0\]
**	ret
*/

/*
** test_float32_2:
**	zip1	v0\.2s, v0\.2s, v1\.2s
**	dup	v0\.2d, v0\.d\[0\]
**	ret
*/

/*
** test_float32_3:
**	fmov	s0, s0
**	dup	v0\.2d, v0\.d\[0\]
**	ret
*/

/*
** test_float32_4:
**	movi	v31\.2s, #0
**	zip1	v0\.2s, v31\.2s, v0\.2s
**	dup	v0\.2d, v0\.d\[0\]
**	ret
*/

/*
** test_int32_1:
**	dup	v0\.4s, w0
**	ret
*/

/*
** test_int32_2:
**	fmov	s0, w0
**	ins	v0\.s\[1\], w1
**	dup	v0\.2d, v0\.d\[0\]
**	ret
*/

/*
** test_int32_3:
**	fmov	s0, w0
**	dup	v0\.2d, v0\.d\[0\]
**	ret
*/

/*
** test_int32_4:
**	dup	v31\.2s, w0
**	movi	v0\.2s, 0
**	zip1	v0\.4s, v0\.4s, v31\.4s
**	ret
*/

/*
** test_float64_1:
**	dup	v0\.2d, v0\.d\[0\]
**	ret
*/

/*
** test_int64_1:
**	dup	v0\.2d, x0	
**	ret
*/
