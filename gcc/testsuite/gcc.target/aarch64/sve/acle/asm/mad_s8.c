/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" } } */

#include "test_sve_acle.h"

/*
** mad_s8_m_tied1:
**	mad	z0\.b, p0/m, z1\.b, z2\.b
**	ret
*/
TEST_UNIFORM_Z (mad_s8_m_tied1, svint8_t,
		z0 = svmad_s8_m (p0, z0, z1, z2),
		z0 = svmad_m (p0, z0, z1, z2))

/*
** mad_s8_m_tied2:
**	mov	(z[0-9]+)\.d, z0\.d
**	movprfx	z0, z1
**	mad	z0\.b, p0/m, \1\.b, z2\.b
**	ret
*/
TEST_UNIFORM_Z (mad_s8_m_tied2, svint8_t,
		z0 = svmad_s8_m (p0, z1, z0, z2),
		z0 = svmad_m (p0, z1, z0, z2))

/*
** mad_s8_m_tied3:
**	mov	(z[0-9]+)\.d, z0\.d
**	movprfx	z0, z1
**	mad	z0\.b, p0/m, z2\.b, \1\.b
**	ret
*/
TEST_UNIFORM_Z (mad_s8_m_tied3, svint8_t,
		z0 = svmad_s8_m (p0, z1, z2, z0),
		z0 = svmad_m (p0, z1, z2, z0))

/*
** mad_s8_m_untied:
**	movprfx	z0, z1
**	mad	z0\.b, p0/m, z2\.b, z3\.b
**	ret
*/
TEST_UNIFORM_Z (mad_s8_m_untied, svint8_t,
		z0 = svmad_s8_m (p0, z1, z2, z3),
		z0 = svmad_m (p0, z1, z2, z3))

/*
** mad_w0_s8_m_tied1:
**	mov	(z[0-9]+\.b), w0
**	mad	z0\.b, p0/m, z1\.b, \1
**	ret
*/
TEST_UNIFORM_ZX (mad_w0_s8_m_tied1, svint8_t, int8_t,
		 z0 = svmad_n_s8_m (p0, z0, z1, x0),
		 z0 = svmad_m (p0, z0, z1, x0))

/*
** mad_w0_s8_m_untied: { xfail *-*-* }
**	mov	(z[0-9]+\.b), w0
**	movprfx	z0, z1
**	mad	z0\.b, p0/m, z2\.b, \1
**	ret
*/
TEST_UNIFORM_ZX (mad_w0_s8_m_untied, svint8_t, int8_t,
		 z0 = svmad_n_s8_m (p0, z1, z2, x0),
		 z0 = svmad_m (p0, z1, z2, x0))

/*
** mad_11_s8_m_tied1:
**	mov	(z[0-9]+\.b), #11
**	mad	z0\.b, p0/m, z1\.b, \1
**	ret
*/
TEST_UNIFORM_Z (mad_11_s8_m_tied1, svint8_t,
		z0 = svmad_n_s8_m (p0, z0, z1, 11),
		z0 = svmad_m (p0, z0, z1, 11))

/*
** mad_11_s8_m_untied: { xfail *-*-* }
**	mov	(z[0-9]+\.b), #11
**	movprfx	z0, z1
**	mad	z0\.b, p0/m, z2\.b, \1
**	ret
*/
TEST_UNIFORM_Z (mad_11_s8_m_untied, svint8_t,
		z0 = svmad_n_s8_m (p0, z1, z2, 11),
		z0 = svmad_m (p0, z1, z2, 11))

/*
** mad_s8_z_tied1:
**	movprfx	z0\.b, p0/z, z0\.b
**	mad	z0\.b, p0/m, z1\.b, z2\.b
**	ret
*/
TEST_UNIFORM_Z (mad_s8_z_tied1, svint8_t,
		z0 = svmad_s8_z (p0, z0, z1, z2),
		z0 = svmad_z (p0, z0, z1, z2))

/*
** mad_s8_z_tied2:
**	movprfx	z0\.b, p0/z, z0\.b
**	mad	z0\.b, p0/m, z1\.b, z2\.b
**	ret
*/
TEST_UNIFORM_Z (mad_s8_z_tied2, svint8_t,
		z0 = svmad_s8_z (p0, z1, z0, z2),
		z0 = svmad_z (p0, z1, z0, z2))

/*
** mad_s8_z_tied3:
**	movprfx	z0\.b, p0/z, z0\.b
**	mla	z0\.b, p0/m, z1\.b, z2\.b
**	ret
*/
TEST_UNIFORM_Z (mad_s8_z_tied3, svint8_t,
		z0 = svmad_s8_z (p0, z1, z2, z0),
		z0 = svmad_z (p0, z1, z2, z0))

/*
** mad_s8_z_untied:
** (
**	movprfx	z0\.b, p0/z, z1\.b
**	mad	z0\.b, p0/m, z2\.b, z3\.b
** |
**	movprfx	z0\.b, p0/z, z2\.b
**	mad	z0\.b, p0/m, z1\.b, z3\.b
** |
**	movprfx	z0\.b, p0/z, z3\.b
**	mla	z0\.b, p0/m, z1\.b, z2\.b
** )
**	ret
*/
TEST_UNIFORM_Z (mad_s8_z_untied, svint8_t,
		z0 = svmad_s8_z (p0, z1, z2, z3),
		z0 = svmad_z (p0, z1, z2, z3))

/*
** mad_w0_s8_z_tied1:
**	mov	(z[0-9]+\.b), w0
**	movprfx	z0\.b, p0/z, z0\.b
**	mad	z0\.b, p0/m, z1\.b, \1
**	ret
*/
TEST_UNIFORM_ZX (mad_w0_s8_z_tied1, svint8_t, int8_t,
		 z0 = svmad_n_s8_z (p0, z0, z1, x0),
		 z0 = svmad_z (p0, z0, z1, x0))

/*
** mad_w0_s8_z_tied2:
**	mov	(z[0-9]+\.b), w0
**	movprfx	z0\.b, p0/z, z0\.b
**	mad	z0\.b, p0/m, z1\.b, \1
**	ret
*/
TEST_UNIFORM_ZX (mad_w0_s8_z_tied2, svint8_t, int8_t,
		 z0 = svmad_n_s8_z (p0, z1, z0, x0),
		 z0 = svmad_z (p0, z1, z0, x0))

/*
** mad_w0_s8_z_untied:
**	mov	(z[0-9]+\.b), w0
** (
**	movprfx	z0\.b, p0/z, z1\.b
**	mad	z0\.b, p0/m, z2\.b, \1
** |
**	movprfx	z0\.b, p0/z, z2\.b
**	mad	z0\.b, p0/m, z1\.b, \1
** |
**	movprfx	z0\.b, p0/z, \1
**	mla	z0\.b, p0/m, z1\.b, z2\.b
** )
**	ret
*/
TEST_UNIFORM_ZX (mad_w0_s8_z_untied, svint8_t, int8_t,
		 z0 = svmad_n_s8_z (p0, z1, z2, x0),
		 z0 = svmad_z (p0, z1, z2, x0))

/*
** mad_11_s8_z_tied1:
**	mov	(z[0-9]+\.b), #11
**	movprfx	z0\.b, p0/z, z0\.b
**	mad	z0\.b, p0/m, z1\.b, \1
**	ret
*/
TEST_UNIFORM_Z (mad_11_s8_z_tied1, svint8_t,
		z0 = svmad_n_s8_z (p0, z0, z1, 11),
		z0 = svmad_z (p0, z0, z1, 11))

/*
** mad_11_s8_z_tied2:
**	mov	(z[0-9]+\.b), #11
**	movprfx	z0\.b, p0/z, z0\.b
**	mad	z0\.b, p0/m, z1\.b, \1
**	ret
*/
TEST_UNIFORM_Z (mad_11_s8_z_tied2, svint8_t,
		z0 = svmad_n_s8_z (p0, z1, z0, 11),
		z0 = svmad_z (p0, z1, z0, 11))

/*
** mad_11_s8_z_untied:
**	mov	(z[0-9]+\.b), #11
** (
**	movprfx	z0\.b, p0/z, z1\.b
**	mad	z0\.b, p0/m, z2\.b, \1
** |
**	movprfx	z0\.b, p0/z, z2\.b
**	mad	z0\.b, p0/m, z1\.b, \1
** |
**	movprfx	z0\.b, p0/z, \1
**	mla	z0\.b, p0/m, z1\.b, z2\.b
** )
**	ret
*/
TEST_UNIFORM_Z (mad_11_s8_z_untied, svint8_t,
		z0 = svmad_n_s8_z (p0, z1, z2, 11),
		z0 = svmad_z (p0, z1, z2, 11))

/*
** mad_s8_x_tied1:
**	mad	z0\.b, p0/m, z1\.b, z2\.b
**	ret
*/
TEST_UNIFORM_Z (mad_s8_x_tied1, svint8_t,
		z0 = svmad_s8_x (p0, z0, z1, z2),
		z0 = svmad_x (p0, z0, z1, z2))

/*
** mad_s8_x_tied2:
**	mad	z0\.b, p0/m, z1\.b, z2\.b
**	ret
*/
TEST_UNIFORM_Z (mad_s8_x_tied2, svint8_t,
		z0 = svmad_s8_x (p0, z1, z0, z2),
		z0 = svmad_x (p0, z1, z0, z2))

/*
** mad_s8_x_tied3:
**	mla	z0\.b, p0/m, z1\.b, z2\.b
**	ret
*/
TEST_UNIFORM_Z (mad_s8_x_tied3, svint8_t,
		z0 = svmad_s8_x (p0, z1, z2, z0),
		z0 = svmad_x (p0, z1, z2, z0))

/*
** mad_s8_x_untied:
** (
**	movprfx	z0, z1
**	mad	z0\.b, p0/m, z2\.b, z3\.b
** |
**	movprfx	z0, z2
**	mad	z0\.b, p0/m, z1\.b, z3\.b
** |
**	movprfx	z0, z3
**	mla	z0\.b, p0/m, z1\.b, z2\.b
** )
**	ret
*/
TEST_UNIFORM_Z (mad_s8_x_untied, svint8_t,
		z0 = svmad_s8_x (p0, z1, z2, z3),
		z0 = svmad_x (p0, z1, z2, z3))

/*
** mad_w0_s8_x_tied1:
**	mov	(z[0-9]+\.b), w0
**	mad	z0\.b, p0/m, z1\.b, \1
**	ret
*/
TEST_UNIFORM_ZX (mad_w0_s8_x_tied1, svint8_t, int8_t,
		 z0 = svmad_n_s8_x (p0, z0, z1, x0),
		 z0 = svmad_x (p0, z0, z1, x0))

/*
** mad_w0_s8_x_tied2:
**	mov	(z[0-9]+\.b), w0
**	mad	z0\.b, p0/m, z1\.b, \1
**	ret
*/
TEST_UNIFORM_ZX (mad_w0_s8_x_tied2, svint8_t, int8_t,
		 z0 = svmad_n_s8_x (p0, z1, z0, x0),
		 z0 = svmad_x (p0, z1, z0, x0))

/*
** mad_w0_s8_x_untied:
**	mov	z0\.b, w0
**	mla	z0\.b, p0/m, z1\.b, z2\.b
**	ret
*/
TEST_UNIFORM_ZX (mad_w0_s8_x_untied, svint8_t, int8_t,
		 z0 = svmad_n_s8_x (p0, z1, z2, x0),
		 z0 = svmad_x (p0, z1, z2, x0))

/*
** mad_11_s8_x_tied1:
**	mov	(z[0-9]+\.b), #11
**	mad	z0\.b, p0/m, z1\.b, \1
**	ret
*/
TEST_UNIFORM_Z (mad_11_s8_x_tied1, svint8_t,
		z0 = svmad_n_s8_x (p0, z0, z1, 11),
		z0 = svmad_x (p0, z0, z1, 11))

/*
** mad_11_s8_x_tied2:
**	mov	(z[0-9]+\.b), #11
**	mad	z0\.b, p0/m, z1\.b, \1
**	ret
*/
TEST_UNIFORM_Z (mad_11_s8_x_tied2, svint8_t,
		z0 = svmad_n_s8_x (p0, z1, z0, 11),
		z0 = svmad_x (p0, z1, z0, 11))

/*
** mad_11_s8_x_untied:
**	mov	z0\.b, #11
**	mla	z0\.b, p0/m, z1\.b, z2\.b
**	ret
*/
TEST_UNIFORM_Z (mad_11_s8_x_untied, svint8_t,
		z0 = svmad_n_s8_x (p0, z1, z2, 11),
		z0 = svmad_x (p0, z1, z2, 11))
