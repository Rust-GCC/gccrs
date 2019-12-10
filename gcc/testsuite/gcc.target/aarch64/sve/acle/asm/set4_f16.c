/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" } } */

#include "test_sve_acle.h"

/*
** set4_f16_z24_0:
**	mov	z25\.d, z5\.d
**	mov	z26\.d, z6\.d
**	mov	z27\.d, z7\.d
**	mov	z24\.d, z0\.d
**	ret
*/
TEST_SET (set4_f16_z24_0, svfloat16x4_t, svfloat16_t,
	  z24 = svset4_f16 (z4, 0, z0),
	  z24 = svset4 (z4, 0, z0))

/*
** set4_f16_z24_1:
**	mov	z24\.d, z4\.d
**	mov	z26\.d, z6\.d
**	mov	z27\.d, z7\.d
**	mov	z25\.d, z0\.d
**	ret
*/
TEST_SET (set4_f16_z24_1, svfloat16x4_t, svfloat16_t,
	  z24 = svset4_f16 (z4, 1, z0),
	  z24 = svset4 (z4, 1, z0))

/*
** set4_f16_z24_2:
**	mov	z24\.d, z4\.d
**	mov	z25\.d, z5\.d
**	mov	z27\.d, z7\.d
**	mov	z26\.d, z0\.d
**	ret
*/
TEST_SET (set4_f16_z24_2, svfloat16x4_t, svfloat16_t,
	  z24 = svset4_f16 (z4, 2, z0),
	  z24 = svset4 (z4, 2, z0))

/*
** set4_f16_z24_3:
**	mov	z24\.d, z4\.d
**	mov	z25\.d, z5\.d
**	mov	z26\.d, z6\.d
**	mov	z27\.d, z0\.d
**	ret
*/
TEST_SET (set4_f16_z24_3, svfloat16x4_t, svfloat16_t,
	  z24 = svset4_f16 (z4, 3, z0),
	  z24 = svset4 (z4, 3, z0))

/*
** set4_f16_z4_0:
**	mov	z4\.d, z0\.d
**	ret
*/
TEST_SET (set4_f16_z4_0, svfloat16x4_t, svfloat16_t,
	  z4 = svset4_f16 (z4, 0, z0),
	  z4 = svset4 (z4, 0, z0))

/*
** set4_f16_z4_1:
**	mov	z5\.d, z0\.d
**	ret
*/
TEST_SET (set4_f16_z4_1, svfloat16x4_t, svfloat16_t,
	  z4 = svset4_f16 (z4, 1, z0),
	  z4 = svset4 (z4, 1, z0))

/*
** set4_f16_z4_2:
**	mov	z6\.d, z0\.d
**	ret
*/
TEST_SET (set4_f16_z4_2, svfloat16x4_t, svfloat16_t,
	  z4 = svset4_f16 (z4, 2, z0),
	  z4 = svset4 (z4, 2, z0))

/*
** set4_f16_z4_3:
**	mov	z7\.d, z0\.d
**	ret
*/
TEST_SET (set4_f16_z4_3, svfloat16x4_t, svfloat16_t,
	  z4 = svset4_f16 (z4, 3, z0),
	  z4 = svset4 (z4, 3, z0))
