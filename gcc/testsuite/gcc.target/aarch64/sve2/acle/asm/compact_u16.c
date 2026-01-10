/* { dg-do assemble { target aarch64_asm_sve2p2_ok } } */
/* { dg-do compile { target { ! aarch64_asm_sve2p2_ok } } } */
/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" } } */

#include "test_sve_acle.h"

#pragma GCC target "+sve2p2"
#ifdef STREAMING_COMPATIBLE
#pragma GCC target "+sme2p2"
#endif

/*
** compact_u16_tied1:
**	compact	z0\.h, p0, z0\.h
**	ret
*/
TEST_UNIFORM_Z (compact_u16_tied1, svuint16_t,
		z0 = svcompact_u16 (p0, z0),
		z0 = svcompact (p0, z0))

/*
** compact_u16_untied:
**	compact	z0\.h, p0, z1\.h
**	ret
*/
TEST_UNIFORM_Z (compact_u16_untied, svuint16_t,
		z0 = svcompact_u16 (p0, z1),
		z0 = svcompact (p0, z1))
