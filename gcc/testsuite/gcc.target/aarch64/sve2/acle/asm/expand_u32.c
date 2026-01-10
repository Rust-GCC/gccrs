/* { dg-do assemble { target aarch64_asm_sve2p2_ok } } */
/* { dg-do compile { target { ! aarch64_asm_sve2p2_ok } } } */
/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" } } */

#include "test_sve_acle.h"

#pragma GCC target "+sve2p2"
#ifdef STREAMING_COMPATIBLE
#pragma GCC target "+sme2p2"
#endif

/*
** expand_u32_tied1:
**	expand	z0\.s, p0, z0\.s
**	ret
*/
TEST_UNIFORM_Z (expand_u32_tied1, svuint32_t,
		z0 = svexpand_u32 (p0, z0),
		z0 = svexpand (p0, z0))

/*
** expand_u32_untied:
**	expand	z0\.s, p0, z1\.s
**	ret
*/
TEST_UNIFORM_Z (expand_u32_untied, svuint32_t,
		z0 = svexpand_u32 (p0, z1),
		z0 = svexpand (p0, z1))
