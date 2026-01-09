/* { dg-do assemble { target aarch64_asm_sve2p2_ok } } */
/* { dg-do compile { target { ! aarch64_asm_sve2p2_ok } } } */
/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" } } */

#include "test_sve_acle.h"

#pragma GCC target "+sve2p2"
#ifdef STREAMING_COMPATIBLE
#pragma GCC target "+sme2p2"
#endif

/*
** rbit_u8_z_tied1:
**	mov	(z[0-9]+)\.d, z0\.d
**	rbit	z0\.b, p0/z, \1\.b
**	ret
*/
TEST_UNIFORM_Z (rbit_u8_z_tied1, svuint8_t,
		z0 = svrbit_u8_z (p0, z0),
		z0 = svrbit_z (p0, z0))

/*
** rbit_u8_z_untied:
**	rbit	z0\.b, p0/z, z1\.b
**	ret
*/
TEST_UNIFORM_Z (rbit_u8_z_untied, svuint8_t,
		z0 = svrbit_u8_z (p0, z1),
		z0 = svrbit_z (p0, z1))
