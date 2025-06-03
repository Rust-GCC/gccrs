/* { dg-do assemble { target aarch64_asm_sve2p1_ok } } */
/* { dg-do compile { target { ! aarch64_asm_sve2p1_ok } } } */
/* { dg-skip-if "" { *-*-* } { "-DSTREAMING_COMPATIBLE" } { "" } } */
/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" } } */

#include "test_sve_acle.h"

#pragma GCC target "+sve2p1"

/*
** tbx_f64_tied1:
**	tbx	z0\.d, z1\.d, z4\.d
**	ret
*/
TEST_DUAL_Z (tbx_f64_tied1, svfloat64_t, svuint64_t,
	     z0 = svtbx_f64 (z0, z1, z4),
	     z0 = svtbx (z0, z1, z4))

/* Bad RA choice: no preferred output sequence.  */
TEST_DUAL_Z (tbx_f64_tied2, svfloat64_t, svuint64_t,
	     z0 = svtbx_f64 (z1, z0, z4),
	     z0 = svtbx (z1, z0, z4))

/* Bad RA choice: no preferred output sequence.  */
TEST_DUAL_Z_REV (tbx_f64_tied3, svfloat64_t, svuint64_t,
		 z0_res = svtbx_f64 (z4, z5, z0),
		 z0_res = svtbx (z4, z5, z0))

/*
** tbx_f64_untied:
** (
**	mov	z0\.d, z1\.d
**	tbx	z0\.d, z2\.d, z4\.d
** |
**	tbx	z1\.d, z2\.d, z4\.d
**	mov	z0\.d, z1\.d
** )
**	ret
*/
TEST_DUAL_Z (tbx_f64_untied, svfloat64_t, svuint64_t,
	     z0 = svtbx_f64 (z1, z2, z4),
	     z0 = svtbx (z1, z2, z4))
