/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" } } */

#include "test_sve_acle.h"

/*
** ld2_s16_base:
**	ld2h	{z0\.h(?: - |, )z1\.h}, p0/z, \[x0\]
**	ret
*/
TEST_LOAD (ld2_s16_base, svint16x2_t, int16_t,
	   z0 = svld2_s16 (p0, x0),
	   z0 = svld2 (p0, x0))

/*
** ld2_s16_index:
**	ld2h	{z0\.h(?: - |, )z1\.h}, p0/z, \[x0, x1, lsl 1\]
**	ret
*/
TEST_LOAD (ld2_s16_index, svint16x2_t, int16_t,
	   z0 = svld2_s16 (p0, x0 + x1),
	   z0 = svld2 (p0, x0 + x1))

/* Moving the constant into a register would also be OK.  */
/*
** ld2_s16_1:
**	incb	x0
**	ld2h	{z0\.h(?: - |, )z1\.h}, p0/z, \[x0\]
**	ret
*/
TEST_LOAD (ld2_s16_1, svint16x2_t, int16_t,
	   z0 = svld2_s16 (p0, x0 + svcnth ()),
	   z0 = svld2 (p0, x0 + svcnth ()))

/*
** ld2_s16_2:
**	ld2h	{z0\.h(?: - |, )z1\.h}, p0/z, \[x0, #2, mul vl\]
**	ret
*/
TEST_LOAD (ld2_s16_2, svint16x2_t, int16_t,
	   z0 = svld2_s16 (p0, x0 + svcnth () * 2),
	   z0 = svld2 (p0, x0 + svcnth () * 2))

/*
** ld2_s16_14:
**	ld2h	{z0\.h(?: - |, )z1\.h}, p0/z, \[x0, #14, mul vl\]
**	ret
*/
TEST_LOAD (ld2_s16_14, svint16x2_t, int16_t,
	   z0 = svld2_s16 (p0, x0 + svcnth () * 14),
	   z0 = svld2 (p0, x0 + svcnth () * 14))

/* Moving the constant into a register would also be OK.  */
/*
** ld2_s16_16:
**	incb	x0, all, mul #16
**	ld2h	{z0\.h(?: - |, )z1\.h}, p0/z, \[x0\]
**	ret
*/
TEST_LOAD (ld2_s16_16, svint16x2_t, int16_t,
	   z0 = svld2_s16 (p0, x0 + svcnth () * 16),
	   z0 = svld2 (p0, x0 + svcnth () * 16))

/* Moving the constant into a register would also be OK.  */
/*
** ld2_s16_m1:
**	decb	x0
**	ld2h	{z0\.h(?: - |, )z1\.h}, p0/z, \[x0\]
**	ret
*/
TEST_LOAD (ld2_s16_m1, svint16x2_t, int16_t,
	   z0 = svld2_s16 (p0, x0 - svcnth ()),
	   z0 = svld2 (p0, x0 - svcnth ()))

/*
** ld2_s16_m2:
**	ld2h	{z0\.h(?: - |, )z1\.h}, p0/z, \[x0, #-2, mul vl\]
**	ret
*/
TEST_LOAD (ld2_s16_m2, svint16x2_t, int16_t,
	   z0 = svld2_s16 (p0, x0 - svcnth () * 2),
	   z0 = svld2 (p0, x0 - svcnth () * 2))

/*
** ld2_s16_m16:
**	ld2h	{z0\.h(?: - |, )z1\.h}, p0/z, \[x0, #-16, mul vl\]
**	ret
*/
TEST_LOAD (ld2_s16_m16, svint16x2_t, int16_t,
	   z0 = svld2_s16 (p0, x0 - svcnth () * 16),
	   z0 = svld2 (p0, x0 - svcnth () * 16))

/*
** ld2_s16_m18:
**	addvl	(x[0-9]+), x0, #-18
**	ld2h	{z0\.h(?: - |, )z1\.h}, p0/z, \[\1\]
**	ret
*/
TEST_LOAD (ld2_s16_m18, svint16x2_t, int16_t,
	   z0 = svld2_s16 (p0, x0 - svcnth () * 18),
	   z0 = svld2 (p0, x0 - svcnth () * 18))

/*
** ld2_vnum_s16_0:
**	ld2h	{z0\.h(?: - |, )z1\.h}, p0/z, \[x0\]
**	ret
*/
TEST_LOAD (ld2_vnum_s16_0, svint16x2_t, int16_t,
	   z0 = svld2_vnum_s16 (p0, x0, 0),
	   z0 = svld2_vnum (p0, x0, 0))

/* Moving the constant into a register would also be OK.  */
/*
** ld2_vnum_s16_1:
**	incb	x0
**	ld2h	{z0\.h(?: - |, )z1\.h}, p0/z, \[x0\]
**	ret
*/
TEST_LOAD (ld2_vnum_s16_1, svint16x2_t, int16_t,
	   z0 = svld2_vnum_s16 (p0, x0, 1),
	   z0 = svld2_vnum (p0, x0, 1))

/*
** ld2_vnum_s16_2:
**	ld2h	{z0\.h(?: - |, )z1\.h}, p0/z, \[x0, #2, mul vl\]
**	ret
*/
TEST_LOAD (ld2_vnum_s16_2, svint16x2_t, int16_t,
	   z0 = svld2_vnum_s16 (p0, x0, 2),
	   z0 = svld2_vnum (p0, x0, 2))

/*
** ld2_vnum_s16_14:
**	ld2h	{z0\.h(?: - |, )z1\.h}, p0/z, \[x0, #14, mul vl\]
**	ret
*/
TEST_LOAD (ld2_vnum_s16_14, svint16x2_t, int16_t,
	   z0 = svld2_vnum_s16 (p0, x0, 14),
	   z0 = svld2_vnum (p0, x0, 14))

/* Moving the constant into a register would also be OK.  */
/*
** ld2_vnum_s16_16:
**	incb	x0, all, mul #16
**	ld2h	{z0\.h(?: - |, )z1\.h}, p0/z, \[x0\]
**	ret
*/
TEST_LOAD (ld2_vnum_s16_16, svint16x2_t, int16_t,
	   z0 = svld2_vnum_s16 (p0, x0, 16),
	   z0 = svld2_vnum (p0, x0, 16))

/* Moving the constant into a register would also be OK.  */
/*
** ld2_vnum_s16_m1:
**	decb	x0
**	ld2h	{z0\.h(?: - |, )z1\.h}, p0/z, \[x0\]
**	ret
*/
TEST_LOAD (ld2_vnum_s16_m1, svint16x2_t, int16_t,
	   z0 = svld2_vnum_s16 (p0, x0, -1),
	   z0 = svld2_vnum (p0, x0, -1))

/*
** ld2_vnum_s16_m2:
**	ld2h	{z0\.h(?: - |, )z1\.h}, p0/z, \[x0, #-2, mul vl\]
**	ret
*/
TEST_LOAD (ld2_vnum_s16_m2, svint16x2_t, int16_t,
	   z0 = svld2_vnum_s16 (p0, x0, -2),
	   z0 = svld2_vnum (p0, x0, -2))

/*
** ld2_vnum_s16_m16:
**	ld2h	{z0\.h(?: - |, )z1\.h}, p0/z, \[x0, #-16, mul vl\]
**	ret
*/
TEST_LOAD (ld2_vnum_s16_m16, svint16x2_t, int16_t,
	   z0 = svld2_vnum_s16 (p0, x0, -16),
	   z0 = svld2_vnum (p0, x0, -16))

/*
** ld2_vnum_s16_m18:
**	addvl	(x[0-9]+), x0, #-18
**	ld2h	{z0\.h(?: - |, )z1\.h}, p0/z, \[\1\]
**	ret
*/
TEST_LOAD (ld2_vnum_s16_m18, svint16x2_t, int16_t,
	   z0 = svld2_vnum_s16 (p0, x0, -18),
	   z0 = svld2_vnum (p0, x0, -18))

/* Using MUL to calculate an index would also be OK.  */
/*
** ld2_vnum_s16_x1:
**	cntb	(x[0-9]+)
**	madd	(x[0-9]+), (x1, \1|\1, x1), x0
**	ld2h	{z0\.h(?: - |, )z1\.h}, p0/z, \[\2\]
**	ret
*/
TEST_LOAD (ld2_vnum_s16_x1, svint16x2_t, int16_t,
	   z0 = svld2_vnum_s16 (p0, x0, x1),
	   z0 = svld2_vnum (p0, x0, x1))
