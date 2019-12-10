/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" } } */

#include "test_sve_acle.h"

/*
** mul_u16_m_tied1:
**	mul	z0\.h, p0/m, z0\.h, z1\.h
**	ret
*/
TEST_UNIFORM_Z (mul_u16_m_tied1, svuint16_t,
		z0 = svmul_u16_m (p0, z0, z1),
		z0 = svmul_m (p0, z0, z1))

/*
** mul_u16_m_tied2:
**	mov	(z[0-9]+)\.d, z0\.d
**	movprfx	z0, z1
**	mul	z0\.h, p0/m, z0\.h, \1\.h
**	ret
*/
TEST_UNIFORM_Z (mul_u16_m_tied2, svuint16_t,
		z0 = svmul_u16_m (p0, z1, z0),
		z0 = svmul_m (p0, z1, z0))

/*
** mul_u16_m_untied:
**	movprfx	z0, z1
**	mul	z0\.h, p0/m, z0\.h, z2\.h
**	ret
*/
TEST_UNIFORM_Z (mul_u16_m_untied, svuint16_t,
		z0 = svmul_u16_m (p0, z1, z2),
		z0 = svmul_m (p0, z1, z2))

/*
** mul_w0_u16_m_tied1:
**	mov	(z[0-9]+\.h), w0
**	mul	z0\.h, p0/m, z0\.h, \1
**	ret
*/
TEST_UNIFORM_ZX (mul_w0_u16_m_tied1, svuint16_t, uint16_t,
		 z0 = svmul_n_u16_m (p0, z0, x0),
		 z0 = svmul_m (p0, z0, x0))

/*
** mul_w0_u16_m_untied: { xfail *-*-* }
**	mov	(z[0-9]+\.h), w0
**	movprfx	z0, z1
**	mul	z0\.h, p0/m, z0\.h, \1
**	ret
*/
TEST_UNIFORM_ZX (mul_w0_u16_m_untied, svuint16_t, uint16_t,
		 z0 = svmul_n_u16_m (p0, z1, x0),
		 z0 = svmul_m (p0, z1, x0))

/*
** mul_2_u16_m_tied1:
**	mov	(z[0-9]+\.h), #2
**	mul	z0\.h, p0/m, z0\.h, \1
**	ret
*/
TEST_UNIFORM_Z (mul_2_u16_m_tied1, svuint16_t,
		z0 = svmul_n_u16_m (p0, z0, 2),
		z0 = svmul_m (p0, z0, 2))

/*
** mul_2_u16_m_untied: { xfail *-*-* }
**	mov	(z[0-9]+\.h), #2
**	movprfx	z0, z1
**	mul	z0\.h, p0/m, z0\.h, \1
**	ret
*/
TEST_UNIFORM_Z (mul_2_u16_m_untied, svuint16_t,
		z0 = svmul_n_u16_m (p0, z1, 2),
		z0 = svmul_m (p0, z1, 2))

/*
** mul_m1_u16_m:
**	mov	(z[0-9]+)\.b, #-1
**	mul	z0\.h, p0/m, z0\.h, \1\.h
**	ret
*/
TEST_UNIFORM_Z (mul_m1_u16_m, svuint16_t,
		z0 = svmul_n_u16_m (p0, z0, -1),
		z0 = svmul_m (p0, z0, -1))

/*
** mul_u16_z_tied1:
**	movprfx	z0\.h, p0/z, z0\.h
**	mul	z0\.h, p0/m, z0\.h, z1\.h
**	ret
*/
TEST_UNIFORM_Z (mul_u16_z_tied1, svuint16_t,
		z0 = svmul_u16_z (p0, z0, z1),
		z0 = svmul_z (p0, z0, z1))

/*
** mul_u16_z_tied2:
**	movprfx	z0\.h, p0/z, z0\.h
**	mul	z0\.h, p0/m, z0\.h, z1\.h
**	ret
*/
TEST_UNIFORM_Z (mul_u16_z_tied2, svuint16_t,
		z0 = svmul_u16_z (p0, z1, z0),
		z0 = svmul_z (p0, z1, z0))

/*
** mul_u16_z_untied:
** (
**	movprfx	z0\.h, p0/z, z1\.h
**	mul	z0\.h, p0/m, z0\.h, z2\.h
** |
**	movprfx	z0\.h, p0/z, z2\.h
**	mul	z0\.h, p0/m, z0\.h, z1\.h
** )
**	ret
*/
TEST_UNIFORM_Z (mul_u16_z_untied, svuint16_t,
		z0 = svmul_u16_z (p0, z1, z2),
		z0 = svmul_z (p0, z1, z2))

/*
** mul_w0_u16_z_tied1:
**	mov	(z[0-9]+\.h), w0
**	movprfx	z0\.h, p0/z, z0\.h
**	mul	z0\.h, p0/m, z0\.h, \1
**	ret
*/
TEST_UNIFORM_ZX (mul_w0_u16_z_tied1, svuint16_t, uint16_t,
		 z0 = svmul_n_u16_z (p0, z0, x0),
		 z0 = svmul_z (p0, z0, x0))

/*
** mul_w0_u16_z_untied:
**	mov	(z[0-9]+\.h), w0
** (
**	movprfx	z0\.h, p0/z, z1\.h
**	mul	z0\.h, p0/m, z0\.h, \1
** |
**	movprfx	z0\.h, p0/z, \1
**	mul	z0\.h, p0/m, z0\.h, z1\.h
** )
**	ret
*/
TEST_UNIFORM_ZX (mul_w0_u16_z_untied, svuint16_t, uint16_t,
		 z0 = svmul_n_u16_z (p0, z1, x0),
		 z0 = svmul_z (p0, z1, x0))

/*
** mul_2_u16_z_tied1:
**	mov	(z[0-9]+\.h), #2
**	movprfx	z0\.h, p0/z, z0\.h
**	mul	z0\.h, p0/m, z0\.h, \1
**	ret
*/
TEST_UNIFORM_Z (mul_2_u16_z_tied1, svuint16_t,
		z0 = svmul_n_u16_z (p0, z0, 2),
		z0 = svmul_z (p0, z0, 2))

/*
** mul_2_u16_z_untied:
**	mov	(z[0-9]+\.h), #2
** (
**	movprfx	z0\.h, p0/z, z1\.h
**	mul	z0\.h, p0/m, z0\.h, \1
** |
**	movprfx	z0\.h, p0/z, \1
**	mul	z0\.h, p0/m, z0\.h, z1\.h
** )
**	ret
*/
TEST_UNIFORM_Z (mul_2_u16_z_untied, svuint16_t,
		z0 = svmul_n_u16_z (p0, z1, 2),
		z0 = svmul_z (p0, z1, 2))

/*
** mul_u16_x_tied1:
**	mul	z0\.h, p0/m, z0\.h, z1\.h
**	ret
*/
TEST_UNIFORM_Z (mul_u16_x_tied1, svuint16_t,
		z0 = svmul_u16_x (p0, z0, z1),
		z0 = svmul_x (p0, z0, z1))

/*
** mul_u16_x_tied2:
**	mul	z0\.h, p0/m, z0\.h, z1\.h
**	ret
*/
TEST_UNIFORM_Z (mul_u16_x_tied2, svuint16_t,
		z0 = svmul_u16_x (p0, z1, z0),
		z0 = svmul_x (p0, z1, z0))

/*
** mul_u16_x_untied:
** (
**	movprfx	z0, z1
**	mul	z0\.h, p0/m, z0\.h, z2\.h
** |
**	movprfx	z0, z2
**	mul	z0\.h, p0/m, z0\.h, z1\.h
** )
**	ret
*/
TEST_UNIFORM_Z (mul_u16_x_untied, svuint16_t,
		z0 = svmul_u16_x (p0, z1, z2),
		z0 = svmul_x (p0, z1, z2))

/*
** mul_w0_u16_x_tied1:
**	mov	(z[0-9]+\.h), w0
**	mul	z0\.h, p0/m, z0\.h, \1
**	ret
*/
TEST_UNIFORM_ZX (mul_w0_u16_x_tied1, svuint16_t, uint16_t,
		 z0 = svmul_n_u16_x (p0, z0, x0),
		 z0 = svmul_x (p0, z0, x0))

/*
** mul_w0_u16_x_untied:
**	mov	z0\.h, w0
**	mul	z0\.h, p0/m, z0\.h, z1\.h
**	ret
*/
TEST_UNIFORM_ZX (mul_w0_u16_x_untied, svuint16_t, uint16_t,
		 z0 = svmul_n_u16_x (p0, z1, x0),
		 z0 = svmul_x (p0, z1, x0))

/*
** mul_2_u16_x_tied1:
**	mul	z0\.h, z0\.h, #2
**	ret
*/
TEST_UNIFORM_Z (mul_2_u16_x_tied1, svuint16_t,
		z0 = svmul_n_u16_x (p0, z0, 2),
		z0 = svmul_x (p0, z0, 2))

/*
** mul_2_u16_x_untied:
**	movprfx	z0, z1
**	mul	z0\.h, z0\.h, #2
**	ret
*/
TEST_UNIFORM_Z (mul_2_u16_x_untied, svuint16_t,
		z0 = svmul_n_u16_x (p0, z1, 2),
		z0 = svmul_x (p0, z1, 2))

/*
** mul_127_u16_x:
**	mul	z0\.h, z0\.h, #127
**	ret
*/
TEST_UNIFORM_Z (mul_127_u16_x, svuint16_t,
		z0 = svmul_n_u16_x (p0, z0, 127),
		z0 = svmul_x (p0, z0, 127))

/*
** mul_128_u16_x:
**	mov	(z[0-9]+\.h), #128
**	mul	z0\.h, p0/m, z0\.h, \1
**	ret
*/
TEST_UNIFORM_Z (mul_128_u16_x, svuint16_t,
		z0 = svmul_n_u16_x (p0, z0, 128),
		z0 = svmul_x (p0, z0, 128))

/*
** mul_255_u16_x:
**	mov	(z[0-9]+\.h), #255
**	mul	z0\.h, p0/m, z0\.h, \1
**	ret
*/
TEST_UNIFORM_Z (mul_255_u16_x, svuint16_t,
		z0 = svmul_n_u16_x (p0, z0, 255),
		z0 = svmul_x (p0, z0, 255))

/*
** mul_m1_u16_x:
**	mul	z0\.h, z0\.h, #-1
**	ret
*/
TEST_UNIFORM_Z (mul_m1_u16_x, svuint16_t,
		z0 = svmul_n_u16_x (p0, z0, -1),
		z0 = svmul_x (p0, z0, -1))

/*
** mul_m127_u16_x:
**	mul	z0\.h, z0\.h, #-127
**	ret
*/
TEST_UNIFORM_Z (mul_m127_u16_x, svuint16_t,
		z0 = svmul_n_u16_x (p0, z0, -127),
		z0 = svmul_x (p0, z0, -127))

/*
** mul_m128_u16_x:
**	mul	z0\.h, z0\.h, #-128
**	ret
*/
TEST_UNIFORM_Z (mul_m128_u16_x, svuint16_t,
		z0 = svmul_n_u16_x (p0, z0, -128),
		z0 = svmul_x (p0, z0, -128))
