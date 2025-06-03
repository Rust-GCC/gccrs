/* { dg-do assemble { target aarch64_asm_sve2p1_ok } } */
/* { dg-do compile { target { ! aarch64_asm_sve2p1_ok } } } */
/* { dg-skip-if "" { *-*-* } { "-DSTREAMING_COMPATIBLE" } { "" } } */
/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" } } */

#include "test_sve_acle.h"

#pragma GCC target "+sve2p1"

/*
** ld2q_s32_base:
**	ld2q	{z0\.q(?: - |, )z1\.q}, p0/z, \[x0\]
**	ret
*/
TEST_LOAD (ld2q_s32_base, svint32x2_t, int32_t,
	   z0 = svld2q_s32 (p0, x0),
	   z0 = svld2q (p0, x0))

/*
** ld2q_s32_index:
**	add	(x[0-9]), x0, x1, lsl #?2
**	ld2q	{z0\.q(?: - |, )z1\.q}, p0/z, \[\1\]
**	ret
*/
TEST_LOAD (ld2q_s32_index, svint32x2_t, int32_t,
	   z0 = svld2q_s32 (p0, x0 + x1),
	   z0 = svld2q (p0, x0 + x1))

/*
** ld2q_s32_index2:
**	add	(x[0-9]), x0, x1, lsl #?3
**	ld2q	{z0\.q(?: - |, )z1\.q}, p0/z, \[\1\]
**	ret
*/
TEST_LOAD (ld2q_s32_index2, svint32x2_t, int32_t,
	   z0 = svld2q_s32 (p0, x0 + x1 * 2),
	   z0 = svld2q (p0, x0 + x1 * 2))

/*
** ld2q_s32_index4:
**	ld2q	{z0\.q(?: - |, )z1\.q}, p0/z, \[x0, x1, lsl #?4\]
**	ret
*/
TEST_LOAD (ld2q_s32_index4, svint32x2_t, int32_t,
	   z0 = svld2q_s32 (p0, x0 + x1 * 4),
	   z0 = svld2q (p0, x0 + x1 * 4))

/* Moving the constant into a register would also be OK.  */
/*
** ld2q_s32_1:
**	incb	x0
**	ld2q	{z0\.q(?: - |, )z1\.q}, p0/z, \[x0\]
**	ret
*/
TEST_LOAD (ld2q_s32_1, svint32x2_t, int32_t,
	   z0 = svld2q_s32 (p0, x0 + svcntw ()),
	   z0 = svld2q (p0, x0 + svcntw ()))

/*
** ld2q_s32_2:
**	ld2q	{z0\.q(?: - |, )z1\.q}, p0/z, \[x0, #2, mul vl\]
**	ret
*/
TEST_LOAD (ld2q_s32_2, svint32x2_t, int32_t,
	   z0 = svld2q_s32 (p0, x0 + svcntw () * 2),
	   z0 = svld2q (p0, x0 + svcntw () * 2))

/*
** ld2q_s32_14:
**	ld2q	{z0\.q(?: - |, )z1\.q}, p0/z, \[x0, #14, mul vl\]
**	ret
*/
TEST_LOAD (ld2q_s32_14, svint32x2_t, int32_t,
	   z0 = svld2q_s32 (p0, x0 + svcntw () * 14),
	   z0 = svld2q (p0, x0 + svcntw () * 14))

/* Moving the constant into a register would also be OK.  */
/*
** ld2q_s32_16:
**	incb	x0, all, mul #16
**	ld2q	{z0\.q(?: - |, )z1\.q}, p0/z, \[x0\]
**	ret
*/
TEST_LOAD (ld2q_s32_16, svint32x2_t, int32_t,
	   z0 = svld2q_s32 (p0, x0 + svcntw () * 16),
	   z0 = svld2q (p0, x0 + svcntw () * 16))

/* Moving the constant into a register would also be OK.  */
/*
** ld2q_s32_m1:
**	decb	x0
**	ld2q	{z0\.q(?: - |, )z1\.q}, p0/z, \[x0\]
**	ret
*/
TEST_LOAD (ld2q_s32_m1, svint32x2_t, int32_t,
	   z0 = svld2q_s32 (p0, x0 - svcntw ()),
	   z0 = svld2q (p0, x0 - svcntw ()))

/*
** ld2q_s32_m2:
**	ld2q	{z0\.q(?: - |, )z1\.q}, p0/z, \[x0, #-2, mul vl\]
**	ret
*/
TEST_LOAD (ld2q_s32_m2, svint32x2_t, int32_t,
	   z0 = svld2q_s32 (p0, x0 - svcntw () * 2),
	   z0 = svld2q (p0, x0 - svcntw () * 2))

/*
** ld2q_s32_m16:
**	ld2q	{z0\.q(?: - |, )z1\.q}, p0/z, \[x0, #-16, mul vl\]
**	ret
*/
TEST_LOAD (ld2q_s32_m16, svint32x2_t, int32_t,
	   z0 = svld2q_s32 (p0, x0 - svcntw () * 16),
	   z0 = svld2q (p0, x0 - svcntw () * 16))

/*
** ld2q_s32_m18:
**	addvl	(x[0-9]+), x0, #-18
**	ld2q	{z0\.q(?: - |, )z1\.q}, p0/z, \[\1\]
**	ret
*/
TEST_LOAD (ld2q_s32_m18, svint32x2_t, int32_t,
	   z0 = svld2q_s32 (p0, x0 - svcntw () * 18),
	   z0 = svld2q (p0, x0 - svcntw () * 18))

/*
** ld2q_vnum_s32_0:
**	ld2q	{z0\.q(?: - |, )z1\.q}, p0/z, \[x0\]
**	ret
*/
TEST_LOAD (ld2q_vnum_s32_0, svint32x2_t, int32_t,
	   z0 = svld2q_vnum_s32 (p0, x0, 0),
	   z0 = svld2q_vnum (p0, x0, 0))

/* Moving the constant into a register would also be OK.  */
/*
** ld2q_vnum_s32_1:
**	incb	x0
**	ld2q	{z0\.q(?: - |, )z1\.q}, p0/z, \[x0\]
**	ret
*/
TEST_LOAD (ld2q_vnum_s32_1, svint32x2_t, int32_t,
	   z0 = svld2q_vnum_s32 (p0, x0, 1),
	   z0 = svld2q_vnum (p0, x0, 1))

/*
** ld2q_vnum_s32_2:
**	ld2q	{z0\.q(?: - |, )z1\.q}, p0/z, \[x0, #2, mul vl\]
**	ret
*/
TEST_LOAD (ld2q_vnum_s32_2, svint32x2_t, int32_t,
	   z0 = svld2q_vnum_s32 (p0, x0, 2),
	   z0 = svld2q_vnum (p0, x0, 2))

/*
** ld2q_vnum_s32_14:
**	ld2q	{z0\.q(?: - |, )z1\.q}, p0/z, \[x0, #14, mul vl\]
**	ret
*/
TEST_LOAD (ld2q_vnum_s32_14, svint32x2_t, int32_t,
	   z0 = svld2q_vnum_s32 (p0, x0, 14),
	   z0 = svld2q_vnum (p0, x0, 14))

/* Moving the constant into a register would also be OK.  */
/*
** ld2q_vnum_s32_16:
**	incb	x0, all, mul #16
**	ld2q	{z0\.q(?: - |, )z1\.q}, p0/z, \[x0\]
**	ret
*/
TEST_LOAD (ld2q_vnum_s32_16, svint32x2_t, int32_t,
	   z0 = svld2q_vnum_s32 (p0, x0, 16),
	   z0 = svld2q_vnum (p0, x0, 16))

/* Moving the constant into a register would also be OK.  */
/*
** ld2q_vnum_s32_m1:
**	decb	x0
**	ld2q	{z0\.q(?: - |, )z1\.q}, p0/z, \[x0\]
**	ret
*/
TEST_LOAD (ld2q_vnum_s32_m1, svint32x2_t, int32_t,
	   z0 = svld2q_vnum_s32 (p0, x0, -1),
	   z0 = svld2q_vnum (p0, x0, -1))

/*
** ld2q_vnum_s32_m2:
**	ld2q	{z0\.q(?: - |, )z1\.q}, p0/z, \[x0, #-2, mul vl\]
**	ret
*/
TEST_LOAD (ld2q_vnum_s32_m2, svint32x2_t, int32_t,
	   z0 = svld2q_vnum_s32 (p0, x0, -2),
	   z0 = svld2q_vnum (p0, x0, -2))

/*
** ld2q_vnum_s32_m16:
**	ld2q	{z0\.q(?: - |, )z1\.q}, p0/z, \[x0, #-16, mul vl\]
**	ret
*/
TEST_LOAD (ld2q_vnum_s32_m16, svint32x2_t, int32_t,
	   z0 = svld2q_vnum_s32 (p0, x0, -16),
	   z0 = svld2q_vnum (p0, x0, -16))

/*
** ld2q_vnum_s32_m18:
**	addvl	(x[0-9]+), x0, #-18
**	ld2q	{z0\.q(?: - |, )z1\.q}, p0/z, \[\1\]
**	ret
*/
TEST_LOAD (ld2q_vnum_s32_m18, svint32x2_t, int32_t,
	   z0 = svld2q_vnum_s32 (p0, x0, -18),
	   z0 = svld2q_vnum (p0, x0, -18))

/*
** ld2q_vnum_s32_x1:
**	cntb	(x[0-9]+)
**	madd	(x[0-9]+), (?:x1, \1|\1, x1), x0
**	ld2q	{z0\.q(?: - |, )z1\.q}, p0/z, \[\2\]
**	ret
*/
TEST_LOAD (ld2q_vnum_s32_x1, svint32x2_t, int32_t,
	   z0 = svld2q_vnum_s32 (p0, x0, x1),
	   z0 = svld2q_vnum (p0, x0, x1))
