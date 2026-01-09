/* { dg-do assemble { target aarch64_asm_sve2p2_ok } } */
/* { dg-do compile { target { ! aarch64_asm_sve2p2_ok } } } */
/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" } } */

#include "test_sve_acle.h"

#pragma GCC target "+sve2p2"
#ifdef STREAMING_COMPATIBLE
#pragma GCC target "+sme2p2"
#endif

/*
** cnot_s64_z_tied1:
**	mov	(z[0-9]+)\.d, z0\.d
**	cnot	z0\.d, p0/z, \1\.d
**	ret
*/
TEST_UNIFORM_Z (cnot_s64_z_tied1, svint64_t,
		z0 = svcnot_s64_z (p0, z0),
		z0 = svcnot_z (p0, z0))

/*
** cnot_s64_z_untied:
**	cnot	z0\.d, p0/z, z1\.d
**	ret
*/
TEST_UNIFORM_Z (cnot_s64_z_untied, svint64_t,
		z0 = svcnot_s64_z (p0, z1),
		z0 = svcnot_z (p0, z1))
