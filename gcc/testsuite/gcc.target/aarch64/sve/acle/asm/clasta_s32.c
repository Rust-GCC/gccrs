/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" } } */

#include "test_sve_acle.h"

/*
** clasta_s32_tied1:
**	clasta	z0\.s, p0, z0\.s, z1\.s
**	ret
*/
TEST_UNIFORM_Z (clasta_s32_tied1, svint32_t,
		z0 = svclasta_s32 (p0, z0, z1),
		z0 = svclasta (p0, z0, z1))

/*
** clasta_s32_tied2:
**	mov	(z[0-9]+)\.d, z0\.d
**	movprfx	z0, z1
**	clasta	z0\.s, p0, z0\.s, \1\.s
**	ret
*/
TEST_UNIFORM_Z (clasta_s32_tied2, svint32_t,
		z0 = svclasta_s32 (p0, z1, z0),
		z0 = svclasta (p0, z1, z0))

/*
** clasta_s32_untied:
**	movprfx	z0, z1
**	clasta	z0\.s, p0, z0\.s, z2\.s
**	ret
*/
TEST_UNIFORM_Z (clasta_s32_untied, svint32_t,
		z0 = svclasta_s32 (p0, z1, z2),
		z0 = svclasta (p0, z1, z2))

/*
** clasta_x0_s32:
**	clasta	w0, p0, w0, z0\.s
**	ret
*/
TEST_FOLD_LEFT_X (clasta_x0_s32, int32_t, svint32_t,
		  x0 = svclasta_n_s32 (p0, x0, z0),
		  x0 = svclasta (p0, x0, z0))

/*
** clasta_x1_s32:
**	mov	w0, w1
**	clasta	w0, p0, w0, z0\.s
**	ret
*/
TEST_FOLD_LEFT_X (clasta_x1_s32, int32_t, svint32_t,
		  x0 = svclasta_n_s32 (p0, x1, z0),
		  x0 = svclasta (p0, x1, z0))
