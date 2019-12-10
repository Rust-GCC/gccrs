/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" } } */

#include "test_sve_acle.h"

/*
** ld2_u32_base:
**	ld2w	{z0\.s(?: - |, )z1\.s}, p0/z, \[x0\]
**	ret
*/
TEST_LOAD (ld2_u32_base, svuint32x2_t, uint32_t,
	   z0 = svld2_u32 (p0, x0),
	   z0 = svld2 (p0, x0))

/*
** ld2_u32_index:
**	ld2w	{z0\.s(?: - |, )z1\.s}, p0/z, \[x0, x1, lsl 2\]
**	ret
*/
TEST_LOAD (ld2_u32_index, svuint32x2_t, uint32_t,
	   z0 = svld2_u32 (p0, x0 + x1),
	   z0 = svld2 (p0, x0 + x1))

/* Moving the constant into a register would also be OK.  */
/*
** ld2_u32_1:
**	incb	x0
**	ld2w	{z0\.s(?: - |, )z1\.s}, p0/z, \[x0\]
**	ret
*/
TEST_LOAD (ld2_u32_1, svuint32x2_t, uint32_t,
	   z0 = svld2_u32 (p0, x0 + svcntw ()),
	   z0 = svld2 (p0, x0 + svcntw ()))

/*
** ld2_u32_2:
**	ld2w	{z0\.s(?: - |, )z1\.s}, p0/z, \[x0, #2, mul vl\]
**	ret
*/
TEST_LOAD (ld2_u32_2, svuint32x2_t, uint32_t,
	   z0 = svld2_u32 (p0, x0 + svcntw () * 2),
	   z0 = svld2 (p0, x0 + svcntw () * 2))

/*
** ld2_u32_14:
**	ld2w	{z0\.s(?: - |, )z1\.s}, p0/z, \[x0, #14, mul vl\]
**	ret
*/
TEST_LOAD (ld2_u32_14, svuint32x2_t, uint32_t,
	   z0 = svld2_u32 (p0, x0 + svcntw () * 14),
	   z0 = svld2 (p0, x0 + svcntw () * 14))

/* Moving the constant into a register would also be OK.  */
/*
** ld2_u32_16:
**	incb	x0, all, mul #16
**	ld2w	{z0\.s(?: - |, )z1\.s}, p0/z, \[x0\]
**	ret
*/
TEST_LOAD (ld2_u32_16, svuint32x2_t, uint32_t,
	   z0 = svld2_u32 (p0, x0 + svcntw () * 16),
	   z0 = svld2 (p0, x0 + svcntw () * 16))

/* Moving the constant into a register would also be OK.  */
/*
** ld2_u32_m1:
**	decb	x0
**	ld2w	{z0\.s(?: - |, )z1\.s}, p0/z, \[x0\]
**	ret
*/
TEST_LOAD (ld2_u32_m1, svuint32x2_t, uint32_t,
	   z0 = svld2_u32 (p0, x0 - svcntw ()),
	   z0 = svld2 (p0, x0 - svcntw ()))

/*
** ld2_u32_m2:
**	ld2w	{z0\.s(?: - |, )z1\.s}, p0/z, \[x0, #-2, mul vl\]
**	ret
*/
TEST_LOAD (ld2_u32_m2, svuint32x2_t, uint32_t,
	   z0 = svld2_u32 (p0, x0 - svcntw () * 2),
	   z0 = svld2 (p0, x0 - svcntw () * 2))

/*
** ld2_u32_m16:
**	ld2w	{z0\.s(?: - |, )z1\.s}, p0/z, \[x0, #-16, mul vl\]
**	ret
*/
TEST_LOAD (ld2_u32_m16, svuint32x2_t, uint32_t,
	   z0 = svld2_u32 (p0, x0 - svcntw () * 16),
	   z0 = svld2 (p0, x0 - svcntw () * 16))

/*
** ld2_u32_m18:
**	addvl	(x[0-9]+), x0, #-18
**	ld2w	{z0\.s(?: - |, )z1\.s}, p0/z, \[\1\]
**	ret
*/
TEST_LOAD (ld2_u32_m18, svuint32x2_t, uint32_t,
	   z0 = svld2_u32 (p0, x0 - svcntw () * 18),
	   z0 = svld2 (p0, x0 - svcntw () * 18))

/*
** ld2_vnum_u32_0:
**	ld2w	{z0\.s(?: - |, )z1\.s}, p0/z, \[x0\]
**	ret
*/
TEST_LOAD (ld2_vnum_u32_0, svuint32x2_t, uint32_t,
	   z0 = svld2_vnum_u32 (p0, x0, 0),
	   z0 = svld2_vnum (p0, x0, 0))

/* Moving the constant into a register would also be OK.  */
/*
** ld2_vnum_u32_1:
**	incb	x0
**	ld2w	{z0\.s(?: - |, )z1\.s}, p0/z, \[x0\]
**	ret
*/
TEST_LOAD (ld2_vnum_u32_1, svuint32x2_t, uint32_t,
	   z0 = svld2_vnum_u32 (p0, x0, 1),
	   z0 = svld2_vnum (p0, x0, 1))

/*
** ld2_vnum_u32_2:
**	ld2w	{z0\.s(?: - |, )z1\.s}, p0/z, \[x0, #2, mul vl\]
**	ret
*/
TEST_LOAD (ld2_vnum_u32_2, svuint32x2_t, uint32_t,
	   z0 = svld2_vnum_u32 (p0, x0, 2),
	   z0 = svld2_vnum (p0, x0, 2))

/*
** ld2_vnum_u32_14:
**	ld2w	{z0\.s(?: - |, )z1\.s}, p0/z, \[x0, #14, mul vl\]
**	ret
*/
TEST_LOAD (ld2_vnum_u32_14, svuint32x2_t, uint32_t,
	   z0 = svld2_vnum_u32 (p0, x0, 14),
	   z0 = svld2_vnum (p0, x0, 14))

/* Moving the constant into a register would also be OK.  */
/*
** ld2_vnum_u32_16:
**	incb	x0, all, mul #16
**	ld2w	{z0\.s(?: - |, )z1\.s}, p0/z, \[x0\]
**	ret
*/
TEST_LOAD (ld2_vnum_u32_16, svuint32x2_t, uint32_t,
	   z0 = svld2_vnum_u32 (p0, x0, 16),
	   z0 = svld2_vnum (p0, x0, 16))

/* Moving the constant into a register would also be OK.  */
/*
** ld2_vnum_u32_m1:
**	decb	x0
**	ld2w	{z0\.s(?: - |, )z1\.s}, p0/z, \[x0\]
**	ret
*/
TEST_LOAD (ld2_vnum_u32_m1, svuint32x2_t, uint32_t,
	   z0 = svld2_vnum_u32 (p0, x0, -1),
	   z0 = svld2_vnum (p0, x0, -1))

/*
** ld2_vnum_u32_m2:
**	ld2w	{z0\.s(?: - |, )z1\.s}, p0/z, \[x0, #-2, mul vl\]
**	ret
*/
TEST_LOAD (ld2_vnum_u32_m2, svuint32x2_t, uint32_t,
	   z0 = svld2_vnum_u32 (p0, x0, -2),
	   z0 = svld2_vnum (p0, x0, -2))

/*
** ld2_vnum_u32_m16:
**	ld2w	{z0\.s(?: - |, )z1\.s}, p0/z, \[x0, #-16, mul vl\]
**	ret
*/
TEST_LOAD (ld2_vnum_u32_m16, svuint32x2_t, uint32_t,
	   z0 = svld2_vnum_u32 (p0, x0, -16),
	   z0 = svld2_vnum (p0, x0, -16))

/*
** ld2_vnum_u32_m18:
**	addvl	(x[0-9]+), x0, #-18
**	ld2w	{z0\.s(?: - |, )z1\.s}, p0/z, \[\1\]
**	ret
*/
TEST_LOAD (ld2_vnum_u32_m18, svuint32x2_t, uint32_t,
	   z0 = svld2_vnum_u32 (p0, x0, -18),
	   z0 = svld2_vnum (p0, x0, -18))

/* Using MUL to calculate an index would also be OK.  */
/*
** ld2_vnum_u32_x1:
**	cntb	(x[0-9]+)
**	madd	(x[0-9]+), (x1, \1|\1, x1), x0
**	ld2w	{z0\.s(?: - |, )z1\.s}, p0/z, \[\2\]
**	ret
*/
TEST_LOAD (ld2_vnum_u32_x1, svuint32x2_t, uint32_t,
	   z0 = svld2_vnum_u32 (p0, x0, x1),
	   z0 = svld2_vnum (p0, x0, x1))
