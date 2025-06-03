/* { dg-do assemble { target aarch64_asm_sve2p1_ok } } */
/* { dg-do compile { target { ! aarch64_asm_sve2p1_ok } } } */
/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" { target { ! ilp32 } } } } */

#include "test_sve_acle.h"

#pragma GCC target "+sve2p1"
#ifdef STREAMING_COMPATIBLE
#pragma GCC target "+sme2"
#endif

/*
** stnt1_bf16_base:
**	stnt1h	{z0\.h(?: - |, )z1\.h}, pn8, \[x0\]
**	ret
*/
TEST_STORE_COUNT (stnt1_bf16_base, svbfloat16x2_t, bfloat16_t,
		  svstnt1_bf16_x2 (pn8, x0, z0),
		  svstnt1 (pn8, x0, z0))

/*
** stnt1_bf16_index:
**	stnt1h	{z0\.h(?: - |, )z1\.h}, pn8, \[x0, x1, lsl #?1\]
**	ret
*/
TEST_STORE_COUNT (stnt1_bf16_index, svbfloat16x2_t, bfloat16_t,
		  svstnt1_bf16_x2 (pn8, x0 + x1, z0),
		  svstnt1 (pn8, x0 + x1, z0))

/* Moving the constant into a register would also be OK.  */
/*
** stnt1_bf16_1:
**	incb	x0
**	stnt1h	{z0\.h(?: - |, )z1\.h}, pn8, \[x0\]
**	ret
*/
TEST_STORE_COUNT (stnt1_bf16_1, svbfloat16x2_t, bfloat16_t,
		  svstnt1_bf16_x2 (pn8, x0 + svcnth (), z0),
		  svstnt1 (pn8, x0 + svcnth (), z0))

/*
** stnt1_bf16_2:
**	stnt1h	{z0\.h(?: - |, )z1\.h}, pn8, \[x0, #2, mul vl\]
**	ret
*/
TEST_STORE_COUNT (stnt1_bf16_2, svbfloat16x2_t, bfloat16_t,
		  svstnt1_bf16_x2 (pn8, x0 + svcnth () * 2, z0),
		  svstnt1 (pn8, x0 + svcnth () * 2, z0))

/*
** stnt1_bf16_14:
**	stnt1h	{z0\.h(?: - |, )z1\.h}, pn8, \[x0, #14, mul vl\]
**	ret
*/
TEST_STORE_COUNT (stnt1_bf16_14, svbfloat16x2_t, bfloat16_t,
		  svstnt1_bf16_x2 (pn8, x0 + svcnth () * 14, z0),
		  svstnt1 (pn8, x0 + svcnth () * 14, z0))

/* Moving the constant into a register would also be OK.  */
/*
** stnt1_bf16_16:
**	incb	x0, all, mul #16
**	stnt1h	{z0\.h(?: - |, )z1\.h}, pn8, \[x0\]
**	ret
*/
TEST_STORE_COUNT (stnt1_bf16_16, svbfloat16x2_t, bfloat16_t,
		  svstnt1_bf16_x2 (pn8, x0 + svcnth () * 16, z0),
		  svstnt1 (pn8, x0 + svcnth () * 16, z0))

/* Moving the constant into a register would also be OK.  */
/*
** stnt1_bf16_m1:
**	decb	x0
**	stnt1h	{z0\.h(?: - |, )z1\.h}, pn8, \[x0\]
**	ret
*/
TEST_STORE_COUNT (stnt1_bf16_m1, svbfloat16x2_t, bfloat16_t,
		  svstnt1_bf16_x2 (pn8, x0 - svcnth (), z0),
		  svstnt1 (pn8, x0 - svcnth (), z0))

/*
** stnt1_bf16_m2:
**	stnt1h	{z0\.h(?: - |, )z1\.h}, pn8, \[x0, #-2, mul vl\]
**	ret
*/
TEST_STORE_COUNT (stnt1_bf16_m2, svbfloat16x2_t, bfloat16_t,
		  svstnt1_bf16_x2 (pn8, x0 - svcnth () * 2, z0),
		  svstnt1 (pn8, x0 - svcnth () * 2, z0))

/*
** stnt1_bf16_m16:
**	stnt1h	{z0\.h(?: - |, )z1\.h}, pn8, \[x0, #-16, mul vl\]
**	ret
*/
TEST_STORE_COUNT (stnt1_bf16_m16, svbfloat16x2_t, bfloat16_t,
		  svstnt1_bf16_x2 (pn8, x0 - svcnth () * 16, z0),
		  svstnt1 (pn8, x0 - svcnth () * 16, z0))

/*
** stnt1_bf16_m18:
**	addvl	(x[0-9]+), x0, #-18
**	stnt1h	{z0\.h(?: - |, )z1\.h}, pn8, \[\1\]
**	ret
*/
TEST_STORE_COUNT (stnt1_bf16_m18, svbfloat16x2_t, bfloat16_t,
		  svstnt1_bf16_x2 (pn8, x0 - svcnth () * 18, z0),
		  svstnt1 (pn8, x0 - svcnth () * 18, z0))

/*
** stnt1_bf16_z17:
**	mov	[^\n]+
**	mov	[^\n]+
**	stnt1h	{z[^\n]+}, pn8, \[x0\]
**	ret
*/
TEST_STORE_COUNT (stnt1_bf16_z17, svbfloat16x2_t, bfloat16_t,
		  svstnt1_bf16_x2 (pn8, x0, z17),
		  svstnt1 (pn8, x0, z17))

/*
** stnt1_bf16_z22:
**	stnt1h	{z22\.h(?: - |, )z23\.h}, pn8, \[x0\]
**	ret
*/
TEST_STORE_COUNT (stnt1_bf16_z22, svbfloat16x2_t, bfloat16_t,
		  svstnt1_bf16_x2 (pn8, x0, z22),
		  svstnt1 (pn8, x0, z22))

/*
** stnt1_bf16_z28:
**	stnt1h	{z28\.h(?: - |, )z29\.h}, pn8, \[x0\]
**	ret
*/
TEST_STORE_COUNT (stnt1_bf16_z28, svbfloat16x2_t, bfloat16_t,
		  svstnt1_bf16_x2 (pn8, x0, z28),
		  svstnt1 (pn8, x0, z28))

/*
** stnt1_bf16_pn0:
**	mov	p([89]|1[0-5])\.b, p0\.b
**	stnt1h	{z0\.h(?: - |, )z1\.h}, pn\1, \[x0\]
**	ret
*/
TEST_STORE_COUNT (stnt1_bf16_pn0, svbfloat16x2_t, bfloat16_t,
		  svstnt1_bf16_x2 (pn0, x0, z0),
		  svstnt1 (pn0, x0, z0))

/*
** stnt1_bf16_pn7:
**	mov	p([89]|1[0-5])\.b, p7\.b
**	stnt1h	{z0\.h(?: - |, )z1\.h}, pn\1, \[x0\]
**	ret
*/
TEST_STORE_COUNT (stnt1_bf16_pn7, svbfloat16x2_t, bfloat16_t,
		  svstnt1_bf16_x2 (pn7, x0, z0),
		  svstnt1 (pn7, x0, z0))

/*
** stnt1_bf16_pn15:
**	stnt1h	{z0\.h(?: - |, )z1\.h}, pn15, \[x0\]
**	ret
*/
TEST_STORE_COUNT (stnt1_bf16_pn15, svbfloat16x2_t, bfloat16_t,
		  svstnt1_bf16_x2 (pn15, x0, z0),
		  svstnt1 (pn15, x0, z0))

/*
** stnt1_vnum_bf16_0:
**	stnt1h	{z0\.h(?: - |, )z1\.h}, pn8, \[x0\]
**	ret
*/
TEST_STORE_COUNT (stnt1_vnum_bf16_0, svbfloat16x2_t, bfloat16_t,
		  svstnt1_vnum_bf16_x2 (pn8, x0, 0, z0),
		  svstnt1_vnum (pn8, x0, 0, z0))

/* Moving the constant into a register would also be OK.  */
/*
** stnt1_vnum_bf16_1:
**	incb	x0
**	stnt1h	{z0\.h(?: - |, )z1\.h}, pn8, \[x0\]
**	ret
*/
TEST_STORE_COUNT (stnt1_vnum_bf16_1, svbfloat16x2_t, bfloat16_t,
		  svstnt1_vnum_bf16_x2 (pn8, x0, 1, z0),
		  svstnt1_vnum (pn8, x0, 1, z0))

/*
** stnt1_vnum_bf16_2:
**	stnt1h	{z0\.h(?: - |, )z1\.h}, pn8, \[x0, #2, mul vl\]
**	ret
*/
TEST_STORE_COUNT (stnt1_vnum_bf16_2, svbfloat16x2_t, bfloat16_t,
		  svstnt1_vnum_bf16_x2 (pn8, x0, 2, z0),
		  svstnt1_vnum (pn8, x0, 2, z0))

/*
** stnt1_vnum_bf16_14:
**	stnt1h	{z0\.h(?: - |, )z1\.h}, pn8, \[x0, #14, mul vl\]
**	ret
*/
TEST_STORE_COUNT (stnt1_vnum_bf16_14, svbfloat16x2_t, bfloat16_t,
		  svstnt1_vnum_bf16_x2 (pn8, x0, 14, z0),
		  svstnt1_vnum (pn8, x0, 14, z0))

/* Moving the constant into a register would also be OK.  */
/*
** stnt1_vnum_bf16_16:
**	incb	x0, all, mul #16
**	stnt1h	{z0\.h(?: - |, )z1\.h}, pn8, \[x0\]
**	ret
*/
TEST_STORE_COUNT (stnt1_vnum_bf16_16, svbfloat16x2_t, bfloat16_t,
		  svstnt1_vnum_bf16_x2 (pn8, x0, 16, z0),
		  svstnt1_vnum (pn8, x0, 16, z0))

/* Moving the constant into a register would also be OK.  */
/*
** stnt1_vnum_bf16_m1:
**	decb	x0
**	stnt1h	{z0\.h(?: - |, )z1\.h}, pn8, \[x0\]
**	ret
*/
TEST_STORE_COUNT (stnt1_vnum_bf16_m1, svbfloat16x2_t, bfloat16_t,
		  svstnt1_vnum_bf16_x2 (pn8, x0, -1, z0),
		  svstnt1_vnum (pn8, x0, -1, z0))

/*
** stnt1_vnum_bf16_m2:
**	stnt1h	{z0\.h(?: - |, )z1\.h}, pn8, \[x0, #-2, mul vl\]
**	ret
*/
TEST_STORE_COUNT (stnt1_vnum_bf16_m2, svbfloat16x2_t, bfloat16_t,
		  svstnt1_vnum_bf16_x2 (pn8, x0, -2, z0),
		  svstnt1_vnum (pn8, x0, -2, z0))

/*
** stnt1_vnum_bf16_m16:
**	stnt1h	{z0\.h(?: - |, )z1\.h}, pn8, \[x0, #-16, mul vl\]
**	ret
*/
TEST_STORE_COUNT (stnt1_vnum_bf16_m16, svbfloat16x2_t, bfloat16_t,
		  svstnt1_vnum_bf16_x2 (pn8, x0, -16, z0),
		  svstnt1_vnum (pn8, x0, -16, z0))

/*
** stnt1_vnum_bf16_m18:
**	addvl	(x[0-9]+), x0, #-18
**	stnt1h	{z0\.h(?: - |, )z1\.h}, pn8, \[\1\]
**	ret
*/
TEST_STORE_COUNT (stnt1_vnum_bf16_m18, svbfloat16x2_t, bfloat16_t,
		  svstnt1_vnum_bf16_x2 (pn8, x0, -18, z0),
		  svstnt1_vnum (pn8, x0, -18, z0))

/*
** stnt1_vnum_bf16_x1:
**	cntb	(x[0-9]+)
** (
**	madd	(x[0-9]+), (?:x1, \1|\1, x1), x0
**	stnt1h	{z0\.h(?: - |, )z1\.h}, pn8, \[\2\]
** |
**	mul	(x[0-9]+), (?:x1, \1|\1, x1)
**	stnt1h	{z0\.h(?: - |, )z1\.h}, pn8, \[x0, \3\]
** )
**	ret
*/
TEST_STORE_COUNT (stnt1_vnum_bf16_x1, svbfloat16x2_t, bfloat16_t,
		  svstnt1_vnum_bf16_x2 (pn8, x0, x1, z0),
		  svstnt1_vnum (pn8, x0, x1, z0))
