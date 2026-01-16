/* { dg-do assemble { target aarch64_asm_sme2p2_ok } } */
/* { dg-do compile { target { ! aarch64_asm_sme2p2_ok } } } */
/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" } } */

#include "test_sme2_acle.h"
#pragma GCC target "+sme2p2"

/*
** mul_z0_z0_z4:
**	fmul	{z0\.s - z1\.s}, {z0\.s - z1\.s}, {z4\.s - z5\.s}
**	ret
*/
TEST_XN (mul_z0_z0_z4, svfloat32x2_t, z0,
	 svmul_f32_x2 (z0, z4),
	 svmul (z0, z4))

/*
** mul_z0_z4_z0:
**	fmul	{z0\.s - z1\.s}, {z4\.s - z5\.s}, {z0\.s - z1\.s}
**	ret
*/
TEST_XN (mul_z0_z4_z0, svfloat32x2_t, z0,
	 svmul_f32_x2 (z4, z0),
	 svmul (z4, z0))

/*
** mul_z0_z4_z28:
**	fmul	{z0\.s - z1\.s}, {z4\.s - z5\.s}, {z28\.s - z29\.s}
**	ret
*/
TEST_XN (mul_z0_z4_z28, svfloat32x2_t, z0,
	 svmul_f32_x2 (z4, z28),
	 svmul (z4, z28))

/*
** mul_z18_z18_z4:
**	fmul	{z18\.s - z19\.s}, {z18\.s - z19\.s}, {z4\.s - z5\.s}
**	ret
*/
TEST_XN (mul_z18_z18_z4, svfloat32x2_t, z18,
	 svmul_f32_x2 (z18, z4),
	 svmul (z18, z4))

/*
** mul_z23_z23_z18:
**	mov	[^\n]+
**	mov	[^\n]+
**	fmul	[^\n]+, {z18\.s - z19\.s}
**	mov	[^\n]+
**	mov	[^\n]+
**	ret
*/
TEST_XN (mul_z23_z23_z18, svfloat32x2_t, z23,
	 svmul_f32_x2 (z23, z18),
	 svmul (z23, z18))

/*
** mul_z28_z28_z0:
**	fmul	{z28\.s - z29\.s}, {z28\.s - z29\.s}, {z0\.s - z1\.s}
**	ret
*/
TEST_XN (mul_z28_z28_z0, svfloat32x2_t, z28,
	 svmul_f32_x2 (z28, z0),
	 svmul (z28, z0))

/*
** mul_z0_z0_z18:
**	fmul	{z0\.s - z1\.s}, {z0\.s - z1\.s}, {z18\.s - z19\.s}
**	ret
*/
TEST_XN (mul_z0_z0_z18, svfloat32x2_t, z0,
	 svmul_f32_x2 (z0, z18),
	 svmul (z0, z18))

/*
** mul_z4_z4_z23:
** (
**	mov	[^\n]+
**	mov	[^\n]+
**	fmul	{z4\.s - z5\.s}, {z4\.s - z5\.s}, [^\n]+
** |
**	fmul	{z4\.s - z5\.s}, {z4\.s - z5\.s}, [^\n]+
**	mov	[^\n]+
**	mov	[^\n]+
** )
**	ret
*/
TEST_XN (mul_z4_z4_z23, svfloat32x2_t, z4,
	 svmul_f32_x2 (z4, z23),
	 svmul (z4, z23))

/*
** mul_single_z24_z24_z0:
**	fmul	{z24\.s - z25\.s}, {z24\.s - z25\.s}, z0\.s
**	ret
*/
TEST_XN_SINGLE (mul_single_z24_z24_z0, svfloat32x2_t, svfloat32_t, z24,
		svmul_single_f32_x2 (z24, z0),
		svmul (z24, z0))

/*
** mul_single_z24_z28_z0:
**	fmul	{z24\.s - z25\.s}, {z28\.s - z29\.s}, z0\.s
**	ret
*/
TEST_XN_SINGLE (mul_single_z24_z28_z0, svfloat32x2_t, svfloat32_t, z24,
		svmul_single_f32_x2 (z28, z0),
		svmul (z28, z0))

/*
** mul_single_z24_z1_z0:
** (
**	mov	z30\.d, z1\.d
**	mov	z31\.d, z2\.d
** |
**	mov	z31\.d, z2\.d
**	mov	z30\.d, z1\.d
** )
**	fmul	{z24\.s - z25\.s}, {z30\.s - z31\.s}, z0\.s
**	ret
*/
TEST_XN_SINGLE (mul_single_z24_z1_z0, svfloat32x2_t, svfloat32_t, z24,
		svmul_single_f32_x2 (z1, z0),
		svmul (z1, z0))

/*
** mul_single_z1_z24_z0:
**	fmul	{z30\.s - z31\.s}, {z24\.s - z25\.s}, z0\.s
** (
**	mov	z2\.d, z31\.d
**	mov	z1\.d, z30\.d
** |
**	mov	z1\.d, z30\.d
**	mov	z2\.d, z31\.d
** )
**	ret
*/
TEST_XN_SINGLE (mul_single_z1_z24_z0, svfloat32x2_t, svfloat32_t, z1,
		svmul_single_f32_x2 (z24, z0),
		svmul (z24, z0))

/*
** mul_single_z1_z1_z0:
**	mov	[^\n]+
**	mov	[^\n]+
**	fmul	({z[0-9]+\.s - z[0-9]+\.s}), \1, z0\.s
**	mov	[^\n]+
**	mov	[^\n]+
**	ret
*/
TEST_XN_SINGLE (mul_single_z1_z1_z0, svfloat32x2_t, svfloat32_t, z1,
		svmul_single_f32_x2 (z1, z0),
		svmul (z1, z0))

/*
** mul_single_z18_z18_z0:
**	fmul	{z18\.s - z19\.s}, {z18\.s - z19\.s}, z0\.s
**	ret
*/
TEST_XN_SINGLE (mul_single_z18_z18_z0, svfloat32x2_t, svfloat32_t, z18,
		svmul_single_f32_x2 (z18, z0),
		svmul (z18, z0))

/*
** mul_single_awkward:
**	...
**	fmul	{z0\.s - z1\.s}, {z30\.s - z31\.s}, z[0-9]+\.s
**	ret
*/
TEST_XN_SINGLE_AWKWARD (mul_single_awkward, svfloat32x2_t, svfloat32_t,
			z0_res = svmul_single_f32_x2 (z1, z0),
			z0_res = svmul (z1, z0))

/*
** mul_single_z0_z0_z15:
**	...
**	fmul	{z0\.s - z1\.s}, {z0\.s - z1\.s}, z15\.s
**	...
**	ret
*/
TEST_XN_SINGLE_Z15 (mul_single_z0_z0_z15, svfloat32x2_t, svfloat32_t,
		    z0 = svmul_single_f32_x2 (z0, z15),
		    z0 = svmul (z0, z15))

/*
** mul_single_z24_z24_z16:
**	mov	(z[0-7])\.d, z16\.d
**	fmul	{z24\.s - z25\.s}, {z24\.s - z25\.s}, \1\.s
**	ret
*/
TEST_XN_SINGLE (mul_single_z24_z24_z16, svfloat32x2_t, svfloat32_t, z24,
		svmul_single_f32_x2 (z24, z16),
		svmul (z24, z16))
