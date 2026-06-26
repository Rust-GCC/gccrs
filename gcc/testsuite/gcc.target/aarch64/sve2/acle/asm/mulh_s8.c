/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" } } */

#include "test_sve_acle.h"

/*
** mulh_s8_x_tied1:
**	smulh	z0\.b, z0\.b, z1\.b
**	ret
*/
TEST_UNIFORM_Z (mulh_s8_x_tied1, svint8_t,
		z0 = svmulh_s8_x (p0, z0, z1),
		z0 = svmulh_x (p0, z0, z1))

/*
** mulh_s8_x_tied2:
**	smulh	z0\.b, z1\.b, z0\.b
**	ret
*/
TEST_UNIFORM_Z (mulh_s8_x_tied2, svint8_t,
		z0 = svmulh_s8_x (p0, z1, z0),
		z0 = svmulh_x (p0, z1, z0))

/*
** mulh_s8_x_untied:
**	smulh	z0\.b, z1\.b, z2\.b
**	ret
*/
TEST_UNIFORM_Z (mulh_s8_x_untied, svint8_t,
		z0 = svmulh_s8_x (p0, z1, z2),
		z0 = svmulh_x (p0, z1, z2))

/*
** mulh_w0_s8_x_tied1:
**	mov	(z[0-9]+\.b), w0
**	smulh	z0\.b, z0\.b, \1
**	ret
*/
TEST_UNIFORM_ZX (mulh_w0_s8_x_tied1, svint8_t, int8_t,
		 z0 = svmulh_n_s8_x (p0, z0, x0),
		 z0 = svmulh_x (p0, z0, x0))

/*
** mulh_w0_s8_x_untied:
**	mov	z0\.b, w0
**	smulh	z0\.b, z1\.b, z0\.b
**	ret
*/
TEST_UNIFORM_ZX (mulh_w0_s8_x_untied, svint8_t, int8_t,
		 z0 = svmulh_n_s8_x (p0, z1, x0),
		 z0 = svmulh_x (p0, z1, x0))

/*
** mulh_11_s8_x_tied1:
**	mov	(z[0-9]+\.b), #11
**	smulh	z0\.b, z0\.b, \1
**	ret
*/
TEST_UNIFORM_Z (mulh_11_s8_x_tied1, svint8_t,
		z0 = svmulh_n_s8_x (p0, z0, 11),
		z0 = svmulh_x (p0, z0, 11))

/*
** mulh_11_s8_x_untied:
**	mov	z0\.b, #11
**	smulh	z0\.b, z1\.b, z0\.b
**	ret
*/
TEST_UNIFORM_Z (mulh_11_s8_x_untied, svint8_t,
		z0 = svmulh_n_s8_x (p0, z1, 11),
		z0 = svmulh_x (p0, z1, 11))
