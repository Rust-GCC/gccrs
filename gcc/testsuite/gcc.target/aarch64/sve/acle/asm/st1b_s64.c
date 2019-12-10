/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" } } */

#include "test_sve_acle.h"

/*
** st1b_s64_base:
**	st1b	z0\.d, p0, \[x0\]
**	ret
*/
TEST_STORE (st1b_s64_base, svint64_t, int8_t,
	    svst1b_s64 (p0, x0, z0),
	    svst1b (p0, x0, z0))

/*
** st1b_s64_index:
**	st1b	z0\.d, p0, \[x0, x1\]
**	ret
*/
TEST_STORE (st1b_s64_index, svint64_t, int8_t,
	    svst1b_s64 (p0, x0 + x1, z0),
	    svst1b (p0, x0 + x1, z0))

/*
** st1b_s64_1:
**	st1b	z0\.d, p0, \[x0, #1, mul vl\]
**	ret
*/
TEST_STORE (st1b_s64_1, svint64_t, int8_t,
	    svst1b_s64 (p0, x0 + svcntd (), z0),
	    svst1b (p0, x0 + svcntd (), z0))

/*
** st1b_s64_7:
**	st1b	z0\.d, p0, \[x0, #7, mul vl\]
**	ret
*/
TEST_STORE (st1b_s64_7, svint64_t, int8_t,
	    svst1b_s64 (p0, x0 + svcntd () * 7, z0),
	    svst1b (p0, x0 + svcntd () * 7, z0))

/* Moving the constant into a register would also be OK.  */
/*
** st1b_s64_8:
**	incb	x0
**	st1b	z0\.d, p0, \[x0\]
**	ret
*/
TEST_STORE (st1b_s64_8, svint64_t, int8_t,
	    svst1b_s64 (p0, x0 + svcntd () * 8, z0),
	    svst1b (p0, x0 + svcntd () * 8, z0))

/*
** st1b_s64_m1:
**	st1b	z0\.d, p0, \[x0, #-1, mul vl\]
**	ret
*/
TEST_STORE (st1b_s64_m1, svint64_t, int8_t,
	    svst1b_s64 (p0, x0 - svcntd (), z0),
	    svst1b (p0, x0 - svcntd (), z0))

/*
** st1b_s64_m8:
**	st1b	z0\.d, p0, \[x0, #-8, mul vl\]
**	ret
*/
TEST_STORE (st1b_s64_m8, svint64_t, int8_t,
	    svst1b_s64 (p0, x0 - svcntd () * 8, z0),
	    svst1b (p0, x0 - svcntd () * 8, z0))

/* Moving the constant into a register would also be OK.  */
/*
** st1b_s64_m9:
**	decd	x0, all, mul #9
**	st1b	z0\.d, p0, \[x0\]
**	ret
*/
TEST_STORE (st1b_s64_m9, svint64_t, int8_t,
	    svst1b_s64 (p0, x0 - svcntd () * 9, z0),
	    svst1b (p0, x0 - svcntd () * 9, z0))

/*
** st1b_vnum_s64_0:
**	st1b	z0\.d, p0, \[x0\]
**	ret
*/
TEST_STORE (st1b_vnum_s64_0, svint64_t, int8_t,
	    svst1b_vnum_s64 (p0, x0, 0, z0),
	    svst1b_vnum (p0, x0, 0, z0))

/*
** st1b_vnum_s64_1:
**	st1b	z0\.d, p0, \[x0, #1, mul vl\]
**	ret
*/
TEST_STORE (st1b_vnum_s64_1, svint64_t, int8_t,
	    svst1b_vnum_s64 (p0, x0, 1, z0),
	    svst1b_vnum (p0, x0, 1, z0))

/*
** st1b_vnum_s64_7:
**	st1b	z0\.d, p0, \[x0, #7, mul vl\]
**	ret
*/
TEST_STORE (st1b_vnum_s64_7, svint64_t, int8_t,
	    svst1b_vnum_s64 (p0, x0, 7, z0),
	    svst1b_vnum (p0, x0, 7, z0))

/* Moving the constant into a register would also be OK.  */
/*
** st1b_vnum_s64_8:
**	incb	x0
**	st1b	z0\.d, p0, \[x0\]
**	ret
*/
TEST_STORE (st1b_vnum_s64_8, svint64_t, int8_t,
	    svst1b_vnum_s64 (p0, x0, 8, z0),
	    svst1b_vnum (p0, x0, 8, z0))

/*
** st1b_vnum_s64_m1:
**	st1b	z0\.d, p0, \[x0, #-1, mul vl\]
**	ret
*/
TEST_STORE (st1b_vnum_s64_m1, svint64_t, int8_t,
	    svst1b_vnum_s64 (p0, x0, -1, z0),
	    svst1b_vnum (p0, x0, -1, z0))

/*
** st1b_vnum_s64_m8:
**	st1b	z0\.d, p0, \[x0, #-8, mul vl\]
**	ret
*/
TEST_STORE (st1b_vnum_s64_m8, svint64_t, int8_t,
	    svst1b_vnum_s64 (p0, x0, -8, z0),
	    svst1b_vnum (p0, x0, -8, z0))

/* Moving the constant into a register would also be OK.  */
/*
** st1b_vnum_s64_m9:
**	decd	x0, all, mul #9
**	st1b	z0\.d, p0, \[x0\]
**	ret
*/
TEST_STORE (st1b_vnum_s64_m9, svint64_t, int8_t,
	    svst1b_vnum_s64 (p0, x0, -9, z0),
	    svst1b_vnum (p0, x0, -9, z0))

/*
** st1b_vnum_s64_x1:
**	cntd	(x[0-9]+)
** (
**	madd	(x[0-9]+), (?:x1, \1|\1, x1), x0
**	st1b	z0\.d, p0, \[\2\]
** |
**	mul	(x[0-9]+), (?:x1, \1|\1, x1)
**	st1b	z0\.d, p0, \[x0, \3\]
** )
**	ret
*/
TEST_STORE (st1b_vnum_s64_x1, svint64_t, int8_t,
	    svst1b_vnum_s64 (p0, x0, x1, z0),
	    svst1b_vnum (p0, x0, x1, z0))
