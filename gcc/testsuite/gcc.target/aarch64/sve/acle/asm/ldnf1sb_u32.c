/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" } } */

#include "test_sve_acle.h"

/*
** ldnf1sb_u32_base:
**	ldnf1sb	z0\.s, p0/z, \[x0\]
**	ret
*/
TEST_LOAD (ldnf1sb_u32_base, svuint32_t, int8_t,
	   z0 = svldnf1sb_u32 (p0, x0),
	   z0 = svldnf1sb_u32 (p0, x0))

/*
** ldnf1sb_u32_index:
**	add	(x[0-9]+), x0, x1
**	ldnf1sb	z0\.s, p0/z, \[\1\]
**	ret
*/
TEST_LOAD (ldnf1sb_u32_index, svuint32_t, int8_t,
	   z0 = svldnf1sb_u32 (p0, x0 + x1),
	   z0 = svldnf1sb_u32 (p0, x0 + x1))

/*
** ldnf1sb_u32_1:
**	ldnf1sb	z0\.s, p0/z, \[x0, #1, mul vl\]
**	ret
*/
TEST_LOAD (ldnf1sb_u32_1, svuint32_t, int8_t,
	   z0 = svldnf1sb_u32 (p0, x0 + svcntw ()),
	   z0 = svldnf1sb_u32 (p0, x0 + svcntw ()))

/*
** ldnf1sb_u32_7:
**	ldnf1sb	z0\.s, p0/z, \[x0, #7, mul vl\]
**	ret
*/
TEST_LOAD (ldnf1sb_u32_7, svuint32_t, int8_t,
	   z0 = svldnf1sb_u32 (p0, x0 + svcntw () * 7),
	   z0 = svldnf1sb_u32 (p0, x0 + svcntw () * 7))

/*
** ldnf1sb_u32_8:
**	incb	x0, all, mul #2
**	ldnf1sb	z0\.s, p0/z, \[x0\]
**	ret
*/
TEST_LOAD (ldnf1sb_u32_8, svuint32_t, int8_t,
	   z0 = svldnf1sb_u32 (p0, x0 + svcntw () * 8),
	   z0 = svldnf1sb_u32 (p0, x0 + svcntw () * 8))

/*
** ldnf1sb_u32_m1:
**	ldnf1sb	z0\.s, p0/z, \[x0, #-1, mul vl\]
**	ret
*/
TEST_LOAD (ldnf1sb_u32_m1, svuint32_t, int8_t,
	   z0 = svldnf1sb_u32 (p0, x0 - svcntw ()),
	   z0 = svldnf1sb_u32 (p0, x0 - svcntw ()))

/*
** ldnf1sb_u32_m8:
**	ldnf1sb	z0\.s, p0/z, \[x0, #-8, mul vl\]
**	ret
*/
TEST_LOAD (ldnf1sb_u32_m8, svuint32_t, int8_t,
	   z0 = svldnf1sb_u32 (p0, x0 - svcntw () * 8),
	   z0 = svldnf1sb_u32 (p0, x0 - svcntw () * 8))

/*
** ldnf1sb_u32_m9:
**	decw	x0, all, mul #9
**	ldnf1sb	z0\.s, p0/z, \[x0\]
**	ret
*/
TEST_LOAD (ldnf1sb_u32_m9, svuint32_t, int8_t,
	   z0 = svldnf1sb_u32 (p0, x0 - svcntw () * 9),
	   z0 = svldnf1sb_u32 (p0, x0 - svcntw () * 9))

/*
** ldnf1sb_vnum_u32_0:
**	ldnf1sb	z0\.s, p0/z, \[x0\]
**	ret
*/
TEST_LOAD (ldnf1sb_vnum_u32_0, svuint32_t, int8_t,
	   z0 = svldnf1sb_vnum_u32 (p0, x0, 0),
	   z0 = svldnf1sb_vnum_u32 (p0, x0, 0))

/*
** ldnf1sb_vnum_u32_1:
**	ldnf1sb	z0\.s, p0/z, \[x0, #1, mul vl\]
**	ret
*/
TEST_LOAD (ldnf1sb_vnum_u32_1, svuint32_t, int8_t,
	   z0 = svldnf1sb_vnum_u32 (p0, x0, 1),
	   z0 = svldnf1sb_vnum_u32 (p0, x0, 1))

/*
** ldnf1sb_vnum_u32_7:
**	ldnf1sb	z0\.s, p0/z, \[x0, #7, mul vl\]
**	ret
*/
TEST_LOAD (ldnf1sb_vnum_u32_7, svuint32_t, int8_t,
	   z0 = svldnf1sb_vnum_u32 (p0, x0, 7),
	   z0 = svldnf1sb_vnum_u32 (p0, x0, 7))

/*
** ldnf1sb_vnum_u32_8:
**	incb	x0, all, mul #2
**	ldnf1sb	z0\.s, p0/z, \[x0\]
**	ret
*/
TEST_LOAD (ldnf1sb_vnum_u32_8, svuint32_t, int8_t,
	   z0 = svldnf1sb_vnum_u32 (p0, x0, 8),
	   z0 = svldnf1sb_vnum_u32 (p0, x0, 8))

/*
** ldnf1sb_vnum_u32_m1:
**	ldnf1sb	z0\.s, p0/z, \[x0, #-1, mul vl\]
**	ret
*/
TEST_LOAD (ldnf1sb_vnum_u32_m1, svuint32_t, int8_t,
	   z0 = svldnf1sb_vnum_u32 (p0, x0, -1),
	   z0 = svldnf1sb_vnum_u32 (p0, x0, -1))

/*
** ldnf1sb_vnum_u32_m8:
**	ldnf1sb	z0\.s, p0/z, \[x0, #-8, mul vl\]
**	ret
*/
TEST_LOAD (ldnf1sb_vnum_u32_m8, svuint32_t, int8_t,
	   z0 = svldnf1sb_vnum_u32 (p0, x0, -8),
	   z0 = svldnf1sb_vnum_u32 (p0, x0, -8))

/*
** ldnf1sb_vnum_u32_m9:
**	decw	x0, all, mul #9
**	ldnf1sb	z0\.s, p0/z, \[x0\]
**	ret
*/
TEST_LOAD (ldnf1sb_vnum_u32_m9, svuint32_t, int8_t,
	   z0 = svldnf1sb_vnum_u32 (p0, x0, -9),
	   z0 = svldnf1sb_vnum_u32 (p0, x0, -9))

/*
** ldnf1sb_vnum_u32_x1:
**	cntw	(x[0-9]+)
**	madd	(x[0-9]+), (?:x1, \1|\1, x1), x0
**	ldnf1sb	z0\.s, p0/z, \[\2\]
**	ret
*/
TEST_LOAD (ldnf1sb_vnum_u32_x1, svuint32_t, int8_t,
	   z0 = svldnf1sb_vnum_u32 (p0, x0, x1),
	   z0 = svldnf1sb_vnum_u32 (p0, x0, x1))
