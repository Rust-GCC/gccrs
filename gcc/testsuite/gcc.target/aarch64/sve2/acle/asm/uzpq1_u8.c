/* { dg-do assemble { target aarch64_asm_sve2p1_ok } } */
/* { dg-do compile { target { ! aarch64_asm_sve2p1_ok } } } */
/* { dg-skip-if "" { *-*-* } { "-DSTREAMING_COMPATIBLE" } { "" } } */
/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" } } */

#include "test_sve_acle.h"

#pragma GCC target "+sve2p1"

/*
** uzpq1_u8_tied1:
**	uzpq1	z0\.b, z0\.b, z1\.b
**	ret
*/
TEST_UNIFORM_Z (uzpq1_u8_tied1, svuint8_t,
		z0 = svuzpq1_u8 (z0, z1),
		z0 = svuzpq1 (z0, z1))

/*
** uzpq1_u8_tied2:
**	uzpq1	z0\.b, z1\.b, z0\.b
**	ret
*/
TEST_UNIFORM_Z (uzpq1_u8_tied2, svuint8_t,
		z0 = svuzpq1_u8 (z1, z0),
		z0 = svuzpq1 (z1, z0))

/*
** uzpq1_u8_untied:
**	uzpq1	z0\.b, z1\.b, z2\.b
**	ret
*/
TEST_UNIFORM_Z (uzpq1_u8_untied, svuint8_t,
		z0 = svuzpq1_u8 (z1, z2),
		z0 = svuzpq1 (z1, z2))
