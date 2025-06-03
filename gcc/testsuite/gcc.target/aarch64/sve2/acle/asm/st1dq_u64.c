/* { dg-do assemble { target aarch64_asm_sve2p1_ok } } */
/* { dg-do compile { target { ! aarch64_asm_sve2p1_ok } } } */
/* { dg-skip-if "" { *-*-* } { "-DSTREAMING_COMPATIBLE" } { "" } } */
/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" } } */

#include "test_sve_acle.h"

#pragma GCC target "+sve2p1"

/*
** st1dq_u64_base:
**	st1d	{z0\.q}, p0, \[x0\]
**	ret
*/
TEST_STORE (st1dq_u64_base, svuint64_t, uint64_t,
	    svst1dq_u64 (p0, x0, z0),
	    svst1dq (p0, x0, z0))

/*
** st1dq_u64_index:
**	st1d	{z0\.q}, p0, \[x0, x1, lsl 3\]
**	ret
*/
TEST_STORE (st1dq_u64_index, svuint64_t, uint64_t,
	    svst1dq_u64 (p0, x0 + x1, z0),
	    svst1dq (p0, x0 + x1, z0))

/*
** st1dq_u64_1:
**	st1d	{z0\.q}, p0, \[x0, #2, mul vl\]
**	ret
*/
TEST_STORE (st1dq_u64_1, svuint64_t, uint64_t,
	    svst1dq_u64 (p0, x0 + svcntd (), z0),
	    svst1dq (p0, x0 + svcntd (), z0))

/*
** st1dq_u64_7:
**	st1d	{z0\.q}, p0, \[x0, #6, mul vl\]
**	ret
*/
TEST_STORE (st1dq_u64_7, svuint64_t, uint64_t,
	    svst1dq_u64 (p0, x0 + svcntd () * 3, z0),
	    svst1dq (p0, x0 + svcntd () * 3, z0))

/* Moving the constant into a register would also be OK.  */
/*
** st1dq_u64_8:
**	incb	x0, all, mul #4
**	st1d	{z0\.q}, p0, \[x0\]
**	ret
*/
TEST_STORE (st1dq_u64_8, svuint64_t, uint64_t,
	    svst1dq_u64 (p0, x0 + svcntd () * 4, z0),
	    svst1dq (p0, x0 + svcntd () * 4, z0))

/*
** st1dq_u64_m2:
**	st1d	{z0\.q}, p0, \[x0, #-2, mul vl\]
**	ret
*/
TEST_STORE (st1dq_u64_m2, svuint64_t, uint64_t,
	    svst1dq_u64 (p0, x0 - svcntd (), z0),
	    svst1dq (p0, x0 - svcntd (), z0))

/*
** st1dq_u64_m8:
**	st1d	{z0\.q}, p0, \[x0, #-8, mul vl\]
**	ret
*/
TEST_STORE (st1dq_u64_m8, svuint64_t, uint64_t,
	    svst1dq_u64 (p0, x0 - svcntd () * 4, z0),
	    svst1dq (p0, x0 - svcntd () * 4, z0))

/* Moving the constant into a register would also be OK.  */
/*
** st1dq_u64_m10:
**	decb	x0, all, mul #5
**	st1d	{z0\.q}, p0, \[x0\]
**	ret
*/
TEST_STORE (st1dq_u64_m10, svuint64_t, uint64_t,
	    svst1dq_u64 (p0, x0 - svcntd () * 5, z0),
	    svst1dq (p0, x0 - svcntd () * 5, z0))

/*
** st1dq_vnum_u64_0:
**	st1d	{z0\.q}, p0, \[x0\]
**	ret
*/
TEST_STORE (st1dq_vnum_u64_0, svuint64_t, uint64_t,
	    svst1dq_vnum_u64 (p0, x0, 0, z0),
	    svst1dq_vnum (p0, x0, 0, z0))

/*
** st1dq_vnum_u64_1:
**	st1d	{z0\.q}, p0, \[x0, #1, mul vl\]
**	ret
*/
TEST_STORE (st1dq_vnum_u64_1, svuint64_t, uint64_t,
	    svst1dq_vnum_u64 (p0, x0, 1, z0),
	    svst1dq_vnum (p0, x0, 1, z0))

/*
** st1dq_vnum_u64_7:
**	st1d	{z0\.q}, p0, \[x0, #7, mul vl\]
**	ret
*/
TEST_STORE (st1dq_vnum_u64_7, svuint64_t, uint64_t,
	    svst1dq_vnum_u64 (p0, x0, 7, z0),
	    svst1dq_vnum (p0, x0, 7, z0))

/* Moving the constant into a register would also be OK.  */
/*
** st1dq_vnum_u64_8:
**	incb	x0, all, mul #4
**	st1d	{z0\.q}, p0, \[x0\]
**	ret
*/
TEST_STORE (st1dq_vnum_u64_8, svuint64_t, uint64_t,
	    svst1dq_vnum_u64 (p0, x0, 8, z0),
	    svst1dq_vnum (p0, x0, 8, z0))

/*
** st1dq_vnum_u64_m1:
**	st1d	{z0\.q}, p0, \[x0, #-1, mul vl\]
**	ret
*/
TEST_STORE (st1dq_vnum_u64_m1, svuint64_t, uint64_t,
	    svst1dq_vnum_u64 (p0, x0, -1, z0),
	    svst1dq_vnum (p0, x0, -1, z0))

/*
** st1dq_vnum_u64_m8:
**	st1d	{z0\.q}, p0, \[x0, #-8, mul vl\]
**	ret
*/
TEST_STORE (st1dq_vnum_u64_m8, svuint64_t, uint64_t,
	    svst1dq_vnum_u64 (p0, x0, -8, z0),
	    svst1dq_vnum (p0, x0, -8, z0))

/* Moving the constant into a register would also be OK.  */
/*
** st1dq_vnum_u64_m9:
**	dech	x0, all, mul #9
**	st1d	{z0\.q}, p0, \[x0\]
**	ret
*/
TEST_STORE (st1dq_vnum_u64_m9, svuint64_t, uint64_t,
	    svst1dq_vnum_u64 (p0, x0, -9, z0),
	    svst1dq_vnum (p0, x0, -9, z0))

/* Using MUL to calculate an index would also be OK.  */
/*
** st1dq_vnum_u64_x1:
**	cnth	(x[0-9]+)
**	madd	(x[0-9]+), (x1, \1|\1, x1), x0
**	st1d	{z0\.q}, p0, \[\2\]
**	ret
*/
TEST_STORE (st1dq_vnum_u64_x1, svuint64_t, uint64_t,
	    svst1dq_vnum_u64 (p0, x0, x1, z0),
	    svst1dq_vnum (p0, x0, x1, z0))
