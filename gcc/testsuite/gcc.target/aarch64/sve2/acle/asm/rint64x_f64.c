/* { dg-do assemble { target aarch64_asm_sve2p2_ok } } */
/* { dg-do compile { target { ! aarch64_asm_sve2p2_ok } } } */
/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" } } */

#include "test_sve_acle.h"

#pragma GCC target "+sve2p2"
#ifdef STREAMING_COMPATIBLE
#pragma GCC target "+sme2p2"
#endif

/*
** rint64x_f64_m_tied1:
**	frint64x	z0\.d, p0/m, z4\.d
**	ret
*/
TEST_DUAL_Z (rint64x_f64_m_tied1, svfloat64_t, svfloat64_t,
	     z0 = svrint64x_f64_m (z0, p0, z4),
	     z0 = svrint64x_m (z0, p0, z4))

/*
** rint64x_f64_m_untied:
**	movprfx	z0, z1
**	frint64x	z0\.d, p0/m, z4\.d
**	ret
*/
TEST_DUAL_Z (rint64x_f64_m_untied, svfloat64_t, svfloat64_t,
	     z0 = svrint64x_f64_m (z1, p0, z4),
	     z0 = svrint64x_m (z1, p0, z4))

/*
** rint64x_f64_z:
**	frint64x	z0\.d, p0/z, z4\.d
**	ret
*/
TEST_DUAL_Z (rint64x_f64_z, svfloat64_t, svfloat64_t,
	     z0 = svrint64x_f64_z (p0, z4),
	     z0 = svrint64x_z (p0, z4))

/*
** rint64x_f64_x:
**	movprfx	z0, z4
**	frint64x	z0\.d, p0/m, z4\.d
**	ret
*/
TEST_DUAL_Z (rint64x_f64_x, svfloat64_t, svfloat64_t,
	     z0 = svrint64x_f64_x (p0, z4),
	     z0 = svrint64x_x (p0, z4))

/*
** ptrue_rint64x_f64_x:
**	...
**	ptrue	p[0-9]+\.b[^\n]*
**	...
**	ret
*/
TEST_DUAL_Z (ptrue_rint64x_f64_x, svfloat64_t, svfloat64_t,
	     z0 = svrint64x_f64_x (svptrue_b64 (), z4),
	     z0 = svrint64x_x (svptrue_b64 (), z4))
