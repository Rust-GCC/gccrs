/* { dg-do assemble { target aarch64_asm_sme-b16b16_ok } } */
/* { dg-do compile { target { ! aarch64_asm_sme-b16b16_ok } } } */
/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" } } */

#include "test_sme2_acle.h"

#pragma GCC target "+sme-b16b16"

/*
** mopa_za16_bf16_0_p0_p1_z0_z1:
**	bfmopa	za0\.h, p0/m, p1/m, z0\.h, z1\.h
**	ret
*/
TEST_UNIFORM_ZA (mopa_za16_bf16_0_p0_p1_z0_z1, svbfloat16_t,
		 svmopa_za16_bf16_m (0, p0, p1, z0, z1),
		 svmopa_za16_m (0, p0, p1, z0, z1))

/*
** mopa_za16_bf16_0_p1_p0_z1_z0:
**	bfmopa	za0\.h, p1/m, p0/m, z1\.h, z0\.h
**	ret
*/
TEST_UNIFORM_ZA (mopa_za16_bf16_0_p1_p0_z1_z0, svbfloat16_t,
		 svmopa_za16_bf16_m (0, p1, p0, z1, z0),
		 svmopa_za16_m (0, p1, p0, z1, z0))

/*
** mopa_za16_bf16_1_p0_p1_z0_z1:
**	bfmopa	za1\.h, p0/m, p1/m, z0\.h, z1\.h
**	ret
*/
TEST_UNIFORM_ZA (mopa_za16_bf16_1_p0_p1_z0_z1, svbfloat16_t,
		 svmopa_za16_bf16_m (1, p0, p1, z0, z1),
		 svmopa_za16_m (1, p0, p1, z0, z1))
