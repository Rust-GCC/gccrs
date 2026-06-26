/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" } } */

#include "test_sve_acle.h"

/*
** mulh_u16_x_tied1:
**	umulh	z0\.h, z0\.h, z1\.h
**	ret
*/
TEST_UNIFORM_Z (mulh_u16_x_tied1, svuint16_t,
		z0 = svmulh_u16_x (p0, z0, z1),
		z0 = svmulh_x (p0, z0, z1))

/*
** mulh_u16_x_tied2:
**	umulh	z0\.h, z1\.h, z0\.h
**	ret
*/
TEST_UNIFORM_Z (mulh_u16_x_tied2, svuint16_t,
		z0 = svmulh_u16_x (p0, z1, z0),
		z0 = svmulh_x (p0, z1, z0))

/*
** mulh_u16_x_untied:
**	umulh	z0\.h, z1\.h, z2\.h
**	ret
*/
TEST_UNIFORM_Z (mulh_u16_x_untied, svuint16_t,
		z0 = svmulh_u16_x (p0, z1, z2),
		z0 = svmulh_x (p0, z1, z2))

/*
** mulh_w0_u16_x_tied1:
**	mov	(z[0-9]+\.h), w0
**	umulh	z0\.h, z0\.h, \1
**	ret
*/
TEST_UNIFORM_ZX (mulh_w0_u16_x_tied1, svuint16_t, uint16_t,
		 z0 = svmulh_n_u16_x (p0, z0, x0),
		 z0 = svmulh_x (p0, z0, x0))

/*
** mulh_w0_u16_x_untied:
**	mov	z0\.h, w0
**	umulh	z0\.h, z1\.h, z0\.h
**	ret
*/
TEST_UNIFORM_ZX (mulh_w0_u16_x_untied, svuint16_t, uint16_t,
		 z0 = svmulh_n_u16_x (p0, z1, x0),
		 z0 = svmulh_x (p0, z1, x0))

/*
** mulh_11_u16_x_tied1:
**	mov	(z[0-9]+\.h), #11
**	umulh	z0\.h, z0\.h, \1
**	ret
*/
TEST_UNIFORM_Z (mulh_11_u16_x_tied1, svuint16_t,
		z0 = svmulh_n_u16_x (p0, z0, 11),
		z0 = svmulh_x (p0, z0, 11))

/*
** mulh_11_u16_x_untied:
**	mov	z0\.h, #11
**	umulh	z0\.h, z1\.h, z0\.h
**	ret
*/
TEST_UNIFORM_Z (mulh_11_u16_x_untied, svuint16_t,
		z0 = svmulh_n_u16_x (p0, z1, 11),
		z0 = svmulh_x (p0, z1, 11))
