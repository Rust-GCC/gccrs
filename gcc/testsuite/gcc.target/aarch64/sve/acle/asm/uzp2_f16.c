/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" } } */

#include "test_sve_acle.h"

/*
** uzp2_f16_tied1:
**	uzp2	z0\.h, z0\.h, z1\.h
**	ret
*/
TEST_UNIFORM_Z (uzp2_f16_tied1, svfloat16_t,
		z0 = svuzp2_f16 (z0, z1),
		z0 = svuzp2 (z0, z1))

/*
** uzp2_f16_tied2:
**	uzp2	z0\.h, z1\.h, z0\.h
**	ret
*/
TEST_UNIFORM_Z (uzp2_f16_tied2, svfloat16_t,
		z0 = svuzp2_f16 (z1, z0),
		z0 = svuzp2 (z1, z0))

/*
** uzp2_f16_untied:
**	uzp2	z0\.h, z1\.h, z2\.h
**	ret
*/
TEST_UNIFORM_Z (uzp2_f16_untied, svfloat16_t,
		z0 = svuzp2_f16 (z1, z2),
		z0 = svuzp2 (z1, z2))
