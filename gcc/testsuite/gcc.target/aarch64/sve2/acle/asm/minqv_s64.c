/* { dg-do assemble { target aarch64_asm_sve2p1_ok } } */
/* { dg-do compile { target { ! aarch64_asm_sve2p1_ok } } } */
/* { dg-skip-if "" { *-*-* } { "-DSTREAMING_COMPATIBLE" } { "" } } */
/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" } } */

#include "test_sve_acle.h"

#pragma GCC target "+sve2p1"

/*
** minqv_d0_s64_tied:
**	sminqv	v0\.2d, p0, z0\.d
**	ret
*/
TEST_REDUCTION_D (minqv_d0_s64_tied, int64x2_t, svint64_t,
		  d0 = svminqv_s64 (p0, z0),
		  d0 = svminqv (p0, z0))

/*
** minqv_d0_s64_untied:
**	sminqv	v0\.2d, p0, z1\.d
**	ret
*/
TEST_REDUCTION_D (minqv_d0_s64_untied, int64x2_t, svint64_t,
		  d0 = svminqv_s64 (p0, z1),
		  d0 = svminqv (p0, z1))
