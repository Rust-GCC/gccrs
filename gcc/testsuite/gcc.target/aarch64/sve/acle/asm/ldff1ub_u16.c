/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" } } */

#include "test_sve_acle.h"

/*
** ldff1ub_u16_base:
**	ldff1b	z0\.h, p0/z, \[x0\]
**	ret
*/
TEST_LOAD (ldff1ub_u16_base, svuint16_t, uint8_t,
	   z0 = svldff1ub_u16 (p0, x0),
	   z0 = svldff1ub_u16 (p0, x0))

/*
** ldff1ub_u16_index:
**	ldff1b	z0\.h, p0/z, \[x0, x1\]
**	ret
*/
TEST_LOAD (ldff1ub_u16_index, svuint16_t, uint8_t,
	   z0 = svldff1ub_u16 (p0, x0 + x1),
	   z0 = svldff1ub_u16 (p0, x0 + x1))

/* Moving the constant into a register would also be OK.  */
/*
** ldff1ub_u16_1:
**	inch	x0
**	ldff1b	z0\.h, p0/z, \[x0\]
**	ret
*/
TEST_LOAD (ldff1ub_u16_1, svuint16_t, uint8_t,
	   z0 = svldff1ub_u16 (p0, x0 + svcnth ()),
	   z0 = svldff1ub_u16 (p0, x0 + svcnth ()))

/* Moving the constant into a register would also be OK.  */
/*
** ldff1ub_u16_m1:
**	dech	x0
**	ldff1b	z0\.h, p0/z, \[x0\]
**	ret
*/
TEST_LOAD (ldff1ub_u16_m1, svuint16_t, uint8_t,
	   z0 = svldff1ub_u16 (p0, x0 - svcnth ()),
	   z0 = svldff1ub_u16 (p0, x0 - svcnth ()))

/*
** ldff1ub_vnum_u16_0:
**	ldff1b	z0\.h, p0/z, \[x0\]
**	ret
*/
TEST_LOAD (ldff1ub_vnum_u16_0, svuint16_t, uint8_t,
	   z0 = svldff1ub_vnum_u16 (p0, x0, 0),
	   z0 = svldff1ub_vnum_u16 (p0, x0, 0))

/* Moving the constant into a register would also be OK.  */
/*
** ldff1ub_vnum_u16_1:
**	inch	x0
**	ldff1b	z0\.h, p0/z, \[x0\]
**	ret
*/
TEST_LOAD (ldff1ub_vnum_u16_1, svuint16_t, uint8_t,
	   z0 = svldff1ub_vnum_u16 (p0, x0, 1),
	   z0 = svldff1ub_vnum_u16 (p0, x0, 1))

/* Moving the constant into a register would also be OK.  */
/*
** ldff1ub_vnum_u16_m1:
**	dech	x0
**	ldff1b	z0\.h, p0/z, \[x0\]
**	ret
*/
TEST_LOAD (ldff1ub_vnum_u16_m1, svuint16_t, uint8_t,
	   z0 = svldff1ub_vnum_u16 (p0, x0, -1),
	   z0 = svldff1ub_vnum_u16 (p0, x0, -1))

/*
** ldff1ub_vnum_u16_x1:
**	cnth	(x[0-9]+)
** (
**	madd	(x[0-9]+), (?:x1, \1|\1, x1), x0
**	ldff1b	z0\.h, p0/z, \[\2\]
** |
**	mul	(x[0-9]+), (?:x1, \1|\1, x1)
**	ldff1b	z0\.h, p0/z, \[x0, \3\]
** )
**	ret
*/
TEST_LOAD (ldff1ub_vnum_u16_x1, svuint16_t, uint8_t,
	   z0 = svldff1ub_vnum_u16 (p0, x0, x1),
	   z0 = svldff1ub_vnum_u16 (p0, x0, x1))
