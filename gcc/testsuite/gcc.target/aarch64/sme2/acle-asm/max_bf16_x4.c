/* { dg-do assemble { target aarch64_asm_sve-b16b16_ok } } */
/* { dg-do compile { target { ! aarch64_asm_sve-b16b16_ok } } } */
/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" } } */

#include "test_sme2_acle.h"

#pragma GCC target "+sve-b16b16"

/*
** max_z0_z0_z4:
**	bfmax	{z0\.h - z3\.h}, {z0\.h - z3\.h}, {z4\.h - z7\.h}
**	ret
*/
TEST_XN (max_z0_z0_z4, svbfloat16x4_t, z0,
	 svmax_bf16_x4 (z0, z4),
	 svmax (z0, z4))

/*
** max_z0_z4_z0:
**	bfmax	{z0\.h - z3\.h}, {z0\.h - z3\.h}, {z4\.h - z7\.h}
**	ret
*/
TEST_XN (max_z0_z4_z0, svbfloat16x4_t, z0,
	 svmax_bf16_x4 (z4, z0),
	 svmax (z4, z0))

/*
** max_z0_z4_z28:
** (
**	mov	[^\n]+
**	mov	[^\n]+
**	mov	[^\n]+
**	mov	[^\n]+
**	bfmax	[^\n]+, {z28\.h - z31\.h}
** |
**	bfmax	[^\n]+, {z28\.h - z31\.h}
**	mov	[^\n]+
**	mov	[^\n]+
**	mov	[^\n]+
**	mov	[^\n]+
** )
**	ret
*/
TEST_XN (max_z0_z4_z28, svbfloat16x4_t, z0,
	 svmax_bf16_x4 (z4, z28),
	 svmax (z4, z28))

/*
** max_z18_z18_z4:
**	mov	[^\n]+
**	mov	[^\n]+
**	mov	[^\n]+
**	mov	[^\n]+
**	bfmax	[^\n]+, {z4\.h - z7\.h}
**	mov	[^\n]+
**	mov	[^\n]+
**	mov	[^\n]+
**	mov	[^\n]+
**	ret
*/
TEST_XN (max_z18_z18_z4, svbfloat16x4_t, z18,
	 svmax_bf16_x4 (z18, z4),
	 svmax (z18, z4))

/*
** max_z23_z23_z28:
**	mov	[^\n]+
**	mov	[^\n]+
**	mov	[^\n]+
**	mov	[^\n]+
**	bfmax	[^\n]+, {z28\.h - z31\.h}
**	mov	[^\n]+
**	mov	[^\n]+
**	mov	[^\n]+
**	mov	[^\n]+
**	ret
*/
TEST_XN (max_z23_z23_z28, svbfloat16x4_t, z23,
	 svmax_bf16_x4 (z23, z28),
	 svmax (z23, z28))

/*
** max_z28_z28_z0:
**	bfmax	{z28\.h - z31\.h}, {z28\.h - z31\.h}, {z0\.h - z3\.h}
**	ret
*/
TEST_XN (max_z28_z28_z0, svbfloat16x4_t, z28,
	 svmax_bf16_x4 (z28, z0),
	 svmax (z28, z0))

/*
** max_z0_z0_z18:
** (
**	mov	[^\n]+
**	mov	[^\n]+
**	mov	[^\n]+
**	mov	[^\n]+
**	bfmax	{z0\.h - z3\.h}, {z0\.h - z3\.h}, [^\n]+
** |
**	bfmax	{z0\.h - z3\.h}, {z0\.h - z3\.h}, [^\n]+
**	mov	[^\n]+
**	mov	[^\n]+
**	mov	[^\n]+
**	mov	[^\n]+
** )
**	ret
*/
TEST_XN (max_z0_z0_z18, svbfloat16x4_t, z0,
	 svmax_bf16_x4 (z0, z18),
	 svmax (z0, z18))

/*
** max_z4_z4_z23:
** (
**	mov	[^\n]+
**	mov	[^\n]+
**	mov	[^\n]+
**	mov	[^\n]+
**	bfmax	{z4\.h - z7\.h}, {z4\.h - z7\.h}, [^\n]+
** |
**	bfmax	{z4\.h - z7\.h}, {z4\.h - z7\.h}, [^\n]+
**	mov	[^\n]+
**	mov	[^\n]+
**	mov	[^\n]+
**	mov	[^\n]+
** )
**	ret
*/
TEST_XN (max_z4_z4_z23, svbfloat16x4_t, z4,
	 svmax_bf16_x4 (z4, z23),
	 svmax (z4, z23))

/*
** max_single_z24_z24_z0:
**	bfmax	{z24\.h - z27\.h}, {z24\.h - z27\.h}, z0\.h
**	ret
*/
TEST_XN_SINGLE (max_single_z24_z24_z0, svbfloat16x4_t, svbfloat16_t, z24,
		svmax_single_bf16_x4 (z24, z0),
		svmax (z24, z0))

/*
** max_single_z24_z28_z0:
** (
**	mov	[^\n]+
**	mov	[^\n]+
**	mov	[^\n]+
**	mov	[^\n]+
**	bfmax	{z24\.h - z27\.h}, {z24\.h - z27\.h}, z0\.h
** |
**	bfmax	{z28\.h - z31\.h}, {z28\.h - z31\.h}, z0\.h
**	mov	[^\n]+
**	mov	[^\n]+
**	mov	[^\n]+
**	mov	[^\n]+
** )
**	ret
*/
TEST_XN_SINGLE (max_single_z24_z28_z0, svbfloat16x4_t, svbfloat16_t, z24,
		svmax_single_bf16_x4 (z28, z0),
		svmax (z28, z0))

/*
** max_single_z24_z1_z0:
**	mov	[^\n]+
**	mov	[^\n]+
**	mov	[^\n]+
**	mov	[^\n]+
**	bfmax	{z24\.h - z27\.h}, {z24\.h - z27\.h}, z0\.h
**	ret
*/
TEST_XN_SINGLE (max_single_z24_z1_z0, svbfloat16x4_t, svbfloat16_t, z24,
		svmax_single_bf16_x4 (z1, z0),
		svmax (z1, z0))

/*
** max_single_z1_z24_z0:
**	bfmax	{z24\.h - z27\.h}, {z24\.h - z27\.h}, z0\.h
**	mov	[^\n]+
**	mov	[^\n]+
**	mov	[^\n]+
**	mov	[^\n]+
**	ret
*/
TEST_XN_SINGLE (max_single_z1_z24_z0, svbfloat16x4_t, svbfloat16_t, z1,
		svmax_single_bf16_x4 (z24, z0),
		svmax (z24, z0))

/*
** max_single_z1_z1_z0:
**	mov	[^\n]+
**	mov	[^\n]+
**	mov	[^\n]+
**	mov	[^\n]+
**	bfmax	({z[0-9]+\.h - z[0-9]+\.h}), \1, z0\.h
**	mov	[^\n]+
**	mov	[^\n]+
**	mov	[^\n]+
**	mov	[^\n]+
**	ret
*/
TEST_XN_SINGLE (max_single_z1_z1_z0, svbfloat16x4_t, svbfloat16_t, z1,
		svmax_single_bf16_x4 (z1, z0),
		svmax (z1, z0))

/*
** max_single_z18_z18_z0:
**	mov	[^\n]+
**	mov	[^\n]+
**	mov	[^\n]+
**	mov	[^\n]+
**	bfmax	[^\n]+, z0\.h
**	mov	[^\n]+
**	mov	[^\n]+
**	mov	[^\n]+
**	mov	[^\n]+
**	ret
*/
TEST_XN_SINGLE (max_single_z18_z18_z0, svbfloat16x4_t, svbfloat16_t, z18,
		svmax_single_bf16_x4 (z18, z0),
		svmax (z18, z0))

/*
** max_single_awkward:
**	...
**	bfmax	({z[0-9]+\.h - z[0-9]+\.h}), \1, z[0-9]+\.h
**	...
**	ret
*/
TEST_XN_SINGLE_AWKWARD (max_single_awkward, svbfloat16x4_t, svbfloat16_t,
			z0_res = svmax_single_bf16_x4 (z1, z0),
			z0_res = svmax (z1, z0))

/*
** max_single_z0_z0_z15:
**	...
**	bfmax	{z0\.h - z3\.h}, {z0\.h - z3\.h}, z15\.h
**	...
**	ret
*/
TEST_XN_SINGLE_Z15 (max_single_z0_z0_z15, svbfloat16x4_t, svbfloat16_t,
		    z0 = svmax_single_bf16_x4 (z0, z15),
		    z0 = svmax (z0, z15))

/*
** max_single_z24_z24_z16:
**	mov	(z[0-7])\.d, z16\.d
**	bfmax	{z24\.h - z27\.h}, {z24\.h - z27\.h}, \1\.h
**	ret
*/
TEST_XN_SINGLE (max_single_z24_z24_z16, svbfloat16x4_t, svbfloat16_t, z24,
		svmax_single_bf16_x4 (z24, z16),
		svmax (z24, z16))
