/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" } } */

#include "test_sve_acle.h"

/*
** lsl_s16_m_tied1:
**	lsl	z0\.h, p0/m, z0\.h, z4\.h
**	ret
*/
TEST_DUAL_Z (lsl_s16_m_tied1, svint16_t, svuint16_t,
	     z0 = svlsl_s16_m (p0, z0, z4),
	     z0 = svlsl_m (p0, z0, z4))

/*
** lsl_s16_m_tied2:
**	mov	(z[0-9]+)\.d, z0\.d
**	movprfx	z0, z4
**	lsl	z0\.h, p0/m, z0\.h, \1\.h
**	ret
*/
TEST_DUAL_Z_REV (lsl_s16_m_tied2, svint16_t, svuint16_t,
		 z0_res = svlsl_s16_m (p0, z4, z0),
		 z0_res = svlsl_m (p0, z4, z0))

/*
** lsl_s16_m_untied:
**	movprfx	z0, z1
**	lsl	z0\.h, p0/m, z0\.h, z4\.h
**	ret
*/
TEST_DUAL_Z (lsl_s16_m_untied, svint16_t, svuint16_t,
	     z0 = svlsl_s16_m (p0, z1, z4),
	     z0 = svlsl_m (p0, z1, z4))

/*
** lsl_w0_s16_m_tied1:
**	mov	(z[0-9]+\.h), w0
**	lsl	z0\.h, p0/m, z0\.h, \1
**	ret
*/
TEST_UNIFORM_ZX (lsl_w0_s16_m_tied1, svint16_t, uint16_t,
		 z0 = svlsl_n_s16_m (p0, z0, x0),
		 z0 = svlsl_m (p0, z0, x0))

/*
** lsl_w0_s16_m_untied: { xfail *-*-* }
**	mov	(z[0-9]+\.h), w0
**	movprfx	z0, z1
**	lsl	z0\.h, p0/m, z0\.h, \1
**	ret
*/
TEST_UNIFORM_ZX (lsl_w0_s16_m_untied, svint16_t, uint16_t,
		 z0 = svlsl_n_s16_m (p0, z1, x0),
		 z0 = svlsl_m (p0, z1, x0))

/*
** lsl_1_s16_m_tied1:
**	lsl	z0\.h, p0/m, z0\.h, #1
**	ret
*/
TEST_UNIFORM_Z (lsl_1_s16_m_tied1, svint16_t,
		z0 = svlsl_n_s16_m (p0, z0, 1),
		z0 = svlsl_m (p0, z0, 1))

/*
** lsl_1_s16_m_untied:
**	movprfx	z0, z1
**	lsl	z0\.h, p0/m, z0\.h, #1
**	ret
*/
TEST_UNIFORM_Z (lsl_1_s16_m_untied, svint16_t,
		z0 = svlsl_n_s16_m (p0, z1, 1),
		z0 = svlsl_m (p0, z1, 1))

/*
** lsl_15_s16_m_tied1:
**	lsl	z0\.h, p0/m, z0\.h, #15
**	ret
*/
TEST_UNIFORM_Z (lsl_15_s16_m_tied1, svint16_t,
		z0 = svlsl_n_s16_m (p0, z0, 15),
		z0 = svlsl_m (p0, z0, 15))

/*
** lsl_15_s16_m_untied:
**	movprfx	z0, z1
**	lsl	z0\.h, p0/m, z0\.h, #15
**	ret
*/
TEST_UNIFORM_Z (lsl_15_s16_m_untied, svint16_t,
		z0 = svlsl_n_s16_m (p0, z1, 15),
		z0 = svlsl_m (p0, z1, 15))

/*
** lsl_16_s16_m_tied1:
**	mov	(z[0-9]+\.h), #16
**	lsl	z0\.h, p0/m, z0\.h, \1
**	ret
*/
TEST_UNIFORM_Z (lsl_16_s16_m_tied1, svint16_t,
		z0 = svlsl_n_s16_m (p0, z0, 16),
		z0 = svlsl_m (p0, z0, 16))

/*
** lsl_16_s16_m_untied: { xfail *-*-* }
**	mov	(z[0-9]+\.h), #16
**	movprfx	z0, z1
**	lsl	z0\.h, p0/m, z0\.h, \1
**	ret
*/
TEST_UNIFORM_Z (lsl_16_s16_m_untied, svint16_t,
		z0 = svlsl_n_s16_m (p0, z1, 16),
		z0 = svlsl_m (p0, z1, 16))

/*
** lsl_s16_z_tied1:
**	movprfx	z0\.h, p0/z, z0\.h
**	lsl	z0\.h, p0/m, z0\.h, z4\.h
**	ret
*/
TEST_DUAL_Z (lsl_s16_z_tied1, svint16_t, svuint16_t,
	     z0 = svlsl_s16_z (p0, z0, z4),
	     z0 = svlsl_z (p0, z0, z4))

/*
** lsl_s16_z_tied2:
**	movprfx	z0\.h, p0/z, z0\.h
**	lslr	z0\.h, p0/m, z0\.h, z4\.h
**	ret
*/
TEST_DUAL_Z_REV (lsl_s16_z_tied2, svint16_t, svuint16_t,
		 z0_res = svlsl_s16_z (p0, z4, z0),
		 z0_res = svlsl_z (p0, z4, z0))

/*
** lsl_s16_z_untied:
** (
**	movprfx	z0\.h, p0/z, z1\.h
**	lsl	z0\.h, p0/m, z0\.h, z4\.h
** |
**	movprfx	z0\.h, p0/z, z4\.h
**	lslr	z0\.h, p0/m, z0\.h, z1\.h
** )
**	ret
*/
TEST_DUAL_Z (lsl_s16_z_untied, svint16_t, svuint16_t,
	     z0 = svlsl_s16_z (p0, z1, z4),
	     z0 = svlsl_z (p0, z1, z4))

/*
** lsl_w0_s16_z_tied1:
**	mov	(z[0-9]+\.h), w0
**	movprfx	z0\.h, p0/z, z0\.h
**	lsl	z0\.h, p0/m, z0\.h, \1
**	ret
*/
TEST_UNIFORM_ZX (lsl_w0_s16_z_tied1, svint16_t, uint16_t,
		 z0 = svlsl_n_s16_z (p0, z0, x0),
		 z0 = svlsl_z (p0, z0, x0))

/*
** lsl_w0_s16_z_untied:
**	mov	(z[0-9]+\.h), w0
** (
**	movprfx	z0\.h, p0/z, z1\.h
**	lsl	z0\.h, p0/m, z0\.h, \1
** |
**	movprfx	z0\.h, p0/z, \1
**	lslr	z0\.h, p0/m, z0\.h, z1\.h
** )
**	ret
*/
TEST_UNIFORM_ZX (lsl_w0_s16_z_untied, svint16_t, uint16_t,
		 z0 = svlsl_n_s16_z (p0, z1, x0),
		 z0 = svlsl_z (p0, z1, x0))

/*
** lsl_1_s16_z_tied1:
**	movprfx	z0\.h, p0/z, z0\.h
**	lsl	z0\.h, p0/m, z0\.h, #1
**	ret
*/
TEST_UNIFORM_Z (lsl_1_s16_z_tied1, svint16_t,
		z0 = svlsl_n_s16_z (p0, z0, 1),
		z0 = svlsl_z (p0, z0, 1))

/*
** lsl_1_s16_z_untied:
**	movprfx	z0\.h, p0/z, z1\.h
**	lsl	z0\.h, p0/m, z0\.h, #1
**	ret
*/
TEST_UNIFORM_Z (lsl_1_s16_z_untied, svint16_t,
		z0 = svlsl_n_s16_z (p0, z1, 1),
		z0 = svlsl_z (p0, z1, 1))

/*
** lsl_15_s16_z_tied1:
**	movprfx	z0\.h, p0/z, z0\.h
**	lsl	z0\.h, p0/m, z0\.h, #15
**	ret
*/
TEST_UNIFORM_Z (lsl_15_s16_z_tied1, svint16_t,
		z0 = svlsl_n_s16_z (p0, z0, 15),
		z0 = svlsl_z (p0, z0, 15))

/*
** lsl_15_s16_z_untied:
**	movprfx	z0\.h, p0/z, z1\.h
**	lsl	z0\.h, p0/m, z0\.h, #15
**	ret
*/
TEST_UNIFORM_Z (lsl_15_s16_z_untied, svint16_t,
		z0 = svlsl_n_s16_z (p0, z1, 15),
		z0 = svlsl_z (p0, z1, 15))

/*
** lsl_16_s16_z_tied1:
**	mov	(z[0-9]+\.h), #16
**	movprfx	z0\.h, p0/z, z0\.h
**	lsl	z0\.h, p0/m, z0\.h, \1
**	ret
*/
TEST_UNIFORM_Z (lsl_16_s16_z_tied1, svint16_t,
		z0 = svlsl_n_s16_z (p0, z0, 16),
		z0 = svlsl_z (p0, z0, 16))

/*
** lsl_16_s16_z_untied:
**	mov	(z[0-9]+\.h), #16
** (
**	movprfx	z0\.h, p0/z, z1\.h
**	lsl	z0\.h, p0/m, z0\.h, \1
** |
**	movprfx	z0\.h, p0/z, \1
**	lslr	z0\.h, p0/m, z0\.h, z1\.h
** )
**	ret
*/
TEST_UNIFORM_Z (lsl_16_s16_z_untied, svint16_t,
		z0 = svlsl_n_s16_z (p0, z1, 16),
		z0 = svlsl_z (p0, z1, 16))

/*
** lsl_s16_x_tied1:
**	lsl	z0\.h, p0/m, z0\.h, z4\.h
**	ret
*/
TEST_DUAL_Z (lsl_s16_x_tied1, svint16_t, svuint16_t,
	     z0 = svlsl_s16_x (p0, z0, z4),
	     z0 = svlsl_x (p0, z0, z4))

/*
** lsl_s16_x_tied2:
**	lslr	z0\.h, p0/m, z0\.h, z4\.h
**	ret
*/
TEST_DUAL_Z_REV (lsl_s16_x_tied2, svint16_t, svuint16_t,
		 z0_res = svlsl_s16_x (p0, z4, z0),
		 z0_res = svlsl_x (p0, z4, z0))

/*
** lsl_s16_x_untied:
** (
**	movprfx	z0, z1
**	lsl	z0\.h, p0/m, z0\.h, z4\.h
** |
**	movprfx	z0, z4
**	lslr	z0\.h, p0/m, z0\.h, z1\.h
** )
**	ret
*/
TEST_DUAL_Z (lsl_s16_x_untied, svint16_t, svuint16_t,
	     z0 = svlsl_s16_x (p0, z1, z4),
	     z0 = svlsl_x (p0, z1, z4))

/*
** lsl_w0_s16_x_tied1:
**	mov	(z[0-9]+\.h), w0
**	lsl	z0\.h, p0/m, z0\.h, \1
**	ret
*/
TEST_UNIFORM_ZX (lsl_w0_s16_x_tied1, svint16_t, uint16_t,
		 z0 = svlsl_n_s16_x (p0, z0, x0),
		 z0 = svlsl_x (p0, z0, x0))

/*
** lsl_w0_s16_x_untied:
**	mov	z0\.h, w0
**	lslr	z0\.h, p0/m, z0\.h, z1\.h
**	ret
*/
TEST_UNIFORM_ZX (lsl_w0_s16_x_untied, svint16_t, uint16_t,
		 z0 = svlsl_n_s16_x (p0, z1, x0),
		 z0 = svlsl_x (p0, z1, x0))

/*
** lsl_1_s16_x_tied1:
**	lsl	z0\.h, z0\.h, #1
**	ret
*/
TEST_UNIFORM_Z (lsl_1_s16_x_tied1, svint16_t,
		z0 = svlsl_n_s16_x (p0, z0, 1),
		z0 = svlsl_x (p0, z0, 1))

/*
** lsl_1_s16_x_untied:
**	lsl	z0\.h, z1\.h, #1
**	ret
*/
TEST_UNIFORM_Z (lsl_1_s16_x_untied, svint16_t,
		z0 = svlsl_n_s16_x (p0, z1, 1),
		z0 = svlsl_x (p0, z1, 1))

/*
** lsl_15_s16_x_tied1:
**	lsl	z0\.h, z0\.h, #15
**	ret
*/
TEST_UNIFORM_Z (lsl_15_s16_x_tied1, svint16_t,
		z0 = svlsl_n_s16_x (p0, z0, 15),
		z0 = svlsl_x (p0, z0, 15))

/*
** lsl_15_s16_x_untied:
**	lsl	z0\.h, z1\.h, #15
**	ret
*/
TEST_UNIFORM_Z (lsl_15_s16_x_untied, svint16_t,
		z0 = svlsl_n_s16_x (p0, z1, 15),
		z0 = svlsl_x (p0, z1, 15))

/*
** lsl_16_s16_x_tied1:
**	mov	(z[0-9]+\.h), #16
**	lsl	z0\.h, p0/m, z0\.h, \1
**	ret
*/
TEST_UNIFORM_Z (lsl_16_s16_x_tied1, svint16_t,
		z0 = svlsl_n_s16_x (p0, z0, 16),
		z0 = svlsl_x (p0, z0, 16))

/*
** lsl_16_s16_x_untied:
**	mov	z0\.h, #16
**	lslr	z0\.h, p0/m, z0\.h, z1\.h
**	ret
*/
TEST_UNIFORM_Z (lsl_16_s16_x_untied, svint16_t,
		z0 = svlsl_n_s16_x (p0, z1, 16),
		z0 = svlsl_x (p0, z1, 16))
