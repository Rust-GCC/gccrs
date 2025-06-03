/* { dg-do assemble { target aarch64_asm_sme-b16b16_ok } } */
/* { dg-do compile { target { ! aarch64_asm_sme-b16b16_ok } } } */
/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" } } */

#include "test_sme2_acle.h"

#pragma GCC target "+sme-b16b16"

/*
** mls_lane_0_z0_z4_0:
**	mov	(w8|w9|w10|w11), #?0
**	bfmls	za\.h\[\1, 0, vgx4\], {z0\.h - z3\.h}, z4\.h\[0\]
**	ret
*/
TEST_ZA_LANE (mls_lane_0_z0_z4_0, svbfloat16x4_t, svbfloat16_t,
	      svmls_lane_za16_bf16_vg1x4 (0, z0, z4, 0),
	      svmls_lane_za16_vg1x4 (0, z0, z4, 0))

/*
** mls_lane_w0_z0_z7_1:
**	mov	(w8|w9|w10|w11), w0
**	bfmls	za\.h\[\1, 0, vgx4\], {z0\.h - z3\.h}, z7\.h\[1\]
**	ret
*/
TEST_ZA_LANE (mls_lane_w0_z0_z7_1, svbfloat16x4_t, svbfloat16_t,
	      svmls_lane_za16_bf16_vg1x4 (w0, z0, z7, 1),
	      svmls_lane_za16_vg1x4 (w0, z0, z7, 1))

/*
** mls_lane_w8_z28_z4_2:
**	bfmls	za\.h\[w8, 0, vgx4\], {z28\.h - z31\.h}, z4\.h\[2\]
**	ret
*/
TEST_ZA_LANE (mls_lane_w8_z28_z4_2, svbfloat16x4_t, svbfloat16_t,
	      svmls_lane_za16_bf16_vg1x4 (w8, z28, z4, 2),
	      svmls_lane_za16_vg1x4 (w8, z28, z4, 2))

/*
** mls_lane_w8p7_z0_z4_3:
**	bfmls	za\.h\[w8, 7, vgx4\], {z0\.h - z3\.h}, z4\.h\[3\]
**	ret
*/
TEST_ZA_LANE (mls_lane_w8p7_z0_z4_3, svbfloat16x4_t, svbfloat16_t,
	      svmls_lane_za16_bf16_vg1x4 (w8 + 7, z0, z4, 3),
	      svmls_lane_za16_vg1x4 (w8 + 7, z0, z4, 3))

/*
** mls_lane_w8p8_z0_z4_4:
**	add	(w8|w9|w10|w11), w8, #?8
**	bfmls	za\.h\[\1, 0, vgx4\], {z0\.h - z3\.h}, z4\.h\[4\]
**	ret
*/
TEST_ZA_LANE (mls_lane_w8p8_z0_z4_4, svbfloat16x4_t, svbfloat16_t,
	      svmls_lane_za16_bf16_vg1x4 (w8 + 8, z0, z4, 4),
	      svmls_lane_za16_vg1x4 (w8 + 8, z0, z4, 4))

/*
** mls_lane_w0m1_z0_z4_5:
**	sub	(w8|w9|w10|w11), w0, #?1
**	bfmls	za\.h\[\1, 0, vgx4\], {z0\.h - z3\.h}, z4\.h\[5\]
**	ret
*/
TEST_ZA_LANE (mls_lane_w0m1_z0_z4_5, svbfloat16x4_t, svbfloat16_t,
	      svmls_lane_za16_bf16_vg1x4 (w0 - 1, z0, z4, 5),
	      svmls_lane_za16_vg1x4 (w0 - 1, z0, z4, 5))

/*
** mls_lane_w8_z4_z15_6:
**	str	d15, \[sp, #?-16\]!
**	bfmls	za\.h\[w8, 0, vgx4\], {z4\.h - z7\.h}, z15\.h\[6\]
**	ldr	d15, \[sp\], #?16
**	ret
*/
TEST_ZA_LANE_Z15 (mls_lane_w8_z4_z15_6, svbfloat16x4_t, svbfloat16_t,
		  svmls_lane_za16_bf16_vg1x4 (w8, z4, z15, 6),
		  svmls_lane_za16_vg1x4 (w8, z4, z15, 6))

/*
** mls_lane_w8_z28_z16_7:
**	mov	(z[0-7]).d, z16.d
**	bfmls	za\.h\[w8, 0, vgx4\], {z28\.h - z31\.h}, \1\.h\[7\]
**	ret
*/
TEST_ZA_LANE (mls_lane_w8_z28_z16_7, svbfloat16x4_t, svbfloat16_t,
	      svmls_lane_za16_bf16_vg1x4 (w8, z28, z16, 7),
	      svmls_lane_za16_vg1x4 (w8, z28, z16, 7))

/*
** mls_lane_w8_z17_z7_0:
**	mov	[^\n]+
**	mov	[^\n]+
**	mov	[^\n]+
**	mov	[^\n]+
**	bfmls	za\.h\[w8, 0, vgx4\], [^\n]+, z7\.h\[0\]
**	ret
*/
TEST_ZA_LANE (mls_lane_w8_z17_z7_0, svbfloat16x4_t, svbfloat16_t,
	      svmls_lane_za16_bf16_vg1x4 (w8, z17, z7, 0),
	      svmls_lane_za16_vg1x4 (w8, z17, z7, 0))

/*
** mls_lane_w8_z22_z4_1:
**	mov	[^\n]+
**	mov	[^\n]+
**	mov	[^\n]+
**	mov	[^\n]+
**	bfmls	za\.h\[w8, 0, vgx4\], [^\n]+, z4\.h\[1\]
**	ret
*/
TEST_ZA_LANE (mls_lane_w8_z22_z4_1, svbfloat16x4_t, svbfloat16_t,
	      svmls_lane_za16_bf16_vg1x4 (w8, z22, z4, 1),
	      svmls_lane_za16_vg1x4 (w8, z22, z4, 1))
