/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" } } */

#include "test_sve_acle.h"

/*
** mls_s32_m_tied1:
**	mls	z0\.s, p0/m, z1\.s, z2\.s
**	ret
*/
TEST_UNIFORM_Z (mls_s32_m_tied1, svint32_t,
		z0 = svmls_s32_m (p0, z0, z1, z2),
		z0 = svmls_m (p0, z0, z1, z2))

/*
** mls_s32_m_tied2:
**	mov	(z[0-9]+)\.d, z0\.d
**	movprfx	z0, z1
**	mls	z0\.s, p0/m, \1\.s, z2\.s
**	ret
*/
TEST_UNIFORM_Z (mls_s32_m_tied2, svint32_t,
		z0 = svmls_s32_m (p0, z1, z0, z2),
		z0 = svmls_m (p0, z1, z0, z2))

/*
** mls_s32_m_tied3:
**	mov	(z[0-9]+)\.d, z0\.d
**	movprfx	z0, z1
**	mls	z0\.s, p0/m, z2\.s, \1\.s
**	ret
*/
TEST_UNIFORM_Z (mls_s32_m_tied3, svint32_t,
		z0 = svmls_s32_m (p0, z1, z2, z0),
		z0 = svmls_m (p0, z1, z2, z0))

/*
** mls_s32_m_untied:
**	movprfx	z0, z1
**	mls	z0\.s, p0/m, z2\.s, z3\.s
**	ret
*/
TEST_UNIFORM_Z (mls_s32_m_untied, svint32_t,
		z0 = svmls_s32_m (p0, z1, z2, z3),
		z0 = svmls_m (p0, z1, z2, z3))

/*
** mls_w0_s32_m_tied1:
**	mov	(z[0-9]+\.s), w0
**	mls	z0\.s, p0/m, z1\.s, \1
**	ret
*/
TEST_UNIFORM_ZX (mls_w0_s32_m_tied1, svint32_t, int32_t,
		 z0 = svmls_n_s32_m (p0, z0, z1, x0),
		 z0 = svmls_m (p0, z0, z1, x0))

/*
** mls_w0_s32_m_untied:
**	mov	(z[0-9]+\.s), w0
**	movprfx	z0, z1
**	mls	z0\.s, p0/m, z2\.s, \1
**	ret
*/
TEST_UNIFORM_ZX (mls_w0_s32_m_untied, svint32_t, int32_t,
		 z0 = svmls_n_s32_m (p0, z1, z2, x0),
		 z0 = svmls_m (p0, z1, z2, x0))

/*
** mls_11_s32_m_tied1:
**	mov	(z[0-9]+\.s), #11
**	mls	z0\.s, p0/m, z1\.s, \1
**	ret
*/
TEST_UNIFORM_Z (mls_11_s32_m_tied1, svint32_t,
		z0 = svmls_n_s32_m (p0, z0, z1, 11),
		z0 = svmls_m (p0, z0, z1, 11))

/*
** mls_11_s32_m_untied: { xfail *-*-* }
**	mov	(z[0-9]+\.s), #11
**	movprfx	z0, z1
**	mls	z0\.s, p0/m, z2\.s, \1
**	ret
*/
TEST_UNIFORM_Z (mls_11_s32_m_untied, svint32_t,
		z0 = svmls_n_s32_m (p0, z1, z2, 11),
		z0 = svmls_m (p0, z1, z2, 11))

/*
** mls_s32_z_tied1:
**	movprfx	z0\.s, p0/z, z0\.s
**	mls	z0\.s, p0/m, z1\.s, z2\.s
**	ret
*/
TEST_UNIFORM_Z (mls_s32_z_tied1, svint32_t,
		z0 = svmls_s32_z (p0, z0, z1, z2),
		z0 = svmls_z (p0, z0, z1, z2))

/*
** mls_s32_z_tied2:
**	movprfx	z0\.s, p0/z, z0\.s
**	msb	z0\.s, p0/m, z2\.s, z1\.s
**	ret
*/
TEST_UNIFORM_Z (mls_s32_z_tied2, svint32_t,
		z0 = svmls_s32_z (p0, z1, z0, z2),
		z0 = svmls_z (p0, z1, z0, z2))

/*
** mls_s32_z_tied3:
**	movprfx	z0\.s, p0/z, z0\.s
**	msb	z0\.s, p0/m, z2\.s, z1\.s
**	ret
*/
TEST_UNIFORM_Z (mls_s32_z_tied3, svint32_t,
		z0 = svmls_s32_z (p0, z1, z2, z0),
		z0 = svmls_z (p0, z1, z2, z0))

/*
** mls_s32_z_untied:
** (
**	movprfx	z0\.s, p0/z, z1\.s
**	mls	z0\.s, p0/m, z2\.s, z3\.s
** |
**	movprfx	z0\.s, p0/z, z2\.s
**	msb	z0\.s, p0/m, z3\.s, z1\.s
** |
**	movprfx	z0\.s, p0/z, z3\.s
**	msb	z0\.s, p0/m, z2\.s, z1\.s
** )
**	ret
*/
TEST_UNIFORM_Z (mls_s32_z_untied, svint32_t,
		z0 = svmls_s32_z (p0, z1, z2, z3),
		z0 = svmls_z (p0, z1, z2, z3))

/*
** mls_w0_s32_z_tied1:
**	mov	(z[0-9]+\.s), w0
**	movprfx	z0\.s, p0/z, z0\.s
**	mls	z0\.s, p0/m, z1\.s, \1
**	ret
*/
TEST_UNIFORM_ZX (mls_w0_s32_z_tied1, svint32_t, int32_t,
		 z0 = svmls_n_s32_z (p0, z0, z1, x0),
		 z0 = svmls_z (p0, z0, z1, x0))

/*
** mls_w0_s32_z_tied2:
**	mov	(z[0-9]+\.s), w0
**	movprfx	z0\.s, p0/z, z0\.s
**	msb	z0\.s, p0/m, \1, z1\.s
**	ret
*/
TEST_UNIFORM_ZX (mls_w0_s32_z_tied2, svint32_t, int32_t,
		 z0 = svmls_n_s32_z (p0, z1, z0, x0),
		 z0 = svmls_z (p0, z1, z0, x0))

/*
** mls_w0_s32_z_untied:
**	mov	(z[0-9]+\.s), w0
** (
**	movprfx	z0\.s, p0/z, z1\.s
**	mls	z0\.s, p0/m, z2\.s, \1
** |
**	movprfx	z0\.s, p0/z, z2\.s
**	msb	z0\.s, p0/m, \1, z1\.s
** |
**	movprfx	z0\.s, p0/z, \1
**	msb	z0\.s, p0/m, z2\.s, z1\.s
** )
**	ret
*/
TEST_UNIFORM_ZX (mls_w0_s32_z_untied, svint32_t, int32_t,
		 z0 = svmls_n_s32_z (p0, z1, z2, x0),
		 z0 = svmls_z (p0, z1, z2, x0))

/*
** mls_11_s32_z_tied1:
**	mov	(z[0-9]+\.s), #11
**	movprfx	z0\.s, p0/z, z0\.s
**	mls	z0\.s, p0/m, z1\.s, \1
**	ret
*/
TEST_UNIFORM_Z (mls_11_s32_z_tied1, svint32_t,
		z0 = svmls_n_s32_z (p0, z0, z1, 11),
		z0 = svmls_z (p0, z0, z1, 11))

/*
** mls_11_s32_z_tied2:
**	mov	(z[0-9]+\.s), #11
**	movprfx	z0\.s, p0/z, z0\.s
**	msb	z0\.s, p0/m, \1, z1\.s
**	ret
*/
TEST_UNIFORM_Z (mls_11_s32_z_tied2, svint32_t,
		z0 = svmls_n_s32_z (p0, z1, z0, 11),
		z0 = svmls_z (p0, z1, z0, 11))

/*
** mls_11_s32_z_untied:
**	mov	(z[0-9]+\.s), #11
** (
**	movprfx	z0\.s, p0/z, z1\.s
**	mls	z0\.s, p0/m, z2\.s, \1
** |
**	movprfx	z0\.s, p0/z, z2\.s
**	msb	z0\.s, p0/m, \1, z1\.s
** |
**	movprfx	z0\.s, p0/z, \1
**	msb	z0\.s, p0/m, z2\.s, z1\.s
** )
**	ret
*/
TEST_UNIFORM_Z (mls_11_s32_z_untied, svint32_t,
		z0 = svmls_n_s32_z (p0, z1, z2, 11),
		z0 = svmls_z (p0, z1, z2, 11))

/*
** mls_s32_x_tied1:
**	mls	z0\.s, p0/m, z1\.s, z2\.s
**	ret
*/
TEST_UNIFORM_Z (mls_s32_x_tied1, svint32_t,
		z0 = svmls_s32_x (p0, z0, z1, z2),
		z0 = svmls_x (p0, z0, z1, z2))

/*
** mls_s32_x_tied2:
**	msb	z0\.s, p0/m, z2\.s, z1\.s
**	ret
*/
TEST_UNIFORM_Z (mls_s32_x_tied2, svint32_t,
		z0 = svmls_s32_x (p0, z1, z0, z2),
		z0 = svmls_x (p0, z1, z0, z2))

/*
** mls_s32_x_tied3:
**	msb	z0\.s, p0/m, z2\.s, z1\.s
**	ret
*/
TEST_UNIFORM_Z (mls_s32_x_tied3, svint32_t,
		z0 = svmls_s32_x (p0, z1, z2, z0),
		z0 = svmls_x (p0, z1, z2, z0))

/*
** mls_s32_x_untied:
** (
**	movprfx	z0, z1
**	mls	z0\.s, p0/m, z2\.s, z3\.s
** |
**	movprfx	z0, z2
**	msb	z0\.s, p0/m, z3\.s, z1\.s
** |
**	movprfx	z0, z3
**	msb	z0\.s, p0/m, z2\.s, z1\.s
** )
**	ret
*/
TEST_UNIFORM_Z (mls_s32_x_untied, svint32_t,
		z0 = svmls_s32_x (p0, z1, z2, z3),
		z0 = svmls_x (p0, z1, z2, z3))

/*
** mls_w0_s32_x_tied1:
**	mov	(z[0-9]+\.s), w0
**	mls	z0\.s, p0/m, z1\.s, \1
**	ret
*/
TEST_UNIFORM_ZX (mls_w0_s32_x_tied1, svint32_t, int32_t,
		 z0 = svmls_n_s32_x (p0, z0, z1, x0),
		 z0 = svmls_x (p0, z0, z1, x0))

/*
** mls_w0_s32_x_tied2:
**	mov	(z[0-9]+\.s), w0
**	msb	z0\.s, p0/m, \1, z1\.s
**	ret
*/
TEST_UNIFORM_ZX (mls_w0_s32_x_tied2, svint32_t, int32_t,
		 z0 = svmls_n_s32_x (p0, z1, z0, x0),
		 z0 = svmls_x (p0, z1, z0, x0))

/*
** mls_w0_s32_x_untied:
**	mov	z0\.s, w0
**	msb	z0\.s, p0/m, z2\.s, z1\.s
**	ret
*/
TEST_UNIFORM_ZX (mls_w0_s32_x_untied, svint32_t, int32_t,
		 z0 = svmls_n_s32_x (p0, z1, z2, x0),
		 z0 = svmls_x (p0, z1, z2, x0))

/*
** mls_11_s32_x_tied1:
**	mov	(z[0-9]+\.s), #11
**	mls	z0\.s, p0/m, z1\.s, \1
**	ret
*/
TEST_UNIFORM_Z (mls_11_s32_x_tied1, svint32_t,
		z0 = svmls_n_s32_x (p0, z0, z1, 11),
		z0 = svmls_x (p0, z0, z1, 11))

/*
** mls_11_s32_x_tied2:
**	mov	(z[0-9]+\.s), #11
**	msb	z0\.s, p0/m, \1, z1\.s
**	ret
*/
TEST_UNIFORM_Z (mls_11_s32_x_tied2, svint32_t,
		z0 = svmls_n_s32_x (p0, z1, z0, 11),
		z0 = svmls_x (p0, z1, z0, 11))

/*
** mls_11_s32_x_untied:
**	mov	z0\.s, #11
**	msb	z0\.s, p0/m, z2\.s, z1\.s
**	ret
*/
TEST_UNIFORM_Z (mls_11_s32_x_untied, svint32_t,
		z0 = svmls_n_s32_x (p0, z1, z2, 11),
		z0 = svmls_x (p0, z1, z2, 11))
