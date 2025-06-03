/* { dg-do assemble { target aarch64_asm_sve2p1_ok } } */
/* { dg-do compile { target { ! aarch64_asm_sve2p1_ok } } } */
/* { dg-skip-if "" { *-*-* } { "-DSTREAMING_COMPATIBLE" } { "" } } */
/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" } } */

#include "test_sve_acle.h"

#pragma GCC target "+sve2p1"

/*
** ld2q_bf16_base:
**	ld2q	{z0\.q(?: - |, )z1\.q}, p0/z, \[x0\]
**	ret
*/
TEST_LOAD (ld2q_bf16_base, svbfloat16x2_t, bfloat16_t,
	   z0 = svld2q_bf16 (p0, x0),
	   z0 = svld2q (p0, x0))

/*
** ld2q_bf16_index:
**	add	(x[0-9]), x0, x1, lsl #?1
**	ld2q	{z0\.q(?: - |, )z1\.q}, p0/z, \[\1\]
**	ret
*/
TEST_LOAD (ld2q_bf16_index, svbfloat16x2_t, bfloat16_t,
	   z0 = svld2q_bf16 (p0, x0 + x1),
	   z0 = svld2q (p0, x0 + x1))

/*
** ld2q_bf16_index2:
**	add	(x[0-9]), x0, x1, lsl #?2
**	ld2q	{z0\.q(?: - |, )z1\.q}, p0/z, \[\1\]
**	ret
*/
TEST_LOAD (ld2q_bf16_index2, svbfloat16x2_t, bfloat16_t,
	   z0 = svld2q_bf16 (p0, x0 + x1 * 2),
	   z0 = svld2q (p0, x0 + x1 * 2))

/*
** ld2q_bf16_index4:
**	add	(x[0-9]), x0, x1, lsl #?3
**	ld2q	{z0\.q(?: - |, )z1\.q}, p0/z, \[\1\]
**	ret
*/
TEST_LOAD (ld2q_bf16_index4, svbfloat16x2_t, bfloat16_t,
	   z0 = svld2q_bf16 (p0, x0 + x1 * 4),
	   z0 = svld2q (p0, x0 + x1 * 4))

/*
** ld2q_bf16_index8:
**	ld2q	{z0\.q(?: - |, )z1\.q}, p0/z, \[x0, x1, lsl #?4\]
**	ret
*/
TEST_LOAD (ld2q_bf16_index8, svbfloat16x2_t, bfloat16_t,
	   z0 = svld2q_bf16 (p0, x0 + x1 * 8),
	   z0 = svld2q (p0, x0 + x1 * 8))

/* Moving the constant into a register would also be OK.  */
/*
** ld2q_bf16_1:
**	incb	x0
**	ld2q	{z0\.q(?: - |, )z1\.q}, p0/z, \[x0\]
**	ret
*/
TEST_LOAD (ld2q_bf16_1, svbfloat16x2_t, bfloat16_t,
	   z0 = svld2q_bf16 (p0, x0 + svcnth ()),
	   z0 = svld2q (p0, x0 + svcnth ()))

/*
** ld2q_bf16_2:
**	ld2q	{z0\.q(?: - |, )z1\.q}, p0/z, \[x0, #2, mul vl\]
**	ret
*/
TEST_LOAD (ld2q_bf16_2, svbfloat16x2_t, bfloat16_t,
	   z0 = svld2q_bf16 (p0, x0 + svcnth () * 2),
	   z0 = svld2q (p0, x0 + svcnth () * 2))

/*
** ld2q_bf16_14:
**	ld2q	{z0\.q(?: - |, )z1\.q}, p0/z, \[x0, #14, mul vl\]
**	ret
*/
TEST_LOAD (ld2q_bf16_14, svbfloat16x2_t, bfloat16_t,
	   z0 = svld2q_bf16 (p0, x0 + svcnth () * 14),
	   z0 = svld2q (p0, x0 + svcnth () * 14))

/* Moving the constant into a register would also be OK.  */
/*
** ld2q_bf16_16:
**	incb	x0, all, mul #16
**	ld2q	{z0\.q(?: - |, )z1\.q}, p0/z, \[x0\]
**	ret
*/
TEST_LOAD (ld2q_bf16_16, svbfloat16x2_t, bfloat16_t,
	   z0 = svld2q_bf16 (p0, x0 + svcnth () * 16),
	   z0 = svld2q (p0, x0 + svcnth () * 16))

/* Moving the constant into a register would also be OK.  */
/*
** ld2q_bf16_m1:
**	decb	x0
**	ld2q	{z0\.q(?: - |, )z1\.q}, p0/z, \[x0\]
**	ret
*/
TEST_LOAD (ld2q_bf16_m1, svbfloat16x2_t, bfloat16_t,
	   z0 = svld2q_bf16 (p0, x0 - svcnth ()),
	   z0 = svld2q (p0, x0 - svcnth ()))

/*
** ld2q_bf16_m2:
**	ld2q	{z0\.q(?: - |, )z1\.q}, p0/z, \[x0, #-2, mul vl\]
**	ret
*/
TEST_LOAD (ld2q_bf16_m2, svbfloat16x2_t, bfloat16_t,
	   z0 = svld2q_bf16 (p0, x0 - svcnth () * 2),
	   z0 = svld2q (p0, x0 - svcnth () * 2))

/*
** ld2q_bf16_m16:
**	ld2q	{z0\.q(?: - |, )z1\.q}, p0/z, \[x0, #-16, mul vl\]
**	ret
*/
TEST_LOAD (ld2q_bf16_m16, svbfloat16x2_t, bfloat16_t,
	   z0 = svld2q_bf16 (p0, x0 - svcnth () * 16),
	   z0 = svld2q (p0, x0 - svcnth () * 16))

/*
** ld2q_bf16_m18:
**	addvl	(x[0-9]+), x0, #-18
**	ld2q	{z0\.q(?: - |, )z1\.q}, p0/z, \[\1\]
**	ret
*/
TEST_LOAD (ld2q_bf16_m18, svbfloat16x2_t, bfloat16_t,
	   z0 = svld2q_bf16 (p0, x0 - svcnth () * 18),
	   z0 = svld2q (p0, x0 - svcnth () * 18))

/*
** ld2q_vnum_bf16_0:
**	ld2q	{z0\.q(?: - |, )z1\.q}, p0/z, \[x0\]
**	ret
*/
TEST_LOAD (ld2q_vnum_bf16_0, svbfloat16x2_t, bfloat16_t,
	   z0 = svld2q_vnum_bf16 (p0, x0, 0),
	   z0 = svld2q_vnum (p0, x0, 0))

/* Moving the constant into a register would also be OK.  */
/*
** ld2q_vnum_bf16_1:
**	incb	x0
**	ld2q	{z0\.q(?: - |, )z1\.q}, p0/z, \[x0\]
**	ret
*/
TEST_LOAD (ld2q_vnum_bf16_1, svbfloat16x2_t, bfloat16_t,
	   z0 = svld2q_vnum_bf16 (p0, x0, 1),
	   z0 = svld2q_vnum (p0, x0, 1))

/*
** ld2q_vnum_bf16_2:
**	ld2q	{z0\.q(?: - |, )z1\.q}, p0/z, \[x0, #2, mul vl\]
**	ret
*/
TEST_LOAD (ld2q_vnum_bf16_2, svbfloat16x2_t, bfloat16_t,
	   z0 = svld2q_vnum_bf16 (p0, x0, 2),
	   z0 = svld2q_vnum (p0, x0, 2))

/*
** ld2q_vnum_bf16_14:
**	ld2q	{z0\.q(?: - |, )z1\.q}, p0/z, \[x0, #14, mul vl\]
**	ret
*/
TEST_LOAD (ld2q_vnum_bf16_14, svbfloat16x2_t, bfloat16_t,
	   z0 = svld2q_vnum_bf16 (p0, x0, 14),
	   z0 = svld2q_vnum (p0, x0, 14))

/* Moving the constant into a register would also be OK.  */
/*
** ld2q_vnum_bf16_16:
**	incb	x0, all, mul #16
**	ld2q	{z0\.q(?: - |, )z1\.q}, p0/z, \[x0\]
**	ret
*/
TEST_LOAD (ld2q_vnum_bf16_16, svbfloat16x2_t, bfloat16_t,
	   z0 = svld2q_vnum_bf16 (p0, x0, 16),
	   z0 = svld2q_vnum (p0, x0, 16))

/* Moving the constant into a register would also be OK.  */
/*
** ld2q_vnum_bf16_m1:
**	decb	x0
**	ld2q	{z0\.q(?: - |, )z1\.q}, p0/z, \[x0\]
**	ret
*/
TEST_LOAD (ld2q_vnum_bf16_m1, svbfloat16x2_t, bfloat16_t,
	   z0 = svld2q_vnum_bf16 (p0, x0, -1),
	   z0 = svld2q_vnum (p0, x0, -1))

/*
** ld2q_vnum_bf16_m2:
**	ld2q	{z0\.q(?: - |, )z1\.q}, p0/z, \[x0, #-2, mul vl\]
**	ret
*/
TEST_LOAD (ld2q_vnum_bf16_m2, svbfloat16x2_t, bfloat16_t,
	   z0 = svld2q_vnum_bf16 (p0, x0, -2),
	   z0 = svld2q_vnum (p0, x0, -2))

/*
** ld2q_vnum_bf16_m16:
**	ld2q	{z0\.q(?: - |, )z1\.q}, p0/z, \[x0, #-16, mul vl\]
**	ret
*/
TEST_LOAD (ld2q_vnum_bf16_m16, svbfloat16x2_t, bfloat16_t,
	   z0 = svld2q_vnum_bf16 (p0, x0, -16),
	   z0 = svld2q_vnum (p0, x0, -16))

/*
** ld2q_vnum_bf16_m18:
**	addvl	(x[0-9]+), x0, #-18
**	ld2q	{z0\.q(?: - |, )z1\.q}, p0/z, \[\1\]
**	ret
*/
TEST_LOAD (ld2q_vnum_bf16_m18, svbfloat16x2_t, bfloat16_t,
	   z0 = svld2q_vnum_bf16 (p0, x0, -18),
	   z0 = svld2q_vnum (p0, x0, -18))

/*
** ld2q_vnum_bf16_x1:
**	cntb	(x[0-9]+)
**	madd	(x[0-9]+), (?:x1, \1|\1, x1), x0
**	ld2q	{z0\.q(?: - |, )z1\.q}, p0/z, \[\2\]
**	ret
*/
TEST_LOAD (ld2q_vnum_bf16_x1, svbfloat16x2_t, bfloat16_t,
	   z0 = svld2q_vnum_bf16 (p0, x0, x1),
	   z0 = svld2q_vnum (p0, x0, x1))
