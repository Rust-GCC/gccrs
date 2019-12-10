/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" } } */

#include "test_sve_acle.h"

/*
** ldff1ub_s32_base:
**	ldff1b	z0\.s, p0/z, \[x0\]
**	ret
*/
TEST_LOAD (ldff1ub_s32_base, svint32_t, uint8_t,
	   z0 = svldff1ub_s32 (p0, x0),
	   z0 = svldff1ub_s32 (p0, x0))

/*
** ldff1ub_s32_index:
**	ldff1b	z0\.s, p0/z, \[x0, x1\]
**	ret
*/
TEST_LOAD (ldff1ub_s32_index, svint32_t, uint8_t,
	   z0 = svldff1ub_s32 (p0, x0 + x1),
	   z0 = svldff1ub_s32 (p0, x0 + x1))

/* Moving the constant into a register would also be OK.  */
/*
** ldff1ub_s32_1:
**	incw	x0
**	ldff1b	z0\.s, p0/z, \[x0\]
**	ret
*/
TEST_LOAD (ldff1ub_s32_1, svint32_t, uint8_t,
	   z0 = svldff1ub_s32 (p0, x0 + svcntw ()),
	   z0 = svldff1ub_s32 (p0, x0 + svcntw ()))

/* Moving the constant into a register would also be OK.  */
/*
** ldff1ub_s32_m1:
**	decw	x0
**	ldff1b	z0\.s, p0/z, \[x0\]
**	ret
*/
TEST_LOAD (ldff1ub_s32_m1, svint32_t, uint8_t,
	   z0 = svldff1ub_s32 (p0, x0 - svcntw ()),
	   z0 = svldff1ub_s32 (p0, x0 - svcntw ()))

/*
** ldff1ub_vnum_s32_0:
**	ldff1b	z0\.s, p0/z, \[x0\]
**	ret
*/
TEST_LOAD (ldff1ub_vnum_s32_0, svint32_t, uint8_t,
	   z0 = svldff1ub_vnum_s32 (p0, x0, 0),
	   z0 = svldff1ub_vnum_s32 (p0, x0, 0))

/* Moving the constant into a register would also be OK.  */
/*
** ldff1ub_vnum_s32_1:
**	incw	x0
**	ldff1b	z0\.s, p0/z, \[x0\]
**	ret
*/
TEST_LOAD (ldff1ub_vnum_s32_1, svint32_t, uint8_t,
	   z0 = svldff1ub_vnum_s32 (p0, x0, 1),
	   z0 = svldff1ub_vnum_s32 (p0, x0, 1))

/* Moving the constant into a register would also be OK.  */
/*
** ldff1ub_vnum_s32_m1:
**	decw	x0
**	ldff1b	z0\.s, p0/z, \[x0\]
**	ret
*/
TEST_LOAD (ldff1ub_vnum_s32_m1, svint32_t, uint8_t,
	   z0 = svldff1ub_vnum_s32 (p0, x0, -1),
	   z0 = svldff1ub_vnum_s32 (p0, x0, -1))

/*
** ldff1ub_vnum_s32_x1:
**	cntw	(x[0-9]+)
** (
**	madd	(x[0-9]+), (?:x1, \1|\1, x1), x0
**	ldff1b	z0\.s, p0/z, \[\2\]
** |
**	mul	(x[0-9]+), (?:x1, \1|\1, x1)
**	ldff1b	z0\.s, p0/z, \[x0, \3\]
** )
**	ret
*/
TEST_LOAD (ldff1ub_vnum_s32_x1, svint32_t, uint8_t,
	   z0 = svldff1ub_vnum_s32 (p0, x0, x1),
	   z0 = svldff1ub_vnum_s32 (p0, x0, x1))
