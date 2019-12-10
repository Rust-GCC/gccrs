/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" } } */

#include "test_sve_acle.h"

/*
** ldff1_f32_base:
**	ldff1w	z0\.s, p0/z, \[x0\]
**	ret
*/
TEST_LOAD (ldff1_f32_base, svfloat32_t, float32_t,
	   z0 = svldff1_f32 (p0, x0),
	   z0 = svldff1 (p0, x0))

/*
** ldff1_f32_index:
**	ldff1w	z0\.s, p0/z, \[x0, x1, lsl 2\]
**	ret
*/
TEST_LOAD (ldff1_f32_index, svfloat32_t, float32_t,
	   z0 = svldff1_f32 (p0, x0 + x1),
	   z0 = svldff1 (p0, x0 + x1))

/* Moving the constant into a register would also be OK.  */
/*
** ldff1_f32_1:
**	incb	x0
**	ldff1w	z0\.s, p0/z, \[x0\]
**	ret
*/
TEST_LOAD (ldff1_f32_1, svfloat32_t, float32_t,
	   z0 = svldff1_f32 (p0, x0 + svcntw ()),
	   z0 = svldff1 (p0, x0 + svcntw ()))

/* Moving the constant into a register would also be OK.  */
/*
** ldff1_f32_m1:
**	decb	x0
**	ldff1w	z0\.s, p0/z, \[x0\]
**	ret
*/
TEST_LOAD (ldff1_f32_m1, svfloat32_t, float32_t,
	   z0 = svldff1_f32 (p0, x0 - svcntw ()),
	   z0 = svldff1 (p0, x0 - svcntw ()))

/*
** ldff1_vnum_f32_0:
**	ldff1w	z0\.s, p0/z, \[x0\]
**	ret
*/
TEST_LOAD (ldff1_vnum_f32_0, svfloat32_t, float32_t,
	   z0 = svldff1_vnum_f32 (p0, x0, 0),
	   z0 = svldff1_vnum (p0, x0, 0))

/* Moving the constant into a register would also be OK.  */
/*
** ldff1_vnum_f32_1:
**	incb	x0
**	ldff1w	z0\.s, p0/z, \[x0\]
**	ret
*/
TEST_LOAD (ldff1_vnum_f32_1, svfloat32_t, float32_t,
	   z0 = svldff1_vnum_f32 (p0, x0, 1),
	   z0 = svldff1_vnum (p0, x0, 1))

/* Moving the constant into a register would also be OK.  */
/*
** ldff1_vnum_f32_m1:
**	decb	x0
**	ldff1w	z0\.s, p0/z, \[x0\]
**	ret
*/
TEST_LOAD (ldff1_vnum_f32_m1, svfloat32_t, float32_t,
	   z0 = svldff1_vnum_f32 (p0, x0, -1),
	   z0 = svldff1_vnum (p0, x0, -1))

/* Using MUL to calculate an index would also be OK.  */
/*
** ldff1_vnum_f32_x1:
**	cntb	(x[0-9]+)
**	madd	(x[0-9]+), (x1, \1|\1, x1), x0
**	ldff1w	z0\.s, p0/z, \[\2\]
**	ret
*/
TEST_LOAD (ldff1_vnum_f32_x1, svfloat32_t, float32_t,
	   z0 = svldff1_vnum_f32 (p0, x0, x1),
	   z0 = svldff1_vnum (p0, x0, x1))
