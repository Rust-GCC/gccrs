/* { dg-do assemble { target aarch64_asm_sve2p1_ok } } */
/* { dg-do compile { target { ! aarch64_asm_sve2p1_ok } } } */
/* { dg-skip-if "" { *-*-* } { "-DSTREAMING_COMPATIBLE" } { "" } } */
/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" } } */

#include "test_sve_acle.h"

#pragma GCC target "+sve2p1"

/*
** st4q_s64_base:
**	st4q	{z0\.q - z3\.q}, p0, \[x0\]
**	ret
*/
TEST_STORE (st4q_s64_base, svint64x4_t, int64_t,
	    svst4q_s64 (p0, x0, z0),
	    svst4q (p0, x0, z0))

/*
** st4q_s64_index:
**	add	(x[0-9]), x0, x1, lsl #?3
**	st4q	{z0\.q - z3\.q}, p0, \[\1\]
**	ret
*/
TEST_STORE (st4q_s64_index, svint64x4_t, int64_t,
	    svst4q_s64 (p0, x0 + x1, z0),
	    svst4q (p0, x0 + x1, z0))

/*
** st4q_s64_index2:
**	st4q	{z0\.q - z3\.q}, p0, \[x0, x1, lsl #?4\]
**	ret
*/
TEST_STORE (st4q_s64_index2, svint64x4_t, int64_t,
	    svst4q_s64 (p0, x0 + x1 * 2, z0),
	    svst4q (p0, x0 + x1 * 2, z0))

/* Moving the constant into a register would also be OK.  */
/*
** st4q_s64_1:
**	incb	x0
**	st4q	{z0\.q - z3\.q}, p0, \[x0\]
**	ret
*/
TEST_STORE (st4q_s64_1, svint64x4_t, int64_t,
	    svst4q_s64 (p0, x0 + svcntd (), z0),
	    svst4q (p0, x0 + svcntd (), z0))

/* Moving the constant into a register would also be OK.  */
/*
** st4q_s64_2:
**	incb	x0, all, mul #2
**	st4q	{z0\.q - z3\.q}, p0, \[x0\]
**	ret
*/
TEST_STORE (st4q_s64_2, svint64x4_t, int64_t,
	    svst4q_s64 (p0, x0 + svcntd () * 2, z0),
	    svst4q (p0, x0 + svcntd () * 2, z0))

/* Moving the constant into a register would also be OK.  */
/*
** st4q_s64_3:
**	incb	x0, all, mul #3
**	st4q	{z0\.q - z3\.q}, p0, \[x0\]
**	ret
*/
TEST_STORE (st4q_s64_3, svint64x4_t, int64_t,
	    svst4q_s64 (p0, x0 + svcntd () * 3, z0),
	    svst4q (p0, x0 + svcntd () * 3, z0))

/*
** st4q_s64_4:
**	st4q	{z0\.q - z3\.q}, p0, \[x0, #4, mul vl\]
**	ret
*/
TEST_STORE (st4q_s64_4, svint64x4_t, int64_t,
	    svst4q_s64 (p0, x0 + svcntd () * 4, z0),
	    svst4q (p0, x0 + svcntd () * 4, z0))

/*
** st4q_s64_28:
**	st4q	{z0\.q - z3\.q}, p0, \[x0, #28, mul vl\]
**	ret
*/
TEST_STORE (st4q_s64_28, svint64x4_t, int64_t,
	    svst4q_s64 (p0, x0 + svcntd () * 28, z0),
	    svst4q (p0, x0 + svcntd () * 28, z0))

/*
** st4q_s64_32:
**	[^{]*
**	st4q	{z0\.q - z3\.q}, p0, \[x[0-9]+\]
**	ret
*/
TEST_STORE (st4q_s64_32, svint64x4_t, int64_t,
	    svst4q_s64 (p0, x0 + svcntd () * 32, z0),
	    svst4q (p0, x0 + svcntd () * 32, z0))

/* Moving the constant into a register would also be OK.  */
/*
** st4q_s64_m1:
**	decb	x0
**	st4q	{z0\.q - z3\.q}, p0, \[x0\]
**	ret
*/
TEST_STORE (st4q_s64_m1, svint64x4_t, int64_t,
	    svst4q_s64 (p0, x0 - svcntd (), z0),
	    svst4q (p0, x0 - svcntd (), z0))

/* Moving the constant into a register would also be OK.  */
/*
** st4q_s64_m2:
**	decb	x0, all, mul #2
**	st4q	{z0\.q - z3\.q}, p0, \[x0\]
**	ret
*/
TEST_STORE (st4q_s64_m2, svint64x4_t, int64_t,
	    svst4q_s64 (p0, x0 - svcntd () * 2, z0),
	    svst4q (p0, x0 - svcntd () * 2, z0))

/* Moving the constant into a register would also be OK.  */
/*
** st4q_s64_m3:
**	decb	x0, all, mul #3
**	st4q	{z0\.q - z3\.q}, p0, \[x0\]
**	ret
*/
TEST_STORE (st4q_s64_m3, svint64x4_t, int64_t,
	    svst4q_s64 (p0, x0 - svcntd () * 3, z0),
	    svst4q (p0, x0 - svcntd () * 3, z0))

/*
** st4q_s64_m4:
**	st4q	{z0\.q - z3\.q}, p0, \[x0, #-4, mul vl\]
**	ret
*/
TEST_STORE (st4q_s64_m4, svint64x4_t, int64_t,
	    svst4q_s64 (p0, x0 - svcntd () * 4, z0),
	    svst4q (p0, x0 - svcntd () * 4, z0))

/*
** st4q_s64_m32:
**	st4q	{z0\.q - z3\.q}, p0, \[x0, #-32, mul vl\]
**	ret
*/
TEST_STORE (st4q_s64_m32, svint64x4_t, int64_t,
	    svst4q_s64 (p0, x0 - svcntd () * 32, z0),
	    svst4q (p0, x0 - svcntd () * 32, z0))

/*
** st4q_s64_m36:
**	[^{]*
**	st4q	{z0\.q - z3\.q}, p0, \[x[0-9]+\]
**	ret
*/
TEST_STORE (st4q_s64_m36, svint64x4_t, int64_t,
	    svst4q_s64 (p0, x0 - svcntd () * 36, z0),
	    svst4q (p0, x0 - svcntd () * 36, z0))

/*
** st4q_vnum_s64_0:
**	st4q	{z0\.q - z3\.q}, p0, \[x0\]
**	ret
*/
TEST_STORE (st4q_vnum_s64_0, svint64x4_t, int64_t,
	    svst4q_vnum_s64 (p0, x0, 0, z0),
	    svst4q_vnum (p0, x0, 0, z0))

/* Moving the constant into a register would also be OK.  */
/*
** st4q_vnum_s64_1:
**	incb	x0
**	st4q	{z0\.q - z3\.q}, p0, \[x0\]
**	ret
*/
TEST_STORE (st4q_vnum_s64_1, svint64x4_t, int64_t,
	    svst4q_vnum_s64 (p0, x0, 1, z0),
	    svst4q_vnum (p0, x0, 1, z0))

/* Moving the constant into a register would also be OK.  */
/*
** st4q_vnum_s64_2:
**	incb	x0, all, mul #2
**	st4q	{z0\.q - z3\.q}, p0, \[x0\]
**	ret
*/
TEST_STORE (st4q_vnum_s64_2, svint64x4_t, int64_t,
	    svst4q_vnum_s64 (p0, x0, 2, z0),
	    svst4q_vnum (p0, x0, 2, z0))

/* Moving the constant into a register would also be OK.  */
/*
** st4q_vnum_s64_3:
**	incb	x0, all, mul #3
**	st4q	{z0\.q - z3\.q}, p0, \[x0\]
**	ret
*/
TEST_STORE (st4q_vnum_s64_3, svint64x4_t, int64_t,
	    svst4q_vnum_s64 (p0, x0, 3, z0),
	    svst4q_vnum (p0, x0, 3, z0))

/*
** st4q_vnum_s64_4:
**	st4q	{z0\.q - z3\.q}, p0, \[x0, #4, mul vl\]
**	ret
*/
TEST_STORE (st4q_vnum_s64_4, svint64x4_t, int64_t,
	    svst4q_vnum_s64 (p0, x0, 4, z0),
	    svst4q_vnum (p0, x0, 4, z0))

/*
** st4q_vnum_s64_28:
**	st4q	{z0\.q - z3\.q}, p0, \[x0, #28, mul vl\]
**	ret
*/
TEST_STORE (st4q_vnum_s64_28, svint64x4_t, int64_t,
	    svst4q_vnum_s64 (p0, x0, 28, z0),
	    svst4q_vnum (p0, x0, 28, z0))

/*
** st4q_vnum_s64_32:
**	[^{]*
**	st4q	{z0\.q - z3\.q}, p0, \[x[0-9]+\]
**	ret
*/
TEST_STORE (st4q_vnum_s64_32, svint64x4_t, int64_t,
	    svst4q_vnum_s64 (p0, x0, 32, z0),
	    svst4q_vnum (p0, x0, 32, z0))

/* Moving the constant into a register would also be OK.  */
/*
** st4q_vnum_s64_m1:
**	decb	x0
**	st4q	{z0\.q - z3\.q}, p0, \[x0\]
**	ret
*/
TEST_STORE (st4q_vnum_s64_m1, svint64x4_t, int64_t,
	    svst4q_vnum_s64 (p0, x0, -1, z0),
	    svst4q_vnum (p0, x0, -1, z0))

/* Moving the constant into a register would also be OK.  */
/*
** st4q_vnum_s64_m2:
**	decb	x0, all, mul #2
**	st4q	{z0\.q - z3\.q}, p0, \[x0\]
**	ret
*/
TEST_STORE (st4q_vnum_s64_m2, svint64x4_t, int64_t,
	    svst4q_vnum_s64 (p0, x0, -2, z0),
	    svst4q_vnum (p0, x0, -2, z0))

/* Moving the constant into a register would also be OK.  */
/*
** st4q_vnum_s64_m3:
**	decb	x0, all, mul #3
**	st4q	{z0\.q - z3\.q}, p0, \[x0\]
**	ret
*/
TEST_STORE (st4q_vnum_s64_m3, svint64x4_t, int64_t,
	    svst4q_vnum_s64 (p0, x0, -3, z0),
	    svst4q_vnum (p0, x0, -3, z0))

/*
** st4q_vnum_s64_m4:
**	st4q	{z0\.q - z3\.q}, p0, \[x0, #-4, mul vl\]
**	ret
*/
TEST_STORE (st4q_vnum_s64_m4, svint64x4_t, int64_t,
	    svst4q_vnum_s64 (p0, x0, -4, z0),
	    svst4q_vnum (p0, x0, -4, z0))

/*
** st4q_vnum_s64_m32:
**	st4q	{z0\.q - z3\.q}, p0, \[x0, #-32, mul vl\]
**	ret
*/
TEST_STORE (st4q_vnum_s64_m32, svint64x4_t, int64_t,
	    svst4q_vnum_s64 (p0, x0, -32, z0),
	    svst4q_vnum (p0, x0, -32, z0))

/*
** st4q_vnum_s64_m36:
**	[^{]*
**	st4q	{z0\.q - z3\.q}, p0, \[x[0-9]+\]
**	ret
*/
TEST_STORE (st4q_vnum_s64_m36, svint64x4_t, int64_t,
	    svst4q_vnum_s64 (p0, x0, -36, z0),
	    svst4q_vnum (p0, x0, -36, z0))

/*
** st4q_vnum_s64_x1:
**	cntb	(x[0-9]+)
** (
**	madd	(x[0-9]+), (?:x1, \1|\1, x1), x0
**	st4q	{z0\.q - z3\.q}, p0, \[\2\]
** |
**	mul	(x[0-9]+), (?:x1, \1|\1, x1)
**	st4q	{z0\.q - z3\.q}, p0, \[x0, \3\]
** )
**	ret
*/
TEST_STORE (st4q_vnum_s64_x1, svint64x4_t, int64_t,
	    svst4q_vnum_s64 (p0, x0, x1, z0),
	    svst4q_vnum (p0, x0, x1, z0))
