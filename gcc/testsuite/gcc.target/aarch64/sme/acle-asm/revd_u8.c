/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" } } */

#include "test_sme_acle.h"

/*
** revd_u8_m_tied12:
**	revd	z0\.q, p0/m, z0\.q
**	ret
*/
TEST_UNIFORM_Z (revd_u8_m_tied12, svuint8_t,
		z0 = svrevd_u8_m (z0, p0, z0),
		z0 = svrevd_m (z0, p0, z0))

/*
** revd_u8_m_tied1:
**	revd	z0\.q, p0/m, z1\.q
**	ret
*/
TEST_UNIFORM_Z (revd_u8_m_tied1, svuint8_t,
		z0 = svrevd_u8_m (z0, p0, z1),
		z0 = svrevd_m (z0, p0, z1))

/*
** revd_u8_m_tied2:
**	mov	(z[0-9]+)\.d, z0\.d
**	movprfx	z0, z1
**	revd	z0\.q, p0/m, \1\.q
**	ret
*/
TEST_UNIFORM_Z (revd_u8_m_tied2, svuint8_t,
		z0 = svrevd_u8_m (z1, p0, z0),
		z0 = svrevd_m (z1, p0, z0))

/*
** revd_u8_m_untied:
**	movprfx	z0, z2
**	revd	z0\.q, p0/m, z1\.q
**	ret
*/
TEST_UNIFORM_Z (revd_u8_m_untied, svuint8_t,
		z0 = svrevd_u8_m (z2, p0, z1),
		z0 = svrevd_m (z2, p0, z1))

/* Awkward register allocation.  Don't require specific output.  */
TEST_UNIFORM_Z (revd_u8_z_tied1, svuint8_t,
		z0 = svrevd_u8_z (p0, z0),
		z0 = svrevd_z (p0, z0))

/*
** revd_u8_z_untied:
**	movi?	[vdz]0\.?(?:[0-9]*[bhsd])?, #?0
**	revd	z0\.q, p0/m, z1\.q
**	ret
*/
TEST_UNIFORM_Z (revd_u8_z_untied, svuint8_t,
		z0 = svrevd_u8_z (p0, z1),
		z0 = svrevd_z (p0, z1))

/*
** revd_u8_x_tied1:
**	revd	z0\.q, p0/m, z0\.q
**	ret
*/
TEST_UNIFORM_Z (revd_u8_x_tied1, svuint8_t,
		z0 = svrevd_u8_x (p0, z0),
		z0 = svrevd_x (p0, z0))

/*
** revd_u8_x_untied:
**	movprfx	z0, z1
**	revd	z0\.q, p0/m, z1\.q
**	ret
*/
TEST_UNIFORM_Z (revd_u8_x_untied, svuint8_t,
		z0 = svrevd_u8_x (p0, z1),
		z0 = svrevd_x (p0, z1))
