/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" } } */

#include "test_sme_acle.h"

/*
** revd_s8_m_tied12:
**	revd	z0\.q, p0/m, z0\.q
**	ret
*/
TEST_UNIFORM_Z (revd_s8_m_tied12, svint8_t,
		z0 = svrevd_s8_m (z0, p0, z0),
		z0 = svrevd_m (z0, p0, z0))

/*
** revd_s8_m_tied1:
**	revd	z0\.q, p0/m, z1\.q
**	ret
*/
TEST_UNIFORM_Z (revd_s8_m_tied1, svint8_t,
		z0 = svrevd_s8_m (z0, p0, z1),
		z0 = svrevd_m (z0, p0, z1))

/*
** revd_s8_m_tied2:
**	revd	z1\.q, p0/m, z0\.q
**	mov	z0.d, z1.d
**	ret
*/
TEST_UNIFORM_Z (revd_s8_m_tied2, svint8_t,
		z0 = svrevd_s8_m (z1, p0, z0),
		z0 = svrevd_m (z1, p0, z0))

/*
** revd_s8_m_untied:
**	revd	z2\.q, p0/m, z1\.q
**	mov	z0.d, z2.d
**	ret
*/
TEST_UNIFORM_Z (revd_s8_m_untied, svint8_t,
		z0 = svrevd_s8_m (z2, p0, z1),
		z0 = svrevd_m (z2, p0, z1))

/* Awkward register allocation.  Don't require specific output.  */
TEST_UNIFORM_Z (revd_s8_z_tied1, svint8_t,
		z0 = svrevd_s8_z (p0, z0),
		z0 = svrevd_z (p0, z0))

/*
** revd_s8_z_untied:
**	movi?	[vdz]0\.?(?:[0-9]*[bhsd])?, #?0
**	revd	z0\.q, p0/m, z1\.q
**	ret
*/
TEST_UNIFORM_Z (revd_s8_z_untied, svint8_t,
		z0 = svrevd_s8_z (p0, z1),
		z0 = svrevd_z (p0, z1))

/*
** revd_s8_x_tied1:
**	revd	z0\.q, p0/m, z0\.q
**	ret
*/
TEST_UNIFORM_Z (revd_s8_x_tied1, svint8_t,
		z0 = svrevd_s8_x (p0, z0),
		z0 = svrevd_x (p0, z0))

/*
** revd_s8_x_untied:
**	revd	z1\.q, p0/m, z1\.q
**	mov	z0.d, z1.d
**	ret
*/
TEST_UNIFORM_Z (revd_s8_x_untied, svint8_t,
		z0 = svrevd_s8_x (p0, z1),
		z0 = svrevd_x (p0, z1))
