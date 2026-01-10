/* { dg-do assemble { target aarch64_asm_sve2p2_ok } } */
/* { dg-do compile { target { ! aarch64_asm_sve2p2_ok } } } */
/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" } } */

#include "test_sve_acle.h"

#pragma GCC target "+sve2p2"
#ifdef STREAMING_COMPATIBLE
#pragma GCC target "+sme2p2"
#endif

/*
** expand_s8_tied1:
**	expand	z0\.b, p0, z0\.b
**	ret
*/
TEST_UNIFORM_Z (expand_s8_tied1, svint8_t,
		z0 = svexpand_s8 (p0, z0),
		z0 = svexpand (p0, z0))

/*
** expand_s8_untied:
**	expand	z0\.b, p0, z1\.b
**	ret
*/
TEST_UNIFORM_Z (expand_s8_untied, svint8_t,
		z0 = svexpand_s8 (p0, z1),
		z0 = svexpand (p0, z1))
