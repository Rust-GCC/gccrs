/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" } } */

#include "test_sve_acle.h"

/*
** splice_s8_tied1:
**	splice	z0\.b, p0, z0\.b, z1\.b
**	ret
*/
TEST_UNIFORM_Z (splice_s8_tied1, svint8_t,
		z0 = svsplice_s8 (p0, z0, z1),
		z0 = svsplice (p0, z0, z1))

/*
** splice_s8_tied2:
**	mov	(z[0-9]+)\.d, z0\.d
**	movprfx	z0, z1
**	splice	z0\.b, p0, z0\.b, \1\.b
**	ret
*/
TEST_UNIFORM_Z (splice_s8_tied2, svint8_t,
		z0 = svsplice_s8 (p0, z1, z0),
		z0 = svsplice (p0, z1, z0))

/*
** splice_s8_untied:
**	movprfx	z0, z1
**	splice	z0\.b, p0, z0\.b, z2\.b
**	ret
*/
TEST_UNIFORM_Z (splice_s8_untied, svint8_t,
		z0 = svsplice_s8 (p0, z1, z2),
		z0 = svsplice (p0, z1, z2))
