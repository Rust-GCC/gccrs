/* { dg-do assemble { target aarch64_asm_sve2p1_ok } } */
/* { dg-do compile { target { ! aarch64_asm_sve2p1_ok } } } */
/* { dg-skip-if "" { *-*-* } { "-DSTREAMING_COMPATIBLE" } { "" } } */
/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" } } */

#include "test_sve_acle.h"

#pragma GCC target "+sve2p1"

/*
** st4q_u32_base:
**	st4q	{z0\.q - z3\.q}, p0, \[x0\]
**	ret
*/
TEST_STORE (st4q_u32_base, svuint32x4_t, uint32_t,
	    svst4q_u32 (p0, x0, z0),
	    svst4q (p0, x0, z0))

/*
** st4q_u32_index:
**	add	(x[0-9]), x0, x1, lsl #?2
**	st4q	{z0\.q - z3\.q}, p0, \[\1\]
**	ret
*/
TEST_STORE (st4q_u32_index, svuint32x4_t, uint32_t,
	    svst4q_u32 (p0, x0 + x1, z0),
	    svst4q (p0, x0 + x1, z0))

/*
** st4q_u32_index2:
**	add	(x[0-9]), x0, x1, lsl #?3
**	st4q	{z0\.q - z3\.q}, p0, \[\1\]
**	ret
*/
TEST_STORE (st4q_u32_index2, svuint32x4_t, uint32_t,
	    svst4q_u32 (p0, x0 + x1 * 2, z0),
	    svst4q (p0, x0 + x1 * 2, z0))

/*
** st4q_u32_index4:
**	st4q	{z0\.q - z3\.q}, p0, \[x0, x1, lsl #?4\]
**	ret
*/
TEST_STORE (st4q_u32_index4, svuint32x4_t, uint32_t,
	    svst4q_u32 (p0, x0 + x1 * 4, z0),
	    svst4q (p0, x0 + x1 * 4, z0))

/* Moving the constant into a register would also be OK.  */
/*
** st4q_u32_1:
**	incb	x0
**	st4q	{z0\.q - z3\.q}, p0, \[x0\]
**	ret
*/
TEST_STORE (st4q_u32_1, svuint32x4_t, uint32_t,
	    svst4q_u32 (p0, x0 + svcntw (), z0),
	    svst4q (p0, x0 + svcntw (), z0))

/* Moving the constant into a register would also be OK.  */
/*
** st4q_u32_2:
**	incb	x0, all, mul #2
**	st4q	{z0\.q - z3\.q}, p0, \[x0\]
**	ret
*/
TEST_STORE (st4q_u32_2, svuint32x4_t, uint32_t,
	    svst4q_u32 (p0, x0 + svcntw () * 2, z0),
	    svst4q (p0, x0 + svcntw () * 2, z0))

/* Moving the constant into a register would also be OK.  */
/*
** st4q_u32_3:
**	incb	x0, all, mul #3
**	st4q	{z0\.q - z3\.q}, p0, \[x0\]
**	ret
*/
TEST_STORE (st4q_u32_3, svuint32x4_t, uint32_t,
	    svst4q_u32 (p0, x0 + svcntw () * 3, z0),
	    svst4q (p0, x0 + svcntw () * 3, z0))

/*
** st4q_u32_4:
**	st4q	{z0\.q - z3\.q}, p0, \[x0, #4, mul vl\]
**	ret
*/
TEST_STORE (st4q_u32_4, svuint32x4_t, uint32_t,
	    svst4q_u32 (p0, x0 + svcntw () * 4, z0),
	    svst4q (p0, x0 + svcntw () * 4, z0))

/*
** st4q_u32_28:
**	st4q	{z0\.q - z3\.q}, p0, \[x0, #28, mul vl\]
**	ret
*/
TEST_STORE (st4q_u32_28, svuint32x4_t, uint32_t,
	    svst4q_u32 (p0, x0 + svcntw () * 28, z0),
	    svst4q (p0, x0 + svcntw () * 28, z0))

/*
** st4q_u32_32:
**	[^{]*
**	st4q	{z0\.q - z3\.q}, p0, \[x[0-9]+\]
**	ret
*/
TEST_STORE (st4q_u32_32, svuint32x4_t, uint32_t,
	    svst4q_u32 (p0, x0 + svcntw () * 32, z0),
	    svst4q (p0, x0 + svcntw () * 32, z0))

/* Moving the constant into a register would also be OK.  */
/*
** st4q_u32_m1:
**	decb	x0
**	st4q	{z0\.q - z3\.q}, p0, \[x0\]
**	ret
*/
TEST_STORE (st4q_u32_m1, svuint32x4_t, uint32_t,
	    svst4q_u32 (p0, x0 - svcntw (), z0),
	    svst4q (p0, x0 - svcntw (), z0))

/* Moving the constant into a register would also be OK.  */
/*
** st4q_u32_m2:
**	decb	x0, all, mul #2
**	st4q	{z0\.q - z3\.q}, p0, \[x0\]
**	ret
*/
TEST_STORE (st4q_u32_m2, svuint32x4_t, uint32_t,
	    svst4q_u32 (p0, x0 - svcntw () * 2, z0),
	    svst4q (p0, x0 - svcntw () * 2, z0))

/* Moving the constant into a register would also be OK.  */
/*
** st4q_u32_m3:
**	decb	x0, all, mul #3
**	st4q	{z0\.q - z3\.q}, p0, \[x0\]
**	ret
*/
TEST_STORE (st4q_u32_m3, svuint32x4_t, uint32_t,
	    svst4q_u32 (p0, x0 - svcntw () * 3, z0),
	    svst4q (p0, x0 - svcntw () * 3, z0))

/*
** st4q_u32_m4:
**	st4q	{z0\.q - z3\.q}, p0, \[x0, #-4, mul vl\]
**	ret
*/
TEST_STORE (st4q_u32_m4, svuint32x4_t, uint32_t,
	    svst4q_u32 (p0, x0 - svcntw () * 4, z0),
	    svst4q (p0, x0 - svcntw () * 4, z0))

/*
** st4q_u32_m32:
**	st4q	{z0\.q - z3\.q}, p0, \[x0, #-32, mul vl\]
**	ret
*/
TEST_STORE (st4q_u32_m32, svuint32x4_t, uint32_t,
	    svst4q_u32 (p0, x0 - svcntw () * 32, z0),
	    svst4q (p0, x0 - svcntw () * 32, z0))

/*
** st4q_u32_m36:
**	[^{]*
**	st4q	{z0\.q - z3\.q}, p0, \[x[0-9]+\]
**	ret
*/
TEST_STORE (st4q_u32_m36, svuint32x4_t, uint32_t,
	    svst4q_u32 (p0, x0 - svcntw () * 36, z0),
	    svst4q (p0, x0 - svcntw () * 36, z0))

/*
** st4q_vnum_u32_0:
**	st4q	{z0\.q - z3\.q}, p0, \[x0\]
**	ret
*/
TEST_STORE (st4q_vnum_u32_0, svuint32x4_t, uint32_t,
	    svst4q_vnum_u32 (p0, x0, 0, z0),
	    svst4q_vnum (p0, x0, 0, z0))

/* Moving the constant into a register would also be OK.  */
/*
** st4q_vnum_u32_1:
**	incb	x0
**	st4q	{z0\.q - z3\.q}, p0, \[x0\]
**	ret
*/
TEST_STORE (st4q_vnum_u32_1, svuint32x4_t, uint32_t,
	    svst4q_vnum_u32 (p0, x0, 1, z0),
	    svst4q_vnum (p0, x0, 1, z0))

/* Moving the constant into a register would also be OK.  */
/*
** st4q_vnum_u32_2:
**	incb	x0, all, mul #2
**	st4q	{z0\.q - z3\.q}, p0, \[x0\]
**	ret
*/
TEST_STORE (st4q_vnum_u32_2, svuint32x4_t, uint32_t,
	    svst4q_vnum_u32 (p0, x0, 2, z0),
	    svst4q_vnum (p0, x0, 2, z0))

/* Moving the constant into a register would also be OK.  */
/*
** st4q_vnum_u32_3:
**	incb	x0, all, mul #3
**	st4q	{z0\.q - z3\.q}, p0, \[x0\]
**	ret
*/
TEST_STORE (st4q_vnum_u32_3, svuint32x4_t, uint32_t,
	    svst4q_vnum_u32 (p0, x0, 3, z0),
	    svst4q_vnum (p0, x0, 3, z0))

/*
** st4q_vnum_u32_4:
**	st4q	{z0\.q - z3\.q}, p0, \[x0, #4, mul vl\]
**	ret
*/
TEST_STORE (st4q_vnum_u32_4, svuint32x4_t, uint32_t,
	    svst4q_vnum_u32 (p0, x0, 4, z0),
	    svst4q_vnum (p0, x0, 4, z0))

/*
** st4q_vnum_u32_28:
**	st4q	{z0\.q - z3\.q}, p0, \[x0, #28, mul vl\]
**	ret
*/
TEST_STORE (st4q_vnum_u32_28, svuint32x4_t, uint32_t,
	    svst4q_vnum_u32 (p0, x0, 28, z0),
	    svst4q_vnum (p0, x0, 28, z0))

/*
** st4q_vnum_u32_32:
**	[^{]*
**	st4q	{z0\.q - z3\.q}, p0, \[x[0-9]+\]
**	ret
*/
TEST_STORE (st4q_vnum_u32_32, svuint32x4_t, uint32_t,
	    svst4q_vnum_u32 (p0, x0, 32, z0),
	    svst4q_vnum (p0, x0, 32, z0))

/* Moving the constant into a register would also be OK.  */
/*
** st4q_vnum_u32_m1:
**	decb	x0
**	st4q	{z0\.q - z3\.q}, p0, \[x0\]
**	ret
*/
TEST_STORE (st4q_vnum_u32_m1, svuint32x4_t, uint32_t,
	    svst4q_vnum_u32 (p0, x0, -1, z0),
	    svst4q_vnum (p0, x0, -1, z0))

/* Moving the constant into a register would also be OK.  */
/*
** st4q_vnum_u32_m2:
**	decb	x0, all, mul #2
**	st4q	{z0\.q - z3\.q}, p0, \[x0\]
**	ret
*/
TEST_STORE (st4q_vnum_u32_m2, svuint32x4_t, uint32_t,
	    svst4q_vnum_u32 (p0, x0, -2, z0),
	    svst4q_vnum (p0, x0, -2, z0))

/* Moving the constant into a register would also be OK.  */
/*
** st4q_vnum_u32_m3:
**	decb	x0, all, mul #3
**	st4q	{z0\.q - z3\.q}, p0, \[x0\]
**	ret
*/
TEST_STORE (st4q_vnum_u32_m3, svuint32x4_t, uint32_t,
	    svst4q_vnum_u32 (p0, x0, -3, z0),
	    svst4q_vnum (p0, x0, -3, z0))

/*
** st4q_vnum_u32_m4:
**	st4q	{z0\.q - z3\.q}, p0, \[x0, #-4, mul vl\]
**	ret
*/
TEST_STORE (st4q_vnum_u32_m4, svuint32x4_t, uint32_t,
	    svst4q_vnum_u32 (p0, x0, -4, z0),
	    svst4q_vnum (p0, x0, -4, z0))

/*
** st4q_vnum_u32_m32:
**	st4q	{z0\.q - z3\.q}, p0, \[x0, #-32, mul vl\]
**	ret
*/
TEST_STORE (st4q_vnum_u32_m32, svuint32x4_t, uint32_t,
	    svst4q_vnum_u32 (p0, x0, -32, z0),
	    svst4q_vnum (p0, x0, -32, z0))

/*
** st4q_vnum_u32_m36:
**	[^{]*
**	st4q	{z0\.q - z3\.q}, p0, \[x[0-9]+\]
**	ret
*/
TEST_STORE (st4q_vnum_u32_m36, svuint32x4_t, uint32_t,
	    svst4q_vnum_u32 (p0, x0, -36, z0),
	    svst4q_vnum (p0, x0, -36, z0))

/*
** st4q_vnum_u32_x1:
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
TEST_STORE (st4q_vnum_u32_x1, svuint32x4_t, uint32_t,
	    svst4q_vnum_u32 (p0, x0, x1, z0),
	    svst4q_vnum (p0, x0, x1, z0))
