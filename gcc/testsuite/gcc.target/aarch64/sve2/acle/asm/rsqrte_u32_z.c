/* { dg-do assemble { target aarch64_asm_sve2p2_ok } } */
/* { dg-do compile { target { ! aarch64_asm_sve2p2_ok } } } */
/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" } } */

#include "test_sve_acle.h"

#pragma GCC target "+sve2p2"
#ifdef STREAMING_COMPATIBLE
#pragma GCC target "+sme2p2"
#endif

/*
** rsqrte_u32_z_tied1:
**	ursqrte	z0\.s, p0/z, z0\.s
**	ret
*/
TEST_UNIFORM_Z (rsqrte_u32_z_tied1, svuint32_t,
		z0 = svrsqrte_u32_z (p0, z0),
		z0 = svrsqrte_z (p0, z0))

/*
** rsqrte_u32_z_untied:
**	ursqrte	z0\.s, p0/z, z1\.s
**	ret
*/
TEST_UNIFORM_Z (rsqrte_u32_z_untied, svuint32_t,
		z0 = svrsqrte_u32_z (p0, z1),
		z0 = svrsqrte_z (p0, z1))
