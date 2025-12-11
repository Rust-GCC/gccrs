/* { dg-do assemble { target aarch64_asm_sve2p2_ok } } */
/* { dg-do compile { target { ! aarch64_asm_sve2p2_ok } } } */
/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" } } */

#include "test_sve_acle.h"

#pragma GCC target "+sve2p2"
#ifdef STREAMING_COMPATIBLE
#pragma GCC target "+sme2p2"
#endif

/*
** rint64z_f32_m_tied1:
**	frint64z	z0\.s, p0/m, z4\.s
**	ret
*/
TEST_DUAL_Z (rint64z_f32_m_tied1, svfloat32_t, svfloat32_t,
	     z0 = svrint64z_f32_m (z0, p0, z4),
	     z0 = svrint64z_m (z0, p0, z4))

/*
** rint64z_f32_m_untied:
**	movprfx	z0, z1
**	frint64z	z0\.s, p0/m, z4\.s
**	ret
*/
TEST_DUAL_Z (rint64z_f32_m_untied, svfloat32_t, svfloat32_t,
	     z0 = svrint64z_f32_m (z1, p0, z4),
	     z0 = svrint64z_m (z1, p0, z4))

/*
** rint64z_f32_z:
**	frint64z	z0\.s, p0/z, z4\.s
**	ret
*/
TEST_DUAL_Z (rint64z_f32_z, svfloat32_t, svfloat32_t,
	     z0 = svrint64z_f32_z (p0, z4),
	     z0 = svrint64z_z (p0, z4))

/*
** rint64z_f32_x:
**	movprfx	z0, z4
**	frint64z	z0\.s, p0/m, z4\.s
**	ret
*/
TEST_DUAL_Z (rint64z_f32_x, svfloat32_t, svfloat32_t,
	     z0 = svrint64z_f32_x (p0, z4),
	     z0 = svrint64z_x (p0, z4))

/*
** ptrue_rint64z_f32_x:
**	...
**	ptrue	p[0-9]+\.b[^\n]*
**	...
**	ret
*/
TEST_DUAL_Z (ptrue_rint64z_f32_x, svfloat32_t, svfloat32_t,
	     z0 = svrint64z_f32_x (svptrue_b32 (), z4),
	     z0 = svrint64z_x (svptrue_b32 (), z4))
