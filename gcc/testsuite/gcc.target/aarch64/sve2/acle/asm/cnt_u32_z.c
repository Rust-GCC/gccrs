/* { dg-do assemble { target aarch64_asm_sve2p2_ok } } */
/* { dg-do compile { target { ! aarch64_asm_sve2p2_ok } } } */
/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" } } */

#include "test_sve_acle.h"

#pragma GCC target "+sve2p2"
#ifdef STREAMING_COMPATIBLE
#pragma GCC target "+sme2p2"
#endif

/*
** cnt_u32_z_tied1:
**	mov	(z[0-9]+)\.d, z0\.d
**	cnt	z0\.s, p0/z, \1\.s
**	ret
*/
TEST_UNIFORM_Z (cnt_u32_z_tied1, svuint32_t,
		z0 = svcnt_u32_z (p0, z0),
		z0 = svcnt_z (p0, z0))

/*
** cnt_u32_z_untied:
**	cnt	z0\.s, p0/z, z1\.s
**	ret
*/
TEST_UNIFORM_Z (cnt_u32_z_untied, svuint32_t,
		z0 = svcnt_u32_z (p0, z1),
		z0 = svcnt_z (p0, z1))

