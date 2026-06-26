/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" } } */

#include "test_sve_acle.h"

/*
** mulh_u64_x_tied1:
**	umulh	z0\.d, z0\.d, z1\.d
**	ret
*/
TEST_UNIFORM_Z (mulh_u64_x_tied1, svuint64_t,
		z0 = svmulh_u64_x (p0, z0, z1),
		z0 = svmulh_x (p0, z0, z1))

/*
** mulh_u64_x_tied2:
**	umulh	z0\.d, z1\.d, z0\.d
**	ret
*/
TEST_UNIFORM_Z (mulh_u64_x_tied2, svuint64_t,
		z0 = svmulh_u64_x (p0, z1, z0),
		z0 = svmulh_x (p0, z1, z0))

/*
** mulh_u64_x_untied:
**	umulh	z0\.d, z1\.d, z2\.d
**	ret
*/
TEST_UNIFORM_Z (mulh_u64_x_untied, svuint64_t,
		z0 = svmulh_u64_x (p0, z1, z2),
		z0 = svmulh_x (p0, z1, z2))

/*
** mulh_x0_u64_x_tied1:
**	mov	(z[0-9]+\.d), x0
**	umulh	z0\.d, z0\.d, \1
**	ret
*/
TEST_UNIFORM_ZX (mulh_x0_u64_x_tied1, svuint64_t, uint64_t,
		 z0 = svmulh_n_u64_x (p0, z0, x0),
		 z0 = svmulh_x (p0, z0, x0))

/*
** mulh_x0_u64_x_untied:
**	mov	z0\.d, x0
**	umulh	z0\.d, z1\.d, z0\.d
**	ret
*/
TEST_UNIFORM_ZX (mulh_x0_u64_x_untied, svuint64_t, uint64_t,
		 z0 = svmulh_n_u64_x (p0, z1, x0),
		 z0 = svmulh_x (p0, z1, x0))

/*
** mulh_11_u64_x_tied1:
**	mov	(z[0-9]+\.d), #11
**	umulh	z0\.d, z0\.d, \1
**	ret
*/
TEST_UNIFORM_Z (mulh_11_u64_x_tied1, svuint64_t,
		z0 = svmulh_n_u64_x (p0, z0, 11),
		z0 = svmulh_x (p0, z0, 11))

/*
** mulh_11_u64_x_untied:
**	mov	z0\.d, #11
**	umulh	z0\.d, z1\.d, z0\.d
**	ret
*/
TEST_UNIFORM_Z (mulh_11_u64_x_untied, svuint64_t,
		z0 = svmulh_n_u64_x (p0, z1, 11),
		z0 = svmulh_x (p0, z1, 11))
