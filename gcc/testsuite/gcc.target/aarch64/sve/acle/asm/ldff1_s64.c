/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" } } */

#include "test_sve_acle.h"

/*
** ldff1_s64_base:
**	ldff1d	z0\.d, p0/z, \[x0\]
**	ret
*/
TEST_LOAD (ldff1_s64_base, svint64_t, int64_t,
	   z0 = svldff1_s64 (p0, x0),
	   z0 = svldff1 (p0, x0))

/*
** ldff1_s64_index:
**	ldff1d	z0\.d, p0/z, \[x0, x1, lsl 3\]
**	ret
*/
TEST_LOAD (ldff1_s64_index, svint64_t, int64_t,
	   z0 = svldff1_s64 (p0, x0 + x1),
	   z0 = svldff1 (p0, x0 + x1))

/* Moving the constant into a register would also be OK.  */
/*
** ldff1_s64_1:
**	incb	x0
**	ldff1d	z0\.d, p0/z, \[x0\]
**	ret
*/
TEST_LOAD (ldff1_s64_1, svint64_t, int64_t,
	   z0 = svldff1_s64 (p0, x0 + svcntd ()),
	   z0 = svldff1 (p0, x0 + svcntd ()))

/* Moving the constant into a register would also be OK.  */
/*
** ldff1_s64_m1:
**	decb	x0
**	ldff1d	z0\.d, p0/z, \[x0\]
**	ret
*/
TEST_LOAD (ldff1_s64_m1, svint64_t, int64_t,
	   z0 = svldff1_s64 (p0, x0 - svcntd ()),
	   z0 = svldff1 (p0, x0 - svcntd ()))

/*
** ldff1_vnum_s64_0:
**	ldff1d	z0\.d, p0/z, \[x0\]
**	ret
*/
TEST_LOAD (ldff1_vnum_s64_0, svint64_t, int64_t,
	   z0 = svldff1_vnum_s64 (p0, x0, 0),
	   z0 = svldff1_vnum (p0, x0, 0))

/* Moving the constant into a register would also be OK.  */
/*
** ldff1_vnum_s64_1:
**	incb	x0
**	ldff1d	z0\.d, p0/z, \[x0\]
**	ret
*/
TEST_LOAD (ldff1_vnum_s64_1, svint64_t, int64_t,
	   z0 = svldff1_vnum_s64 (p0, x0, 1),
	   z0 = svldff1_vnum (p0, x0, 1))

/* Moving the constant into a register would also be OK.  */
/*
** ldff1_vnum_s64_m1:
**	decb	x0
**	ldff1d	z0\.d, p0/z, \[x0\]
**	ret
*/
TEST_LOAD (ldff1_vnum_s64_m1, svint64_t, int64_t,
	   z0 = svldff1_vnum_s64 (p0, x0, -1),
	   z0 = svldff1_vnum (p0, x0, -1))

/* Using MUL to calculate an index would also be OK.  */
/*
** ldff1_vnum_s64_x1:
**	cntb	(x[0-9]+)
**	madd	(x[0-9]+), (x1, \1|\1, x1), x0
**	ldff1d	z0\.d, p0/z, \[\2\]
**	ret
*/
TEST_LOAD (ldff1_vnum_s64_x1, svint64_t, int64_t,
	   z0 = svldff1_vnum_s64 (p0, x0, x1),
	   z0 = svldff1_vnum (p0, x0, x1))
