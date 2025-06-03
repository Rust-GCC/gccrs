/* { dg-do assemble { target aarch64_asm_lut_ok } } */
/* { dg-do compile { target { ! aarch64_asm_lut_ok } } } */
/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" } } */

#include "test_sve_acle.h"

#pragma GCC target "+sve2+lut"
#if STREAMING_COMPATIBLE
#pragma GCC target "+sme2"
#endif

/*
** luti2_min_idx_test:
**	 luti2	z1\.b, \{ z28\.b \}, z0\[0\]
**	 ret
*/

TEST_XN_SINGLE (luti2_min_idx_test, svint8_t, svuint8_t, z1,
                svluti2_lane_s8 (z28, z0, 0),
                svluti2_lane (z28, z0, 0))

/*
** luti2_max_idx_test:
**	 luti2	z1\.b, \{ z28\.b \}, z0\[3\]
**	 ret
*/

TEST_XN_SINGLE (luti2_max_idx_test, svint8_t, svuint8_t, z1,
                svluti2_lane_s8 (z28, z0, 3),
                svluti2_lane (z28, z0, 3))

/*
** luti2_tied_min_idx_test:
**	 luti2	z28\.b, \{ z28\.b \}, z0\[0\]
**	 ret
*/

TEST_XN_SINGLE (luti2_tied_min_idx_test, svint8_t, svuint8_t, z28,
                svluti2_lane_s8 (z28, z0, 0),
                svluti2_lane (z28, z0, 0))

/*
** luti2_tied_max_idx_test:
**	 luti2	z28\.b, \{ z28\.b \}, z0\[3\]
**	 ret
*/

TEST_XN_SINGLE (luti2_tied_max_idx_test, svint8_t, svuint8_t, z28,
                svluti2_lane_s8 (z28, z0, 3),
                svluti2_lane (z28, z0, 3))
