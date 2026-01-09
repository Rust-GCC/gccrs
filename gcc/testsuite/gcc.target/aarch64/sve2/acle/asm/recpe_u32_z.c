/* { dg-do assemble { target aarch64_asm_sve2p2_ok } } */
/* { dg-do compile { target { ! aarch64_asm_sve2p2_ok } } } */
/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" } } */                                       
#include "test_sve_acle.h"
#pragma GCC target "+sve2p2"
#ifdef STREAMING_COMPATIBLE
#pragma GCC target "+sme2p2"
#endif

/*
** recpe_u32_z_tied1:
**	urecpe	z0\.s, p0/z, z0\.s
**	ret
*/
TEST_UNIFORM_Z (recpe_u32_z_tied1, svuint32_t,
		z0 = svrecpe_u32_z (p0, z0),
		z0 = svrecpe_z (p0, z0))

/*
** recpe_u32_z_untied:
**	urecpe	z0\.s, p0/z, z1\.s
**	ret
*/
TEST_UNIFORM_Z (recpe_u32_z_untied, svuint32_t,
		z0 = svrecpe_u32_z (p0, z1),
		z0 = svrecpe_z (p0, z1))
