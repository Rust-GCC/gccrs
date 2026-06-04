/* { dg-do compile } */
/* { dg-options "-O2" } */
/* { dg-final { check-function-bodies "**" "" "" } } */

#include <arm_sve.h>

#define TESTCASE(TYPE, ETYPE, T, SZ, NUM, MULT, ...)\
  sv##TYPE##SZ##_t test_##TYPE##SZ##_##NUM (ETYPE x0, ETYPE x1, ETYPE x2, ETYPE x3,\
					    ETYPE x4, ETYPE x5, ETYPE x6, ETYPE x7)\
  {\
    return svdupq_n_##T##SZ (__VA_ARGS__);\
  }

#include "../vec-init-23.c"

/*
** test_int8_1:
**	mov	z0\.b, w0
**	ret
*/
	
/*
** test_int8_2:
**	bfi	w0, w1, 8, 8
**	mov	z0\.h, w0
**	ret
*/

/*
** test_int8_3:
**	bfi	w0, w1, 8, 8
**	bfi	w0, w2, 16, 8
**	bfi	w0, w3, 24, 8
**	mov	z0\.s, w0
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
**	dup	v30\.2s, w1
**	zip1	v31\.16b, v31\.16b, v30\.16b
**	dup	z0\.q, z31\.q\[0\]
**	ret
*/

/*
** test_int8_5:
**	uxtb	w0, w0
**	mov	z0\.h, w0
**	ret
*/

/*
** test_int8_6:
**	mov	w1, 0
**	bfi	w1, w0, 8, 8
**	mov	z0\.h, w1
**	ret
*/

/*
** test_int8_7:
**	mov	w2, 16777472
**	bfi	w2, w0, 0, 8
**	bfi	w2, w1, 8, 8
**	mov	z0\.s, w2
**	ret
*/

/*
** test_int8_8:
**	mov	w2, 16777472
**	bfi	w2, w0, 16, 8
**	bfi	w2, w1, 24, 8
**	mov	z0\.s, w2
**	ret
*/

/*
** test_int8_9:
**	mov	w2, 16777216
**	bfi	w2, w0, 0, 8
**	bfi	w2, w1, 16, 8
**	mov	z0.s, w2
**	ret
*/

/*
** test_int8_10:
**	bfi	w0, w1, 8, 8
**	bfi	w0, w2, 16, 8
**	bfi	w0, w3, 24, 8
**	dup	v31\.2s, w0
**	adrp	x0, .LANCHOR[0-9]+
**	ldr	d30, \[x0, #:lo12:.LANCHOR[0-9]+\]
**	zip1	v31\.16b, v31\.16b, v30\.16b
**	dup	z0\.q, z31\.q\[0\]
**	ret
*/

/*
** test_int8_11:
**	bfi	w0, w1, 8, 8
**	adrp	x4, .LANCHOR[0-9]+
**	bfi	w0, w2, 16, 8
**	ldr	d31, \[x4, #:lo12:\.LANCHOR[0-9]+\]
**	bfi	w0, w3, 24, 8
**	dup	v30\.2s, w0
**	zip1	v31\.16b, v31\.16b, v30\.16b
**	dup	z0\.q, z31\.q\[0\]
**	ret
*/

/*
** test_int8_12:
**	mov	w4, 255
**	movk	w4, 0x201, lsl 16
**	bfi	w4, w0, 0, 8
**	mov	w0, 256
**	movk	w0, 0x302, lsl 16
**	bfi	w0, w1, 0, 8
**	bfi	w4, w2, 16, 8
**	bfi	w0, w3, 16, 8
**	dup	v31\.2s, w4
**	dup	v30\.2s, w0
**	zip1	v31\.16b, v31\.16b, v30\.16b
**	dup	z0\.q, z31\.q\[0\]
**	ret
*/

/*
** test_int8_13:
**	mov	w4, 256
**	movk	w4, 0x302, lsl 16
**	bfi	w4, w0, 8, 8
**	mov	w0, 513
**	movk	w0, 0x403, lsl 16
**	bfi	w0, w1, 8, 8
**	bfi	w4, w2, 24, 8
**	bfi	w0, w3, 24, 8
**	dup	v31\.2s, w4
**	dup	v30\.2s, w0
**	zip1	v31\.16b, v31\.16b, v30\.16b
**	dup	z0\.q, z31\.q\[0\]
**	ret
*/

/*
** test_int8_14:
**	dup	v31\.8b, w1
**	fmov	w1, s31
**	bfi	w1, w0, 0, 8
**	bfi	w1, w2, 24, 8
**	mov	z0\.s, w1
**	ret
*/

/*
** test_int8_15:
**	dup	v31\.8b, w1
**	fmov	w1, s31
**	bfi	w1, w0, 0, 8
**	bfi	w1, w2, 16, 8
**	mov	z0\.s, w1
**	ret
*/

/*
** test_float16_1:
**	fcvt	h0, s0
**	mov	z0\.h, h0
**	ret
*/

/*
** test_float16_2:
**	fcvt	h1, s1
**	fcvt	h0, s0
**	uzp1	v0\.4h, v0\.4h, v1\.4h
**	mov	z0\.s, s0
**	ret
*/

/*
** test_float16_3:
**	uzp1	v2\.2s, v0\.2s, v2\.2s
**	uzp1	v3\.2s, v1\.2s, v3\.2s
**	zip1	v3\.4s, v2\.4s, v3\.4s
**	fcvtn	v3\.4h, v3\.4s
**	mov	z0\.d, d3
**	ret
*/

/*
** test_float16_4:
**	fcvt	h0, s0
**	fmov	h0, h0
**	mov	z0\.s, s0
**	ret
*/

/*
** test_float16_5:
**	movi	v31\.4h, #0
**	fcvt	h0, s0
**	uzp1	v0\.4h, v31\.4h, v0\.4h
**	mov	z0\.s, s0
**	ret
*/

/*
** test_float16_6:
**	fcvt	h2, s0
**	fcvt	h1, s1
**	fmov	h31, 1.0e\+0
**	fmov	h2, h2
**	uzp1	v1\.4h, v1\.4h, v31\.4h
**	dup	v0\.2s, v2\.s\[0\]
**	dup	v1\.2s, v1\.s\[0\]
**	zip1	v0\.8h, v0\.8h, v1\.8h
**	dup	z0\.q, z0\.q\[0\]
**	ret
*/

/*
** test_float16_7:
**	fcvt	h3, s0
**	fcvt	h2, s1
**	movi	v0\.4h, #0
**	fmov	h1, 1.0e\+0
**	uzp1	v1\.4h, v1\.4h, v2\.4h
**	uzp1	v0\.4h, v0\.4h, v3\.4h
**	dup	v1\.2s, v1\.s\[0\]
**	dup	v0\.2s, v0\.s\[0\]
**	zip1	v0\.8h, v0\.8h, v1\.8h
**	dup	z0\.q, z0\.q\[0\]
**	ret
*/

/*
** test_float16_8:
**	fcvt	h1, s1
**	fcvt	h0, s0
**	movi	v31\.2s, 0x3c, lsl 24
**	uzp1	v0\.4h, v0\.4h, v1.4h
**	dup	v0\.2s, v0\.s\[0\]
**	zip1	v0\.8h, v31\.8h, v0\.8h
**	dup	z0\.q, z0\.q\[0\]
**	ret
*/

/*
** test_float16_9:
**	fcvt	h1, s1
**	fcvt	h2, s2
**	fcvt	h0, s0
**	uzp1	v0\.4h, v0\.4h, v1\.4h
**	uzp1	v1\.4h, v1\.4h, v2\.4h
**	dup	v0\.2s, v0\.s\[0\]
**	dup	v1\.2s, v1\.s\[0\]
**	zip1	v0\.8h, v0\.8h, v1\.8h
**	dup	z0\.q, z0.q\[0\]
**	ret
*/

/*
** test_float16_10:
**	fcvt	h2, s2
**	fcvt	h0, s0
**	fcvt	h1, s1
**	uzp1	v0\.4h, v0\.4h, v2\.4h
**	dup	v1\.4h, v1\.h\[0\]
**	dup	v0\.2s, v0\.s\[0\]
**	zip1	v0\.8h, v0\.8h, v1\.8h
**	dup	z0\.q, z0.q\[0\]
**	ret
*/

/*
** test_int16_1:
**	mov	z0\.h, w0
**	ret
*/

/*
** test_int16_2:
**	bfi	w0, w1, 16, 16
**	mov	z0\.s, w0
**	ret
*/

/*
** test_int16_3:
**	bfi	w0, w2, 16, 16
**	bfi	w1, w3, 16, 16
**	dup	v31\.2s, w0
**	dup	v30\.2s, w1
**	zip1	v31\.8h, v31\.8h, v30\.8h
**	dup	z0\.q, z31\.q\[0\]
**	ret
*/

/*
** test_int16_4:
**	uxth	w0, w0
**	mov	z0\.s, w0
**	ret
*/

/*
** test_int16_5:
**	mov	w1, 0
**	bfi	w1, w0, 16, 16
**	mov	z0\.s, w1
**	ret
*/

/*
** test_int16_6:
**	uxth	w0, w0
**	dup	v31\.2s, w0
**	mov	w0, 1
**	bfi	w1, w0, 16, 16
**	dup	v30\.2s, w1
**	zip1	v31\.8h, v31\.8h, v30\.8h
**	dup	z0\.q, z31\.q\[0\]
**	ret
*/

/*
** test_int16_7:
**	mov	w2, 0
**	bfi	w2, w0, 16, 16
**	mov	w0, 65537
**	bfi	w0, w1, 16, 16
**	dup	v31\.2s, w2
**	dup	v30\.2s, w0
**	zip1	v31\.8h, v31\.8h, v30\.8h
**	dup	z0\.q, z31\.q\[0\]
**	ret
*/

/*
** test_int16_8:
**	bfi	w0, w1, 16, 16
**	movi	v31\.2s, 0x1, lsl 16
**	dup	v30\.2s, w0
**	zip1	v31\.8h, v31\.8h, v30\.8h
**	dup	z0\.q, z31\.q\[0\]
**	ret
*/

/*
** test_int16_9:
**	bfi	w0, w1, 16, 16
**	bfi	w1, w2, 16, 16
**	dup	v31\.2s, w0
**	dup	v30\.2s, w1
**	zip1	v31\.8h, v31\.8h, v30\.8h
**	dup	z0\.q, z31\.q\[0\]
**	ret
*/

/*
** test_int16_10:
**	bfi	w0, w2, 16, 16
**	dup	v30\.4h, w1
**	dup	v31\.2s, w0
**	zip1	v31\.8h, v31\.8h, v30\.8h
**	dup	z0\.q, z31\.q\[0\]
**	ret
*/

/*
** test_float32_1:
**	mov	z0\.s, s0
**	ret
*/

/*
** test_float32_2:
**	uzp1	v0\.2s, v0\.2s, v1\.2s
**	mov	z0\.d, d0
**	ret
*/

/*
** test_float32_3:
**	fmov	s0, s0
**	mov	z0\.d, d0
**	ret
*/

/*
** test_float32_4:
**	movi	v31\.2s, #0
**	uzp1	v0\.2s, v31\.2s, v0\.2s
**	mov	z0\.d, d0
**	ret
*/

/*
** test_int32_1:
**	mov	z0\.s, w0
**	ret
*/

/*
** test_int32_2:
**	fmov	s0, w0
**	ins	v0\.s\[1\], w1
**	mov	z0\.d, d0
**	ret
*/

/*
** test_int32_3:
**	fmov	s0, w0
**	mov	z0\.d, d0
**	ret
*/

/*
** test_int32_4:
**	dup	v30\.2s, w0
**	movi	v31\.2s, 0
**	zip1	v31\.4s, v31\.4s, v30\.4s
**	dup	z0\.q, z31\.q\[0\]
**	ret
*/

/*
** test_int64_1:
**	mov	z0\.d, x0	
**	ret
*/

/*
** test_float64_1:
**	mov	z0\.d, d0	
**	ret
*/

