/* { dg-do assemble { target aarch64_asm_sve2p2_ok } } */
/* { dg-do compile { target { ! aarch64_asm_sve2p2_ok } } } */
/* { dg-additional-options "-msve-vector-bits=scalable" } */
/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" } } */

#include "test_sve_acle.h"
#include <stdbool.h>

#pragma GCC target "+sve2p2"
#ifdef STREAMING_COMPATIBLE
#pragma GCC target "+sme2p2"
#endif

/*
** firstp_b64_32:
**	firstp	x0, p0, p1\.d
**	ret
*/
TEST_PTEST (firstp_b64_32, uint32_t,
	    x0 = svfirstp_b64 (p0, p1));

/*
** firstp_b64_64:
**	firstp	x0, p0, p1\.d
**	ret
*/
TEST_PTEST (firstp_b64_64, uint64_t,
	    x0 = svfirstp_b64 (p0, p1));

/*
** firstp_inc_b64_32_general_x0:
**	firstp	x([0-9]+), p0, p1\.d
**	add	w0, (w0, w\1|w\1, w0)
**	ret
*/
TEST_PTEST (firstp_inc_b64_32_general_x0, uint32_t,
	    x0 += svfirstp_b64 (p0, p1));

/*
** firstp_inc_b64_32_general_x1:
**	firstp	x([0-9]+), p0, p1\.d
**	add	w0, (w1, w\1|w\1, w1)
**	ret
*/
TEST_PTEST (firstp_inc_b64_32_general_x1, uint32_t,
	    x0 = x1 + svfirstp_b64 (p0, p1));

/*
** firstp_inc_b64_64_general_x0:
**	firstp	(x[0-9]+), p0, p1\.d
**	add	x0, (x0, \1|\1, x0)
**	ret
*/
TEST_PTEST (firstp_inc_b64_64_general_x0, uint64_t,
	    x0 += svfirstp_b64 (p0, p1));

/*
** firstp_inc_b64_64_general_x1:
**	firstp	(x[0-9]+), p0, p1\.d
**	add	x0, (x1, \1|\1, x1)
**	ret
*/
TEST_PTEST (firstp_inc_b64_64_general_x1, uint64_t,
	    x0 = x1 + svfirstp_b64 (p0, p1));

/*
** firstp_dec_b64_32_general_x0:
**	firstp	x([0-9]+), p0, p1\.d
**	sub	w0, w0, w\1
**	ret
*/
TEST_PTEST (firstp_dec_b64_32_general_x0, uint32_t,
	    x0 -= svfirstp_b64 (p0, p1));

/*
** firstp_dec_b64_32_general_x1:
**	firstp	x([0-9]+), p0, p1\.d
**	sub	w0, w1, w\1
**	ret
*/
TEST_PTEST (firstp_dec_b64_32_general_x1, uint32_t,
	    x0 = x1 - svfirstp_b64 (p0, p1));

/*
** firstp_dec_b64_64_general_x0:
**	firstp	(x[0-9]+), p0, p1\.d
**	sub	x0, x0, \1
**	ret
*/
TEST_PTEST (firstp_dec_b64_64_general_x0, uint64_t,
	    x0 -= svfirstp_b64 (p0, p1));

/*
** firstp_dec_b64_64_general_x1:
**	firstp	(x[0-9]+), p0, p1\.d
**	sub	x0, x1, \1
**	ret
*/
TEST_PTEST (firstp_dec_b64_64_general_x1, uint64_t,
	    x0 = x1 - svfirstp_b64 (p0, p1));

/*
** firstp_inc_b64_u64_general_z0:
**	firstp	(x[0-9]+), p0, p1\.d
**	mov	(z[0-9]+\.d), \1
**	add	z0\.d, (z0\.d, \2|\2, z0\.d)
**	ret
*/
TEST_UNIFORM_Z (firstp_inc_b64_u64_general_z0, svuint64_t,
		z0 = svadd_n_u64_x (svptrue_b64 (), z0, svfirstp_b64 (p0, p1)),
		z0 = svadd_x (svptrue_b64 (), z0, svfirstp_b64 (p0, p1)));

/*
** firstp_inc_b64_u64_general_z1:
**	firstp	(x[0-9]+), p0, p1\.d
**	mov	(z[0-9]+\.d), \1
**	add	z0\.d, (z1\.d, \2|\2, z1\.d)
**	ret
*/
TEST_UNIFORM_Z (firstp_inc_b64_u64_general_z1, svuint64_t,
		z0 = svadd_n_u64_x (svptrue_b64 (), z1, svfirstp_b64 (p0, p1)),
		z0 = svadd_x (svptrue_b64 (), z1, svfirstp_b64 (p0, p1)));

/*
** firstp_inc_b64_u64_ptrue_z0:
**	ptrue	(p[0-7])\.b, all
**	firstp	(x[0-9]+), \1, p0\.d
**	mov	(z[0-9]+\.d), \2
**	add	z0\.d, (z0\.d, \3|\3, z0\.d)
**	ret
*/
TEST_UNIFORM_Z (firstp_inc_b64_u64_ptrue_z0, svuint64_t,
		z0 = svadd_n_u64_x (svptrue_b64 (), z0, svfirstp_b64 (svptrue_b64 (), p0)),
		z0 = svadd_x (svptrue_b64 (), z0, svfirstp_b64 (svptrue_b64 (), p0)));

/*
** firstp_inc_b64_u64_ptrue_z1:
**	ptrue	(p[0-7])\.b, all
**	firstp	(x[0-9]+), \1, p0\.d
**	mov	(z[0-9]+\.d), \2
**	add	z0\.d, (z1\.d, \3|\3, z1\.d)
**	ret
*/
TEST_UNIFORM_Z (firstp_inc_b64_u64_ptrue_z1, svuint64_t,
		z0 = svadd_n_u64_x (svptrue_b64 (), z1, svfirstp_b64 (svptrue_b64 (), p0)),
		z0 = svadd_x (svptrue_b64 (), z1, svfirstp_b64 (svptrue_b64 (), p0)));

/*
** firstp_dec_b64_u64_general_z0:
**	firstp	(x[0-9]+), p0, p1\.d
**	mov	(z[0-9]+\.d), \1
**	sub	z0\.d, z0\.d, \2
**	ret
*/
TEST_UNIFORM_Z (firstp_dec_b64_u64_general_z0, svuint64_t,
		z0 = svsub_n_u64_x (svptrue_b64 (), z0, svfirstp_b64 (p0, p1)),
		z0 = svsub_x (svptrue_b64 (), z0, svfirstp_b64 (p0, p1)));

/*
** firstp_dec_b64_u64_general_z1:
**	firstp	(x[0-9]+), p0, p1\.d
**	mov	(z[0-9]+\.d), \1
**	sub	z0\.d, z1\.d, \2
**	ret
*/
TEST_UNIFORM_Z (firstp_dec_b64_u64_general_z1, svuint64_t,
		z0 = svsub_n_u64_x (svptrue_b64 (), z1, svfirstp_b64 (p0, p1)),
		z0 = svsub_x (svptrue_b64 (), z1, svfirstp_b64 (p0, p1)));

/*
** firstp_dec_b64_u64_ptrue_z0:
**	ptrue	(p[0-7])\.b, all
**	firstp	(x[0-9]+), \1, p0\.d
**	mov	(z[0-9]+\.d), \2
**	sub	z0\.d, z0\.d, \3
**	ret
*/
TEST_UNIFORM_Z (firstp_dec_b64_u64_ptrue_z0, svuint64_t,
		z0 = svsub_n_u64_x (svptrue_b64 (), z0, svfirstp_b64 (svptrue_b64 (), p0)),
		z0 = svsub_x (svptrue_b64 (), z0, svfirstp_b64 (svptrue_b64 (), p0)));

/*
** firstp_dec_b64_u64_ptrue_z1:
**	ptrue	(p[0-7])\.b, all
**	firstp	(x[0-9]+), \1, p0\.d
**	mov	(z[0-9]+\.d), \2
**	sub	z0\.d, z1\.d, \3
**	ret
*/
TEST_UNIFORM_Z (firstp_dec_b64_u64_ptrue_z1, svuint64_t,
		z0 = svsub_n_u64_x (svptrue_b64 (), z1, svfirstp_b64 (svptrue_b64 (), p0)),
		z0 = svsub_x (svptrue_b64 (), z1, svfirstp_b64 (svptrue_b64 (), p0)));
