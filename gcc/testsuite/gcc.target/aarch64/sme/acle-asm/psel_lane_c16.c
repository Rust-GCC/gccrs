/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" } } */

#include "test_sme_acle.h"

/*
** psel_lane_p0_p2_p7_0:
**	mov	[wx](1[2-5]), #?0
**	psel	p0, p2, p7\.h\[w\1, 0\]
**	ret
*/
TEST_SELECT_P (psel_lane_p0_p2_p7_0, svcount_t,
	       p0 = svpsel_lane_c16 (p2, p7, 0),
	       p0 = svpsel_lane_c16 (p2, p7, 0))

/*
** psel_lane_p2_p0_p8_w11:
**	mov	[wx](1[2-5]), [wx]11
**	psel	p2, p0, p8\.h\[w\1, 0\]
**	ret
*/
TEST_SELECT_P (psel_lane_p2_p0_p8_w11, svcount_t,
	       p2 = svpsel_lane_c16 (p0, p8, w11),
	       p2 = svpsel_lane_c16 (p0, p8, w11))

/*
** psel_lane_p2_p13_p15_w12:
**	psel	p2, p13, p15\.h\[w12, 0\]
**	ret
*/
TEST_SELECT_P (psel_lane_p2_p13_p15_w12, svcount_t,
	       p2 = svpsel_lane_c16 (p13, p15, w12),
	       p2 = svpsel_lane_c16 (p13, p15, w12))

/*
** psel_lane_p0_p13_p15_w15:
**	psel	p0, p13, p15\.h\[w15, 0\]
**	ret
*/
TEST_SELECT_P (psel_lane_p0_p13_p15_w15, svcount_t,
	       p0 = svpsel_lane_c16 (p13, p15, w15),
	       p0 = svpsel_lane_c16 (p13, p15, w15))

/*
** psel_lane_p13_p0_p15_w16:
**	mov	[wx](1[2-5]), [wx]16
**	psel	p13, p0, p15\.h\[w\1, 0\]
**	ret
*/
TEST_SELECT_P (psel_lane_p13_p0_p15_w16, svcount_t,
	       p13 = svpsel_lane_c16 (p0, p15, w16),
	       p13 = svpsel_lane_c16 (p0, p15, w16))

/*
** psel_lane_p2_p13_p8_w12p1:
**	psel	p2, p13, p8\.h\[w12, 1\]
**	ret
*/
TEST_SELECT_P (psel_lane_p2_p13_p8_w12p1, svcount_t,
	       p2 = svpsel_lane_c16 (p13, p8, w12 + 1),
	       p2 = svpsel_lane_c16 (p13, p8, w12 + 1))

/*
** psel_lane_p13_p0_p7_w12p7:
**	psel	p13, p0, p7\.h\[w12, 7\]
**	ret
*/
TEST_SELECT_P (psel_lane_p13_p0_p7_w12p7, svcount_t,
	       p13 = svpsel_lane_c16 (p0, p7, w12 + 7),
	       p13 = svpsel_lane_c16 (p0, p7, w12 + 7))

/*
** psel_lane_p0_p0_p15_w12p8:
**	add	(w[0-9]+), w12, #?8
**	psel	p0, p0, p15\.h\[\1, 0\]
**	ret
*/
TEST_SELECT_P (psel_lane_p0_p0_p15_w12p8, svcount_t,
	       p0 = svpsel_lane_c16 (p0, p15, w12 + 8),
	       p0 = svpsel_lane_c16 (p0, p15, w12 + 8))

/*
** psel_lane_p13_p13_p7_w12m1:
**	sub	(w[0-9]+), w12, #?1
**	psel	p13, p13, p7\.h\[\1, 0\]
**	ret
*/
TEST_SELECT_P (psel_lane_p13_p13_p7_w12m1, svcount_t,
	       p13 = svpsel_lane_c16 (p13, p7, w12 - 1),
	       p13 = svpsel_lane_c16 (p13, p7, w12 - 1))
