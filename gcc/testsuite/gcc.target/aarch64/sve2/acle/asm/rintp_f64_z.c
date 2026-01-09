/* { dg-do assemble { target aarch64_asm_sve2p2_ok } } */
/* { dg-do compile { target { ! aarch64_asm_sve2p2_ok } } } */
/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" } } */

#include "test_sve_acle.h"

#pragma GCC target "+sve2p2"
#ifdef STREAMING_COMPATIBLE
#pragma GCC target "+sme2p2"
#endif

/*
** rintp_f64_z_tied1:
**	mov	(z[0-9]+\.d), z0\.d
**	frintp	z0\.d, p0/z, \1
**	ret
*/
TEST_UNIFORM_Z (rintp_f64_z_tied1, svfloat64_t,
		z0 = svrintp_f64_z (p0, z0),
		z0 = svrintp_z (p0, z0))

/*
** rintp_f64_z_untied:
**	frintp	z0\.d, p0/z, z1\.d
**	ret
*/
TEST_UNIFORM_Z (rintp_f64_z_untied, svfloat64_t,
		z0 = svrintp_f64_z (p0, z1),
		z0 = svrintp_z (p0, z1))
