/* { dg-do assemble { target aarch64_asm_sme-f16f16_ok } } */
/* { dg-do compile { target { ! aarch64_asm_sme-f16f16_ok } } } */
/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" } } */

#include "test_sme2_acle.h"

#pragma GCC target "+sme-f16f16"

/*
** mla_0_z0_z0:
**	mov	(w8|w9|w10|w11), #?0
**	fmla	za\.h\[\1, 0, vgx4\], {z0\.h - z3\.h}, {z0\.h - z3\.h}
**	ret
*/
TEST_ZA_XN (mla_0_z0_z0, svfloat16x4_t,
	    svmla_za16_f16_vg1x4 (0, z0, z0),
	    svmla_za16_vg1x4 (0, z0, z0))

/*
** mla_w0_z0_z0:
**	mov	(w8|w9|w10|w11), w0
**	fmla	za\.h\[\1, 0, vgx4\], {z0\.h - z3\.h}, {z0\.h - z3\.h}
**	ret
*/
TEST_ZA_XN (mla_w0_z0_z0, svfloat16x4_t,
	    svmla_za16_f16_vg1x4 (w0, z0, z0),
	    svmla_za16_vg1x4 (w0, z0, z0))

/*
** mla_w8_z0_z4:
**	fmla	za\.h\[w8, 0, vgx4\], {z0\.h - z3\.h}, {z4\.h - z7\.h}
**	ret
*/
TEST_ZA_XN (mla_w8_z0_z4, svfloat16x4_t,
	    svmla_za16_f16_vg1x4 (w8, z0, z4),
	    svmla_za16_vg1x4 (w8, z0, z4))

/* Leave the assembler to check for correctness for misaligned registers.  */

/*
** mla_w8_z0_z18:
**	...
**	fmla	za\.h\[w8, 0, vgx4\], {z0\.h - z3\.h}, [^\n]+
**	ret
*/
TEST_ZA_XN (mla_w8_z0_z18, svfloat16x4_t,
	    svmla_za16_f16_vg1x4 (w8, z0, z18),
	    svmla_za16_vg1x4 (w8, z0, z18))

/*
** mla_w8_z18_z28:
**	...
**	fmla	za\.h\[w8, 0, vgx4\], [^\n]+, {z28\.h - z31\.h}
**	ret
*/
TEST_ZA_XN (mla_w8_z18_z28, svfloat16x4_t,
	    svmla_za16_f16_vg1x4 (w8, z18, z28),
	    svmla_za16_vg1x4 (w8, z18, z28))

/*
** mla_w8_z28_z23:
**	...
**	fmla	za\.h\[w8, 0, vgx4\], {z28\.h - z31\.h}, [^\n]+
**	ret
*/
TEST_ZA_XN (mla_w8_z28_z23, svfloat16x4_t,
	    svmla_za16_f16_vg1x4 (w8, z28, z23),
	    svmla_za16_vg1x4 (w8, z28, z23))

/*
** mla_w8p7_z4_z0:
**	fmla	za\.h\[w8, 7, vgx4\], {z4\.h - z7\.h}, {z0\.h - z3\.h}
**	ret
*/
TEST_ZA_XN (mla_w8p7_z4_z0, svfloat16x4_t,
	    svmla_za16_f16_vg1x4 (w8 + 7, z4, z0),
	    svmla_za16_vg1x4 (w8 + 7, z4, z0))

/*
** mla_w8p8_z4_z4:
**	add	(w8|w9|w10|w11), w8, #?8
**	fmla	za\.h\[\1, 0, vgx4\], {z4\.h - z7\.h}, {z4\.h - z7\.h}
**	ret
*/
TEST_ZA_XN (mla_w8p8_z4_z4, svfloat16x4_t,
	    svmla_za16_f16_vg1x4 (w8 + 8, z4, z4),
	    svmla_za16_vg1x4 (w8 + 8, z4, z4))

/*
** mla_w8m1_z4_z0:
**	sub	(w8|w9|w10|w11), w8, #?1
**	fmla	za\.h\[\1, 0, vgx4\], {z4\.h - z7\.h}, {z0\.h - z3\.h}
**	ret
*/
TEST_ZA_XN (mla_w8m1_z4_z0, svfloat16x4_t,
	    svmla_za16_f16_vg1x4 (w8 - 1, z4, z0),
	    svmla_za16_vg1x4 (w8 - 1, z4, z0))

/*
** mla_single_0_z1_z0:
**	mov	(w8|w9|w10|w11), #?0
**	fmla	za\.h\[\1, 0, vgx4\], {z1\.h - z4\.h}, z0\.h
**	ret
*/
TEST_ZA_SINGLE (mla_single_0_z1_z0, svfloat16x4_t, svfloat16_t,
	        svmla_single_za16_f16_vg1x4 (0, z1, z0),
	        svmla_za16_vg1x4 (0, z1, z0))

/*
** mla_single_w0_z1_z0:
**	mov	(w8|w9|w10|w11), w0
**	fmla	za\.h\[\1, 0, vgx4\], {z1\.h - z4\.h}, z0\.h
**	ret
*/
TEST_ZA_SINGLE (mla_single_w0_z1_z0, svfloat16x4_t, svfloat16_t,
	        svmla_single_za16_f16_vg1x4 (w0, z1, z0),
	        svmla_za16_vg1x4 (w0, z1, z0))

/*
** mla_single_w8_z1_z0:
**	fmla	za\.h\[w8, 0, vgx4\], {z1\.h - z4\.h}, z0\.h
**	ret
*/
TEST_ZA_SINGLE (mla_single_w8_z1_z0, svfloat16x4_t, svfloat16_t,
	        svmla_single_za16_f16_vg1x4 (w8, z1, z0),
	        svmla_za16_vg1x4 (w8, z1, z0))

/*
** mla_single_w8p7_z1_z0:
**	fmla	za\.h\[w8, 7, vgx4\], {z1\.h - z4\.h}, z0\.h
**	ret
*/
TEST_ZA_SINGLE (mla_single_w8p7_z1_z0, svfloat16x4_t, svfloat16_t,
	        svmla_single_za16_f16_vg1x4 (w8 + 7, z1, z0),
	        svmla_za16_vg1x4 (w8 + 7, z1, z0))

/*
** mla_single_w8p8_z1_z0:
**	add	(w8|w9|w10|w11), w8, #?8
**	fmla	za\.h\[\1, 0, vgx4\], {z1\.h - z4\.h}, z0\.h
**	ret
*/
TEST_ZA_SINGLE (mla_single_w8p8_z1_z0, svfloat16x4_t, svfloat16_t,
	        svmla_single_za16_f16_vg1x4 (w8 + 8, z1, z0),
	        svmla_za16_vg1x4 (w8 + 8, z1, z0))

/*
** mla_single_w0m1_z1_z0:
**	sub	(w8|w9|w10|w11), w0, #?1
**	fmla	za\.h\[\1, 0, vgx4\], {z1\.h - z4\.h}, z0\.h
**	ret
*/
TEST_ZA_SINGLE (mla_single_w0m1_z1_z0, svfloat16x4_t, svfloat16_t,
	        svmla_single_za16_f16_vg1x4 (w0 - 1, z1, z0),
	        svmla_za16_vg1x4 (w0 - 1, z1, z0))

/*
** mla_single_w8_z0_z15:
**	str	d15, \[sp, #?-16\]!
**	fmla	za\.h\[w8, 0, vgx4\], {z0\.h - z3\.h}, z15\.h
**	ldr	d15, \[sp\], #?16
**	ret
*/
TEST_ZA_SINGLE_Z15 (mla_single_w8_z0_z15, svfloat16x4_t, svfloat16_t,
		    svmla_single_za16_f16_vg1x4 (w8, z0, z15),
		    svmla_za16_vg1x4 (w8, z0, z15))

/*
** mla_single_w8_z20_z16:
**	mov	(z[0-7]).d, z16.d
**	fmla	za\.h\[w8, 0, vgx4\], {z20\.h - z23\.h}, \1\.h
**	ret
*/
TEST_ZA_SINGLE (mla_single_w8_z20_z16, svfloat16x4_t, svfloat16_t,
	        svmla_single_za16_f16_vg1x4 (w8, z20, z16),
	        svmla_za16_vg1x4 (w8, z20, z16))
