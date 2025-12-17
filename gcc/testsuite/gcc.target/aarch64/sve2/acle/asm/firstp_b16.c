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
** firstp_b16_32:
**	firstp	x0, p0, p1\.h
**	ret
*/
TEST_PTEST (firstp_b16_32, uint32_t,
	    x0 = svfirstp_b16 (p0, p1));

/*
** firstp_b16_64:
**	firstp	x0, p0, p1\.h
**	ret
*/
TEST_PTEST (firstp_b16_64, uint64_t,
	    x0 = svfirstp_b16 (p0, p1));

/*
** firstp_inc_b16_32_general_x0:
**	firstp	x([0-9]+), p0, p1\.h
**	add	w0, (w0, w\1|w\1, w0)
**	ret
*/
TEST_PTEST (firstp_inc_b16_32_general_x0, uint32_t,
	    x0 += svfirstp_b16 (p0, p1));

/*
** firstp_inc_b16_32_general_x1:
**	firstp	x([0-9]+), p0, p1\.h
**	add	w0, (w1, w\1|w\1, w1)
**	ret
*/
TEST_PTEST (firstp_inc_b16_32_general_x1, uint32_t,
	    x0 = x1 + svfirstp_b16 (p0, p1));

/*
** firstp_inc_b16_64_general_x0:
**	firstp	(x[0-9]+), p0, p1\.h
**	add	x0, (x0, \1|\1, x0)
**	ret
*/
TEST_PTEST (firstp_inc_b16_64_general_x0, uint64_t,
	    x0 += svfirstp_b16 (p0, p1));

/*
** firstp_inc_b16_64_general_x1:
**	firstp	(x[0-9]+), p0, p1\.h
**	add	x0, (x1, \1|\1, x1)
**	ret
*/
TEST_PTEST (firstp_inc_b16_64_general_x1, uint64_t,
	    x0 = x1 + svfirstp_b16 (p0, p1));

/*
** firstp_dec_b16_32_general_x0:
**	firstp	x([0-9]+), p0, p1\.h
**	sub	w0, w0, w\1
**	ret
*/
TEST_PTEST (firstp_dec_b16_32_general_x0, uint32_t,
	    x0 -= svfirstp_b16 (p0, p1));

/*
** firstp_dec_b16_32_general_x1:
**	firstp	x([0-9]+), p0, p1\.h
**	sub	w0, w1, w\1
**	ret
*/
TEST_PTEST (firstp_dec_b16_32_general_x1, uint32_t,
	    x0 = x1 - svfirstp_b16 (p0, p1));

/*
** firstp_dec_b16_64_general_x0:
**	firstp	(x[0-9]+), p0, p1\.h
**	sub	x0, x0, \1
**	ret
*/
TEST_PTEST (firstp_dec_b16_64_general_x0, uint64_t,
	    x0 -= svfirstp_b16 (p0, p1));

/*
** firstp_dec_b16_64_general_x1:
**	firstp	(x[0-9]+), p0, p1\.h
**	sub	x0, x1, \1
**	ret
*/
TEST_PTEST (firstp_dec_b16_64_general_x1, uint64_t,
	    x0 = x1 - svfirstp_b16 (p0, p1));

/*
** firstp_inc_b16_u16_general_z0:
**	firstp	x([0-9]+), p0, p1\.h
**	mov	(z[0-9]+\.h), w\1
**	add	z0\.h, (z0\.h, \2|\2, z0\.h)
**	ret
*/
TEST_UNIFORM_Z (firstp_inc_b16_u16_general_z0, svuint16_t,
		z0 = svadd_n_u16_x (svptrue_b16 (), z0, svfirstp_b16 (p0, p1)),
		z0 = svadd_x (svptrue_b16 (), z0, svfirstp_b16 (p0, p1)));

/*
** firstp_inc_b16_u16_general_z1:
**	firstp	x([0-9]+), p0, p1\.h
**	mov	(z[0-9]+\.h), w\1
**	add	z0\.h, (z1\.h, \2|\2, z1\.h)
**	ret
*/
TEST_UNIFORM_Z (firstp_inc_b16_u16_general_z1, svuint16_t,
		z0 = svadd_n_u16_x (svptrue_b16 (), z1, svfirstp_b16 (p0, p1)),
		z0 = svadd_x (svptrue_b16 (), z1, svfirstp_b16 (p0, p1)));

/*
** firstp_inc_b16_u16_ptrue_z0:
**	ptrue	(p[0-7])\.b, all
**	firstp	x([0-9]+), \1, p0\.h
**	mov	(z[0-9]+\.h), w\2
**	add	z0\.h, (z0\.h, \3|\3, z0\.h)
**	ret
*/
TEST_UNIFORM_Z (firstp_inc_b16_u16_ptrue_z0, svuint16_t,
		z0 = svadd_n_u16_x (svptrue_b16 (), z0, svfirstp_b16 (svptrue_b16 (), p0)),
		z0 = svadd_x (svptrue_b16 (), z0, svfirstp_b16 (svptrue_b16 (), p0)));

/*
** firstp_inc_b16_u16_ptrue_z1:
**	ptrue	(p[0-7])\.b, all
**	firstp	x([0-9]+), \1, p0\.h
**	mov	(z[0-9]+\.h), w\2
**	add	z0\.h, (z1\.h, \3|\3, z1\.h)
**	ret
*/
TEST_UNIFORM_Z (firstp_inc_b16_u16_ptrue_z1, svuint16_t,
		z0 = svadd_n_u16_x (svptrue_b16 (), z1, svfirstp_b16 (svptrue_b16 (), p0)),
		z0 = svadd_x (svptrue_b16 (), z1, svfirstp_b16 (svptrue_b16 (), p0)));

/*
** firstp_dec_b16_u16_general_z0:
**	firstp	x([0-9]+), p0, p1\.h
**	mov	(z[0-9]+\.h), w\1
**	sub	z0\.h, z0\.h, \2
**	ret
*/
TEST_UNIFORM_Z (firstp_dec_b16_u16_general_z0, svuint16_t,
		z0 = svsub_n_u16_x (svptrue_b16 (), z0, svfirstp_b16 (p0, p1)),
		z0 = svsub_x (svptrue_b16 (), z0, svfirstp_b16 (p0, p1)));

/*
** firstp_dec_b16_u16_general_z1:
**	firstp	x([0-9]+), p0, p1\.h
**	mov	(z[0-9]+\.h), w\1
**	sub	z0\.h, z1\.h, \2
**	ret
*/
TEST_UNIFORM_Z (firstp_dec_b16_u16_general_z1, svuint16_t,
		z0 = svsub_n_u16_x (svptrue_b16 (), z1, svfirstp_b16 (p0, p1)),
		z0 = svsub_x (svptrue_b16 (), z1, svfirstp_b16 (p0, p1)));

/*
** firstp_dec_b16_u16_ptrue_z0:
**	ptrue	(p[0-7])\.b, all
**	firstp	x([0-9]+), \1, p0\.h
**	mov	(z[0-9]+\.h), w\2
**	sub	z0\.h, z0\.h, \3
**	ret
*/
TEST_UNIFORM_Z (firstp_dec_b16_u16_ptrue_z0, svuint16_t,
		z0 = svsub_n_u16_x (svptrue_b16 (), z0, svfirstp_b16 (svptrue_b16 (), p0)),
		z0 = svsub_x (svptrue_b16 (), z0, svfirstp_b16 (svptrue_b16 (), p0)));

/*
** firstp_dec_b16_u16_ptrue_z1:
**	ptrue	(p[0-7])\.b, all
**	firstp	x([0-9]+), \1, p0\.h
**	mov	(z[0-9]+\.h), w\2
**	sub	z0\.h, z1\.h, \3
**	ret
*/
TEST_UNIFORM_Z (firstp_dec_b16_u16_ptrue_z1, svuint16_t,
		z0 = svsub_n_u16_x (svptrue_b16 (), z1, svfirstp_b16 (svptrue_b16 (), p0)),
		z0 = svsub_x (svptrue_b16 (), z1, svfirstp_b16 (svptrue_b16 (), p0)));
