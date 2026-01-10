/* { dg-do assemble { target aarch64_asm_sve2p2_ok } } */
/* { dg-do compile { target { ! aarch64_asm_sve2p2_ok } } } */
/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" } } */

#include "test_sve_acle.h"

#pragma GCC target "+sve2p2"
#ifdef STREAMING_COMPATIBLE
#pragma GCC target "+sme2p2"
#endif

/*
** compact_mf8_tied1:
**	compact	z0\.b, p0, z0\.b
**	ret
*/
TEST_UNIFORM_Z (compact_mf8_tied1, svmfloat8_t,
		z0 = svcompact_mf8 (p0, z0),
		z0 = svcompact (p0, z0))

/*
** compact_mf8_untied:
**	compact	z0\.b, p0, z1\.b
**	ret
*/
TEST_UNIFORM_Z (compact_mf8_untied, svmfloat8_t,
		z0 = svcompact_mf8 (p0, z1),
		z0 = svcompact (p0, z1))
