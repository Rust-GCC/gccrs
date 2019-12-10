/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" } } */

#include "test_sve_acle.h"

/*
** ext_0_s16_tied1:
**	ext	z0\.b, z0\.b, z1\.b, #0
**	ret
*/
TEST_UNIFORM_Z (ext_0_s16_tied1, svint16_t,
		z0 = svext_s16 (z0, z1, 0),
		z0 = svext (z0, z1, 0))

/*
** ext_0_s16_tied2:
**	mov	(z[0-9]+)\.d, z0\.d
**	movprfx	z0, z1
**	ext	z0\.b, z0\.b, \1\.b, #0
**	ret
*/
TEST_UNIFORM_Z (ext_0_s16_tied2, svint16_t,
		z0 = svext_s16 (z1, z0, 0),
		z0 = svext (z1, z0, 0))

/*
** ext_0_s16_untied:
**	movprfx	z0, z1
**	ext	z0\.b, z0\.b, z2\.b, #0
**	ret
*/
TEST_UNIFORM_Z (ext_0_s16_untied, svint16_t,
		z0 = svext_s16 (z1, z2, 0),
		z0 = svext (z1, z2, 0))

/*
** ext_1_s16:
**	movprfx	z0, z1
**	ext	z0\.b, z0\.b, z2\.b, #2
**	ret
*/
TEST_UNIFORM_Z (ext_1_s16, svint16_t,
		z0 = svext_s16 (z1, z2, 1),
		z0 = svext (z1, z2, 1))

/*
** ext_2_s16:
**	movprfx	z0, z1
**	ext	z0\.b, z0\.b, z2\.b, #4
**	ret
*/
TEST_UNIFORM_Z (ext_2_s16, svint16_t,
		z0 = svext_s16 (z1, z2, 2),
		z0 = svext (z1, z2, 2))

/*
** ext_3_s16:
**	movprfx	z0, z1
**	ext	z0\.b, z0\.b, z2\.b, #6
**	ret
*/
TEST_UNIFORM_Z (ext_3_s16, svint16_t,
		z0 = svext_s16 (z1, z2, 3),
		z0 = svext (z1, z2, 3))

/*
** ext_127_s16:
**	movprfx	z0, z1
**	ext	z0\.b, z0\.b, z2\.b, #254
**	ret
*/
TEST_UNIFORM_Z (ext_127_s16, svint16_t,
		z0 = svext_s16 (z1, z2, 127),
		z0 = svext (z1, z2, 127))
