/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" } } */

#include "test_sve_acle.h"

/*
** mulh_s64_x_tied1:
**	smulh	z0\.d, z0\.d, z1\.d
**	ret
*/
TEST_UNIFORM_Z (mulh_s64_x_tied1, svint64_t,
		z0 = svmulh_s64_x (p0, z0, z1),
		z0 = svmulh_x (p0, z0, z1))

/*
** mulh_s64_x_tied2:
**	smulh	z0\.d, z1\.d, z0\.d
**	ret
*/
TEST_UNIFORM_Z (mulh_s64_x_tied2, svint64_t,
		z0 = svmulh_s64_x (p0, z1, z0),
		z0 = svmulh_x (p0, z1, z0))

/*
** mulh_s64_x_untied:
**	smulh	z0\.d, z1\.d, z2\.d
**	ret
*/
TEST_UNIFORM_Z (mulh_s64_x_untied, svint64_t,
		z0 = svmulh_s64_x (p0, z1, z2),
		z0 = svmulh_x (p0, z1, z2))

/*
** mulh_x0_s64_x_tied1:
**	mov	(z[0-9]+\.d), x0
**	smulh	z0\.d, z0\.d, \1
**	ret
*/
TEST_UNIFORM_ZX (mulh_x0_s64_x_tied1, svint64_t, int64_t,
		 z0 = svmulh_n_s64_x (p0, z0, x0),
		 z0 = svmulh_x (p0, z0, x0))

/*
** mulh_x0_s64_x_untied:
**	mov	z0\.d, x0
**	smulh	z0\.d, z1\.d, z0\.d
**	ret
*/
TEST_UNIFORM_ZX (mulh_x0_s64_x_untied, svint64_t, int64_t,
		 z0 = svmulh_n_s64_x (p0, z1, x0),
		 z0 = svmulh_x (p0, z1, x0))

/*
** mulh_11_s64_x_tied1:
**	mov	(z[0-9]+\.d), #11
**	smulh	z0\.d, z0\.d, \1
**	ret
*/
TEST_UNIFORM_Z (mulh_11_s64_x_tied1, svint64_t,
		z0 = svmulh_n_s64_x (p0, z0, 11),
		z0 = svmulh_x (p0, z0, 11))

/*
** mulh_11_s64_x_untied:
**	mov	z0\.d, #11
**	smulh	z0\.d, z1\.d, z0\.d
**	ret
*/
TEST_UNIFORM_Z (mulh_11_s64_x_untied, svint64_t,
		z0 = svmulh_n_s64_x (p0, z1, 11),
		z0 = svmulh_x (p0, z1, 11))
