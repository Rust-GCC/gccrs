/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" } } */

#include "test_sve_acle.h"

/*
** ld1sb_s32_base:
**	ld1sb	z0\.s, p0/z, \[x0\]
**	ret
*/
TEST_LOAD (ld1sb_s32_base, svint32_t, int8_t,
	   z0 = svld1sb_s32 (p0, x0),
	   z0 = svld1sb_s32 (p0, x0))

/*
** ld1sb_s32_index:
**	ld1sb	z0\.s, p0/z, \[x0, x1\]
**	ret
*/
TEST_LOAD (ld1sb_s32_index, svint32_t, int8_t,
	   z0 = svld1sb_s32 (p0, x0 + x1),
	   z0 = svld1sb_s32 (p0, x0 + x1))

/*
** ld1sb_s32_1:
**	ld1sb	z0\.s, p0/z, \[x0, #1, mul vl\]
**	ret
*/
TEST_LOAD (ld1sb_s32_1, svint32_t, int8_t,
	   z0 = svld1sb_s32 (p0, x0 + svcntw ()),
	   z0 = svld1sb_s32 (p0, x0 + svcntw ()))

/*
** ld1sb_s32_7:
**	ld1sb	z0\.s, p0/z, \[x0, #7, mul vl\]
**	ret
*/
TEST_LOAD (ld1sb_s32_7, svint32_t, int8_t,
	   z0 = svld1sb_s32 (p0, x0 + svcntw () * 7),
	   z0 = svld1sb_s32 (p0, x0 + svcntw () * 7))

/* Moving the constant into a register would also be OK.  */
/*
** ld1sb_s32_8:
**	incb	x0, all, mul #2
**	ld1sb	z0\.s, p0/z, \[x0\]
**	ret
*/
TEST_LOAD (ld1sb_s32_8, svint32_t, int8_t,
	   z0 = svld1sb_s32 (p0, x0 + svcntw () * 8),
	   z0 = svld1sb_s32 (p0, x0 + svcntw () * 8))

/*
** ld1sb_s32_m1:
**	ld1sb	z0\.s, p0/z, \[x0, #-1, mul vl\]
**	ret
*/
TEST_LOAD (ld1sb_s32_m1, svint32_t, int8_t,
	   z0 = svld1sb_s32 (p0, x0 - svcntw ()),
	   z0 = svld1sb_s32 (p0, x0 - svcntw ()))

/*
** ld1sb_s32_m8:
**	ld1sb	z0\.s, p0/z, \[x0, #-8, mul vl\]
**	ret
*/
TEST_LOAD (ld1sb_s32_m8, svint32_t, int8_t,
	   z0 = svld1sb_s32 (p0, x0 - svcntw () * 8),
	   z0 = svld1sb_s32 (p0, x0 - svcntw () * 8))

/* Moving the constant into a register would also be OK.  */
/*
** ld1sb_s32_m9:
**	decw	x0, all, mul #9
**	ld1sb	z0\.s, p0/z, \[x0\]
**	ret
*/
TEST_LOAD (ld1sb_s32_m9, svint32_t, int8_t,
	   z0 = svld1sb_s32 (p0, x0 - svcntw () * 9),
	   z0 = svld1sb_s32 (p0, x0 - svcntw () * 9))

/*
** ld1sb_vnum_s32_0:
**	ld1sb	z0\.s, p0/z, \[x0\]
**	ret
*/
TEST_LOAD (ld1sb_vnum_s32_0, svint32_t, int8_t,
	   z0 = svld1sb_vnum_s32 (p0, x0, 0),
	   z0 = svld1sb_vnum_s32 (p0, x0, 0))

/*
** ld1sb_vnum_s32_1:
**	ld1sb	z0\.s, p0/z, \[x0, #1, mul vl\]
**	ret
*/
TEST_LOAD (ld1sb_vnum_s32_1, svint32_t, int8_t,
	   z0 = svld1sb_vnum_s32 (p0, x0, 1),
	   z0 = svld1sb_vnum_s32 (p0, x0, 1))

/*
** ld1sb_vnum_s32_7:
**	ld1sb	z0\.s, p0/z, \[x0, #7, mul vl\]
**	ret
*/
TEST_LOAD (ld1sb_vnum_s32_7, svint32_t, int8_t,
	   z0 = svld1sb_vnum_s32 (p0, x0, 7),
	   z0 = svld1sb_vnum_s32 (p0, x0, 7))

/* Moving the constant into a register would also be OK.  */
/*
** ld1sb_vnum_s32_8:
**	incb	x0, all, mul #2
**	ld1sb	z0\.s, p0/z, \[x0\]
**	ret
*/
TEST_LOAD (ld1sb_vnum_s32_8, svint32_t, int8_t,
	   z0 = svld1sb_vnum_s32 (p0, x0, 8),
	   z0 = svld1sb_vnum_s32 (p0, x0, 8))

/*
** ld1sb_vnum_s32_m1:
**	ld1sb	z0\.s, p0/z, \[x0, #-1, mul vl\]
**	ret
*/
TEST_LOAD (ld1sb_vnum_s32_m1, svint32_t, int8_t,
	   z0 = svld1sb_vnum_s32 (p0, x0, -1),
	   z0 = svld1sb_vnum_s32 (p0, x0, -1))

/*
** ld1sb_vnum_s32_m8:
**	ld1sb	z0\.s, p0/z, \[x0, #-8, mul vl\]
**	ret
*/
TEST_LOAD (ld1sb_vnum_s32_m8, svint32_t, int8_t,
	   z0 = svld1sb_vnum_s32 (p0, x0, -8),
	   z0 = svld1sb_vnum_s32 (p0, x0, -8))

/* Moving the constant into a register would also be OK.  */
/*
** ld1sb_vnum_s32_m9:
**	decw	x0, all, mul #9
**	ld1sb	z0\.s, p0/z, \[x0\]
**	ret
*/
TEST_LOAD (ld1sb_vnum_s32_m9, svint32_t, int8_t,
	   z0 = svld1sb_vnum_s32 (p0, x0, -9),
	   z0 = svld1sb_vnum_s32 (p0, x0, -9))

/*
** ld1sb_vnum_s32_x1:
**	cntw	(x[0-9]+)
** (
**	madd	(x[0-9]+), (?:x1, \1|\1, x1), x0
**	ld1sb	z0\.s, p0/z, \[\2\]
** |
**	mul	(x[0-9]+), (?:x1, \1|\1, x1)
**	ld1sb	z0\.s, p0/z, \[x0, \3\]
** )
**	ret
*/
TEST_LOAD (ld1sb_vnum_s32_x1, svint32_t, int8_t,
	   z0 = svld1sb_vnum_s32 (p0, x0, x1),
	   z0 = svld1sb_vnum_s32 (p0, x0, x1))
