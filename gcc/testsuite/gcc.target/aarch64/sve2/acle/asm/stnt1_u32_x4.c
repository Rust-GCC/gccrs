/* { dg-do assemble { target aarch64_asm_sve2p1_ok } } */
/* { dg-do compile { target { ! aarch64_asm_sve2p1_ok } } } */
/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" { target { ! ilp32 } } } } */

#include "test_sve_acle.h"

#pragma GCC target "+sve2p1"
#ifdef STREAMING_COMPATIBLE
#pragma GCC target "+sme2"
#endif

/*
** stnt1_u32_base:
**	stnt1w	{z0\.s - z3\.s}, pn8, \[x0\]
**	ret
*/
TEST_STORE_COUNT (stnt1_u32_base, svuint32x4_t, uint32_t,
		  svstnt1_u32_x4 (pn8, x0, z0),
		  svstnt1 (pn8, x0, z0))

/*
** stnt1_u32_index:
**	stnt1w	{z0\.s - z3\.s}, pn8, \[x0, x1, lsl #?2\]
**	ret
*/
TEST_STORE_COUNT (stnt1_u32_index, svuint32x4_t, uint32_t,
		  svstnt1_u32_x4 (pn8, x0 + x1, z0),
		  svstnt1 (pn8, x0 + x1, z0))

/* Moving the constant into a register would also be OK.  */
/*
** stnt1_u32_1:
**	incb	x0
**	stnt1w	{z0\.s - z3\.s}, pn8, \[x0\]
**	ret
*/
TEST_STORE_COUNT (stnt1_u32_1, svuint32x4_t, uint32_t,
		  svstnt1_u32_x4 (pn8, x0 + svcntw (), z0),
		  svstnt1 (pn8, x0 + svcntw (), z0))

/* Moving the constant into a register would also be OK.  */
/*
** stnt1_u32_2:
**	incb	x0, all, mul #2
**	stnt1w	{z0\.s - z3\.s}, pn8, \[x0\]
**	ret
*/
TEST_STORE_COUNT (stnt1_u32_2, svuint32x4_t, uint32_t,
		  svstnt1_u32_x4 (pn8, x0 + svcntw () * 2, z0),
		  svstnt1 (pn8, x0 + svcntw () * 2, z0))

/* Moving the constant into a register would also be OK.  */
/*
** stnt1_u32_3:
**	incb	x0, all, mul #3
**	stnt1w	{z0\.s - z3\.s}, pn8, \[x0\]
**	ret
*/
TEST_STORE_COUNT (stnt1_u32_3, svuint32x4_t, uint32_t,
		  svstnt1_u32_x4 (pn8, x0 + svcntw () * 3, z0),
		  svstnt1 (pn8, x0 + svcntw () * 3, z0))

/*
** stnt1_u32_4:
**	stnt1w	{z0\.s - z3\.s}, pn8, \[x0, #4, mul vl\]
**	ret
*/
TEST_STORE_COUNT (stnt1_u32_4, svuint32x4_t, uint32_t,
		  svstnt1_u32_x4 (pn8, x0 + svcntw () * 4, z0),
		  svstnt1 (pn8, x0 + svcntw () * 4, z0))

/*
** stnt1_u32_28:
**	stnt1w	{z0\.s - z3\.s}, pn8, \[x0, #28, mul vl\]
**	ret
*/
TEST_STORE_COUNT (stnt1_u32_28, svuint32x4_t, uint32_t,
		  svstnt1_u32_x4 (pn8, x0 + svcntw () * 28, z0),
		  svstnt1 (pn8, x0 + svcntw () * 28, z0))

/*
** stnt1_u32_32:
**	[^{]*
**	stnt1w	{z0\.s - z3\.s}, pn8, \[x[0-9]+\]
**	ret
*/
TEST_STORE_COUNT (stnt1_u32_32, svuint32x4_t, uint32_t,
		  svstnt1_u32_x4 (pn8, x0 + svcntw () * 32, z0),
		  svstnt1 (pn8, x0 + svcntw () * 32, z0))

/* Moving the constant into a register would also be OK.  */
/*
** stnt1_u32_m1:
**	decb	x0
**	stnt1w	{z0\.s - z3\.s}, pn8, \[x0\]
**	ret
*/
TEST_STORE_COUNT (stnt1_u32_m1, svuint32x4_t, uint32_t,
		  svstnt1_u32_x4 (pn8, x0 - svcntw (), z0),
		  svstnt1 (pn8, x0 - svcntw (), z0))

/* Moving the constant into a register would also be OK.  */
/*
** stnt1_u32_m2:
**	decb	x0, all, mul #2
**	stnt1w	{z0\.s - z3\.s}, pn8, \[x0\]
**	ret
*/
TEST_STORE_COUNT (stnt1_u32_m2, svuint32x4_t, uint32_t,
		  svstnt1_u32_x4 (pn8, x0 - svcntw () * 2, z0),
		  svstnt1 (pn8, x0 - svcntw () * 2, z0))

/* Moving the constant into a register would also be OK.  */
/*
** stnt1_u32_m3:
**	decb	x0, all, mul #3
**	stnt1w	{z0\.s - z3\.s}, pn8, \[x0\]
**	ret
*/
TEST_STORE_COUNT (stnt1_u32_m3, svuint32x4_t, uint32_t,
		  svstnt1_u32_x4 (pn8, x0 - svcntw () * 3, z0),
		  svstnt1 (pn8, x0 - svcntw () * 3, z0))

/*
** stnt1_u32_m4:
**	stnt1w	{z0\.s - z3\.s}, pn8, \[x0, #-4, mul vl\]
**	ret
*/
TEST_STORE_COUNT (stnt1_u32_m4, svuint32x4_t, uint32_t,
		  svstnt1_u32_x4 (pn8, x0 - svcntw () * 4, z0),
		  svstnt1 (pn8, x0 - svcntw () * 4, z0))

/*
** stnt1_u32_m32:
**	stnt1w	{z0\.s - z3\.s}, pn8, \[x0, #-32, mul vl\]
**	ret
*/
TEST_STORE_COUNT (stnt1_u32_m32, svuint32x4_t, uint32_t,
		  svstnt1_u32_x4 (pn8, x0 - svcntw () * 32, z0),
		  svstnt1 (pn8, x0 - svcntw () * 32, z0))

/*
** stnt1_u32_m36:
**	[^{]*
**	stnt1w	{z0\.s - z3\.s}, pn8, \[x[0-9]+\]
**	ret
*/
TEST_STORE_COUNT (stnt1_u32_m36, svuint32x4_t, uint32_t,
		  svstnt1_u32_x4 (pn8, x0 - svcntw () * 36, z0),
		  svstnt1 (pn8, x0 - svcntw () * 36, z0))

/*
** stnt1_u32_z17:
**	mov	[^\n]+
**	mov	[^\n]+
**	mov	[^\n]+
**	mov	[^\n]+
**	stnt1w	{z[^\n]+}, pn8, \[x0\]
**	ret
*/
TEST_STORE_COUNT (stnt1_u32_z17, svuint32x4_t, uint32_t,
		  svstnt1_u32_x4 (pn8, x0, z17),
		  svstnt1 (pn8, x0, z17))

/*
** stnt1_u32_z22:
**	mov	[^\n]+
**	mov	[^\n]+
**	mov	[^\n]+
**	mov	[^\n]+
**	stnt1w	{z[^\n]+}, pn8, \[x0\]
**	ret
*/
TEST_STORE_COUNT (stnt1_u32_z22, svuint32x4_t, uint32_t,
		  svstnt1_u32_x4 (pn8, x0, z22),
		  svstnt1 (pn8, x0, z22))

/*
** stnt1_u32_z28:
**	stnt1w	{z28\.s - z31\.s}, pn8, \[x0\]
**	ret
*/
TEST_STORE_COUNT (stnt1_u32_z28, svuint32x4_t, uint32_t,
		  svstnt1_u32_x4 (pn8, x0, z28),
		  svstnt1 (pn8, x0, z28))

/*
** stnt1_u32_pn0:
**	mov	p([89]|1[0-5])\.b, p0\.b
**	stnt1w	{z0\.s - z3\.s}, pn\1, \[x0\]
**	ret
*/
TEST_STORE_COUNT (stnt1_u32_pn0, svuint32x4_t, uint32_t,
		  svstnt1_u32_x4 (pn0, x0, z0),
		  svstnt1 (pn0, x0, z0))

/*
** stnt1_u32_pn7:
**	mov	p([89]|1[0-5])\.b, p7\.b
**	stnt1w	{z0\.s - z3\.s}, pn\1, \[x0\]
**	ret
*/
TEST_STORE_COUNT (stnt1_u32_pn7, svuint32x4_t, uint32_t,
		  svstnt1_u32_x4 (pn7, x0, z0),
		  svstnt1 (pn7, x0, z0))

/*
** stnt1_u32_pn15:
**	stnt1w	{z0\.s - z3\.s}, pn15, \[x0\]
**	ret
*/
TEST_STORE_COUNT (stnt1_u32_pn15, svuint32x4_t, uint32_t,
		  svstnt1_u32_x4 (pn15, x0, z0),
		  svstnt1 (pn15, x0, z0))

/*
** stnt1_vnum_u32_0:
**	stnt1w	{z0\.s - z3\.s}, pn8, \[x0\]
**	ret
*/
TEST_STORE_COUNT (stnt1_vnum_u32_0, svuint32x4_t, uint32_t,
		  svstnt1_vnum_u32_x4 (pn8, x0, 0, z0),
		  svstnt1_vnum (pn8, x0, 0, z0))

/* Moving the constant into a register would also be OK.  */
/*
** stnt1_vnum_u32_1:
**	incb	x0
**	stnt1w	{z0\.s - z3\.s}, pn8, \[x0\]
**	ret
*/
TEST_STORE_COUNT (stnt1_vnum_u32_1, svuint32x4_t, uint32_t,
		  svstnt1_vnum_u32_x4 (pn8, x0, 1, z0),
		  svstnt1_vnum (pn8, x0, 1, z0))

/* Moving the constant into a register would also be OK.  */
/*
** stnt1_vnum_u32_2:
**	incb	x0, all, mul #2
**	stnt1w	{z0\.s - z3\.s}, pn8, \[x0\]
**	ret
*/
TEST_STORE_COUNT (stnt1_vnum_u32_2, svuint32x4_t, uint32_t,
		  svstnt1_vnum_u32_x4 (pn8, x0, 2, z0),
		  svstnt1_vnum (pn8, x0, 2, z0))

/* Moving the constant into a register would also be OK.  */
/*
** stnt1_vnum_u32_3:
**	incb	x0, all, mul #3
**	stnt1w	{z0\.s - z3\.s}, pn8, \[x0\]
**	ret
*/
TEST_STORE_COUNT (stnt1_vnum_u32_3, svuint32x4_t, uint32_t,
		  svstnt1_vnum_u32_x4 (pn8, x0, 3, z0),
		  svstnt1_vnum (pn8, x0, 3, z0))

/*
** stnt1_vnum_u32_4:
**	stnt1w	{z0\.s - z3\.s}, pn8, \[x0, #4, mul vl\]
**	ret
*/
TEST_STORE_COUNT (stnt1_vnum_u32_4, svuint32x4_t, uint32_t,
		  svstnt1_vnum_u32_x4 (pn8, x0, 4, z0),
		  svstnt1_vnum (pn8, x0, 4, z0))

/*
** stnt1_vnum_u32_28:
**	stnt1w	{z0\.s - z3\.s}, pn8, \[x0, #28, mul vl\]
**	ret
*/
TEST_STORE_COUNT (stnt1_vnum_u32_28, svuint32x4_t, uint32_t,
		  svstnt1_vnum_u32_x4 (pn8, x0, 28, z0),
		  svstnt1_vnum (pn8, x0, 28, z0))

/*
** stnt1_vnum_u32_32:
**	[^{]*
**	stnt1w	{z0\.s - z3\.s}, pn8, \[x[0-9]+\]
**	ret
*/
TEST_STORE_COUNT (stnt1_vnum_u32_32, svuint32x4_t, uint32_t,
		  svstnt1_vnum_u32_x4 (pn8, x0, 32, z0),
		  svstnt1_vnum (pn8, x0, 32, z0))

/* Moving the constant into a register would also be OK.  */
/*
** stnt1_vnum_u32_m1:
**	decb	x0
**	stnt1w	{z0\.s - z3\.s}, pn8, \[x0\]
**	ret
*/
TEST_STORE_COUNT (stnt1_vnum_u32_m1, svuint32x4_t, uint32_t,
		  svstnt1_vnum_u32_x4 (pn8, x0, -1, z0),
		  svstnt1_vnum (pn8, x0, -1, z0))

/* Moving the constant into a register would also be OK.  */
/*
** stnt1_vnum_u32_m2:
**	decb	x0, all, mul #2
**	stnt1w	{z0\.s - z3\.s}, pn8, \[x0\]
**	ret
*/
TEST_STORE_COUNT (stnt1_vnum_u32_m2, svuint32x4_t, uint32_t,
		  svstnt1_vnum_u32_x4 (pn8, x0, -2, z0),
		  svstnt1_vnum (pn8, x0, -2, z0))

/* Moving the constant into a register would also be OK.  */
/*
** stnt1_vnum_u32_m3:
**	decb	x0, all, mul #3
**	stnt1w	{z0\.s - z3\.s}, pn8, \[x0\]
**	ret
*/
TEST_STORE_COUNT (stnt1_vnum_u32_m3, svuint32x4_t, uint32_t,
		  svstnt1_vnum_u32_x4 (pn8, x0, -3, z0),
		  svstnt1_vnum (pn8, x0, -3, z0))

/*
** stnt1_vnum_u32_m4:
**	stnt1w	{z0\.s - z3\.s}, pn8, \[x0, #-4, mul vl\]
**	ret
*/
TEST_STORE_COUNT (stnt1_vnum_u32_m4, svuint32x4_t, uint32_t,
		  svstnt1_vnum_u32_x4 (pn8, x0, -4, z0),
		  svstnt1_vnum (pn8, x0, -4, z0))

/*
** stnt1_vnum_u32_m32:
**	stnt1w	{z0\.s - z3\.s}, pn8, \[x0, #-32, mul vl\]
**	ret
*/
TEST_STORE_COUNT (stnt1_vnum_u32_m32, svuint32x4_t, uint32_t,
		  svstnt1_vnum_u32_x4 (pn8, x0, -32, z0),
		  svstnt1_vnum (pn8, x0, -32, z0))

/*
** stnt1_vnum_u32_m36:
**	[^{]*
**	stnt1w	{z0\.s - z3\.s}, pn8, \[x[0-9]+\]
**	ret
*/
TEST_STORE_COUNT (stnt1_vnum_u32_m36, svuint32x4_t, uint32_t,
		  svstnt1_vnum_u32_x4 (pn8, x0, -36, z0),
		  svstnt1_vnum (pn8, x0, -36, z0))

/*
** stnt1_vnum_u32_x1:
**	cntb	(x[0-9]+)
** (
**	madd	(x[0-9]+), (?:x1, \1|\1, x1), x0
**	stnt1w	{z0\.s - z3\.s}, pn8, \[\2\]
** |
**	mul	(x[0-9]+), (?:x1, \1|\1, x1)
**	stnt1w	{z0\.s - z3\.s}, pn8, \[x0, \3\]
** )
**	ret
*/
TEST_STORE_COUNT (stnt1_vnum_u32_x1, svuint32x4_t, uint32_t,
		  svstnt1_vnum_u32_x4 (pn8, x0, x1, z0),
		  svstnt1_vnum (pn8, x0, x1, z0))
