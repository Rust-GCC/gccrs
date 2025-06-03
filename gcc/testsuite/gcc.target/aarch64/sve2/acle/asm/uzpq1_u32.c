/* { dg-do assemble { target aarch64_asm_sve2p1_ok } } */
/* { dg-do compile { target { ! aarch64_asm_sve2p1_ok } } } */
/* { dg-skip-if "" { *-*-* } { "-DSTREAMING_COMPATIBLE" } { "" } } */
/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" } } */

#include "test_sve_acle.h"

#pragma GCC target "+sve2p1"

/*
** uzpq1_u32_tied1:
**	uzpq1	z0\.s, z0\.s, z1\.s
**	ret
*/
TEST_UNIFORM_Z (uzpq1_u32_tied1, svuint32_t,
		z0 = svuzpq1_u32 (z0, z1),
		z0 = svuzpq1 (z0, z1))

/*
** uzpq1_u32_tied2:
**	uzpq1	z0\.s, z1\.s, z0\.s
**	ret
*/
TEST_UNIFORM_Z (uzpq1_u32_tied2, svuint32_t,
		z0 = svuzpq1_u32 (z1, z0),
		z0 = svuzpq1 (z1, z0))

/*
** uzpq1_u32_untied:
**	uzpq1	z0\.s, z1\.s, z2\.s
**	ret
*/
TEST_UNIFORM_Z (uzpq1_u32_untied, svuint32_t,
		z0 = svuzpq1_u32 (z1, z2),
		z0 = svuzpq1 (z1, z2))
