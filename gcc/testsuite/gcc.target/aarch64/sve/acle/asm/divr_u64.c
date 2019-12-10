/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" } } */

#include "test_sve_acle.h"

/*
** divr_u64_m_tied1:
**	udivr	z0\.d, p0/m, z0\.d, z1\.d
**	ret
*/
TEST_UNIFORM_Z (divr_u64_m_tied1, svuint64_t,
		z0 = svdivr_u64_m (p0, z0, z1),
		z0 = svdivr_m (p0, z0, z1))

/*
** divr_u64_m_tied2:
**	mov	(z[0-9]+\.d), z0\.d
**	movprfx	z0, z1
**	udivr	z0\.d, p0/m, z0\.d, \1
**	ret
*/
TEST_UNIFORM_Z (divr_u64_m_tied2, svuint64_t,
		z0 = svdivr_u64_m (p0, z1, z0),
		z0 = svdivr_m (p0, z1, z0))

/*
** divr_u64_m_untied:
**	movprfx	z0, z1
**	udivr	z0\.d, p0/m, z0\.d, z2\.d
**	ret
*/
TEST_UNIFORM_Z (divr_u64_m_untied, svuint64_t,
		z0 = svdivr_u64_m (p0, z1, z2),
		z0 = svdivr_m (p0, z1, z2))

/*
** divr_x0_u64_m_tied1:
**	mov	(z[0-9]+\.d), x0
**	udivr	z0\.d, p0/m, z0\.d, \1
**	ret
*/
TEST_UNIFORM_ZX (divr_x0_u64_m_tied1, svuint64_t, uint64_t,
		 z0 = svdivr_n_u64_m (p0, z0, x0),
		 z0 = svdivr_m (p0, z0, x0))

/*
** divr_x0_u64_m_untied:
**	mov	(z[0-9]+\.d), x0
**	movprfx	z0, z1
**	udivr	z0\.d, p0/m, z0\.d, \1
**	ret
*/
TEST_UNIFORM_ZX (divr_x0_u64_m_untied, svuint64_t, uint64_t,
		 z0 = svdivr_n_u64_m (p0, z1, x0),
		 z0 = svdivr_m (p0, z1, x0))

/*
** divr_2_u64_m_tied1:
**	mov	(z[0-9]+\.d), #2
**	udivr	z0\.d, p0/m, z0\.d, \1
**	ret
*/
TEST_UNIFORM_Z (divr_2_u64_m_tied1, svuint64_t,
		z0 = svdivr_n_u64_m (p0, z0, 2),
		z0 = svdivr_m (p0, z0, 2))

/*
** divr_2_u64_m_untied: { xfail *-*-* }
**	mov	(z[0-9]+\.d), #2
**	movprfx	z0, z1
**	udivr	z0\.d, p0/m, z0\.d, \1
**	ret
*/
TEST_UNIFORM_Z (divr_2_u64_m_untied, svuint64_t,
		z0 = svdivr_n_u64_m (p0, z1, 2),
		z0 = svdivr_m (p0, z1, 2))

/*
** divr_m1_u64_m:
**	mov	(z[0-9]+)\.b, #-1
**	udivr	z0\.d, p0/m, z0\.d, \1\.d
**	ret
*/
TEST_UNIFORM_Z (divr_m1_u64_m, svuint64_t,
		z0 = svdivr_n_u64_m (p0, z0, -1),
		z0 = svdivr_m (p0, z0, -1))

/*
** divr_u64_z_tied1:
**	movprfx	z0\.d, p0/z, z0\.d
**	udivr	z0\.d, p0/m, z0\.d, z1\.d
**	ret
*/
TEST_UNIFORM_Z (divr_u64_z_tied1, svuint64_t,
		z0 = svdivr_u64_z (p0, z0, z1),
		z0 = svdivr_z (p0, z0, z1))

/*
** divr_u64_z_tied2:
**	movprfx	z0\.d, p0/z, z0\.d
**	udiv	z0\.d, p0/m, z0\.d, z1\.d
**	ret
*/
TEST_UNIFORM_Z (divr_u64_z_tied2, svuint64_t,
		z0 = svdivr_u64_z (p0, z1, z0),
		z0 = svdivr_z (p0, z1, z0))

/*
** divr_u64_z_untied:
** (
**	movprfx	z0\.d, p0/z, z1\.d
**	udivr	z0\.d, p0/m, z0\.d, z2\.d
** |
**	movprfx	z0\.d, p0/z, z2\.d
**	udiv	z0\.d, p0/m, z0\.d, z1\.d
** )
**	ret
*/
TEST_UNIFORM_Z (divr_u64_z_untied, svuint64_t,
		z0 = svdivr_u64_z (p0, z1, z2),
		z0 = svdivr_z (p0, z1, z2))

/*
** divr_x0_u64_z_tied1:
**	mov	(z[0-9]+\.d), x0
**	movprfx	z0\.d, p0/z, z0\.d
**	udivr	z0\.d, p0/m, z0\.d, \1
**	ret
*/
TEST_UNIFORM_ZX (divr_x0_u64_z_tied1, svuint64_t, uint64_t,
		 z0 = svdivr_n_u64_z (p0, z0, x0),
		 z0 = svdivr_z (p0, z0, x0))

/*
** divr_x0_u64_z_untied:
**	mov	(z[0-9]+\.d), x0
** (
**	movprfx	z0\.d, p0/z, z1\.d
**	udivr	z0\.d, p0/m, z0\.d, \1
** |
**	movprfx	z0\.d, p0/z, \1
**	udiv	z0\.d, p0/m, z0\.d, z1\.d
** )
**	ret
*/
TEST_UNIFORM_ZX (divr_x0_u64_z_untied, svuint64_t, uint64_t,
		 z0 = svdivr_n_u64_z (p0, z1, x0),
		 z0 = svdivr_z (p0, z1, x0))

/*
** divr_2_u64_z_tied1:
**	mov	(z[0-9]+\.d), #2
**	movprfx	z0\.d, p0/z, z0\.d
**	udivr	z0\.d, p0/m, z0\.d, \1
**	ret
*/
TEST_UNIFORM_Z (divr_2_u64_z_tied1, svuint64_t,
		z0 = svdivr_n_u64_z (p0, z0, 2),
		z0 = svdivr_z (p0, z0, 2))

/*
** divr_2_u64_z_untied:
**	mov	(z[0-9]+\.d), #2
** (
**	movprfx	z0\.d, p0/z, z1\.d
**	udivr	z0\.d, p0/m, z0\.d, \1
** |
**	movprfx	z0\.d, p0/z, \1
**	udiv	z0\.d, p0/m, z0\.d, z1\.d
** )
**	ret
*/
TEST_UNIFORM_Z (divr_2_u64_z_untied, svuint64_t,
		z0 = svdivr_n_u64_z (p0, z1, 2),
		z0 = svdivr_z (p0, z1, 2))

/*
** divr_u64_x_tied1:
**	udivr	z0\.d, p0/m, z0\.d, z1\.d
**	ret
*/
TEST_UNIFORM_Z (divr_u64_x_tied1, svuint64_t,
		z0 = svdivr_u64_x (p0, z0, z1),
		z0 = svdivr_x (p0, z0, z1))

/*
** divr_u64_x_tied2:
**	udiv	z0\.d, p0/m, z0\.d, z1\.d
**	ret
*/
TEST_UNIFORM_Z (divr_u64_x_tied2, svuint64_t,
		z0 = svdivr_u64_x (p0, z1, z0),
		z0 = svdivr_x (p0, z1, z0))

/*
** divr_u64_x_untied:
** (
**	movprfx	z0, z1
**	udivr	z0\.d, p0/m, z0\.d, z2\.d
** |
**	movprfx	z0, z2
**	udiv	z0\.d, p0/m, z0\.d, z1\.d
** )
**	ret
*/
TEST_UNIFORM_Z (divr_u64_x_untied, svuint64_t,
		z0 = svdivr_u64_x (p0, z1, z2),
		z0 = svdivr_x (p0, z1, z2))

/*
** divr_x0_u64_x_tied1:
**	mov	(z[0-9]+\.d), x0
**	udivr	z0\.d, p0/m, z0\.d, \1
**	ret
*/
TEST_UNIFORM_ZX (divr_x0_u64_x_tied1, svuint64_t, uint64_t,
		 z0 = svdivr_n_u64_x (p0, z0, x0),
		 z0 = svdivr_x (p0, z0, x0))

/*
** divr_x0_u64_x_untied:
**	mov	z0\.d, x0
**	udiv	z0\.d, p0/m, z0\.d, z1\.d
**	ret
*/
TEST_UNIFORM_ZX (divr_x0_u64_x_untied, svuint64_t, uint64_t,
		 z0 = svdivr_n_u64_x (p0, z1, x0),
		 z0 = svdivr_x (p0, z1, x0))

/*
** divr_2_u64_x_tied1:
**	mov	(z[0-9]+\.d), #2
**	udivr	z0\.d, p0/m, z0\.d, \1
**	ret
*/
TEST_UNIFORM_Z (divr_2_u64_x_tied1, svuint64_t,
		z0 = svdivr_n_u64_x (p0, z0, 2),
		z0 = svdivr_x (p0, z0, 2))

/*
** divr_2_u64_x_untied:
**	mov	z0\.d, #2
**	udiv	z0\.d, p0/m, z0\.d, z1\.d
**	ret
*/
TEST_UNIFORM_Z (divr_2_u64_x_untied, svuint64_t,
		z0 = svdivr_n_u64_x (p0, z1, 2),
		z0 = svdivr_x (p0, z1, 2))
