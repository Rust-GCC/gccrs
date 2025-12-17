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
** lastp_b8_32:
**	lastp	x0, p0, p1\.b
**	ret
*/
TEST_PTEST (lastp_b8_32, uint32_t,
	    x0 = svlastp_b8 (p0, p1));

/*
** lastp_b8_64:
**	lastp	x0, p0, p1\.b
**	ret
*/
TEST_PTEST (lastp_b8_64, uint64_t,
	    x0 = svlastp_b8 (p0, p1));

/*
** lastp_inc_b8_32_general_x0:
**	lastp	x([0-9]+), p0, p1\.b
**	add	w0, (w0, w\1|w\1, w0)
**	ret
*/
TEST_PTEST (lastp_inc_b8_32_general_x0, uint32_t,
	    x0 += svlastp_b8 (p0, p1));

/*
** lastp_inc_b8_32_general_x1:
**	lastp	x([0-9]+), p0, p1\.b
**	add	w0, (w1, w\1|w\1, w1)
**	ret
*/
TEST_PTEST (lastp_inc_b8_32_general_x1, uint32_t,
	    x0 = x1 + svlastp_b8 (p0, p1));

/*
** lastp_inc_b8_64_general_x0:
**	lastp	(x[0-9]+), p0, p1\.b
**	add	x0, (x0, \1|\1, x0)
**	ret
*/
TEST_PTEST (lastp_inc_b8_64_general_x0, uint64_t,
	    x0 += svlastp_b8 (p0, p1));

/*
** lastp_inc_b8_64_general_x1:
**	lastp	(x[0-9]+), p0, p1\.b
**	add	x0, (x1, \1|\1, x1)
**	ret
*/
TEST_PTEST (lastp_inc_b8_64_general_x1, uint64_t,
	    x0 = x1 + svlastp_b8 (p0, p1));

/*
** lastp_dec_b8_32_general_x0:
**	lastp	x([0-9]+), p0, p1\.b
**	sub	w0, w0, w\1
**	ret
*/
TEST_PTEST (lastp_dec_b8_32_general_x0, uint32_t,
	    x0 -= svlastp_b8 (p0, p1));

/*
** lastp_dec_b8_32_general_x1:
**	lastp	x([0-9]+), p0, p1\.b
**	sub	w0, w1, w\1
**	ret
*/
TEST_PTEST (lastp_dec_b8_32_general_x1, uint32_t,
	    x0 = x1 - svlastp_b8 (p0, p1));

/*
** lastp_dec_b8_64_general_x0:
**	lastp	(x[0-9]+), p0, p1\.b
**	sub	x0, x0, \1
**	ret
*/
TEST_PTEST (lastp_dec_b8_64_general_x0, uint64_t,
	    x0 -= svlastp_b8 (p0, p1));

/*
** lastp_dec_b8_64_general_x1:
**	lastp	(x[0-9]+), p0, p1\.b
**	sub	x0, x1, \1
**	ret
*/
TEST_PTEST (lastp_dec_b8_64_general_x1, uint64_t,
	    x0 = x1 - svlastp_b8 (p0, p1));

/*
** lastp_inc_b8_u8_general_z0:
**	lastp	x([0-9]+), p0, p1\.b
**	mov	(z[0-9]+\.b), w\1
**	add	z0\.b, (z0\.b, \2|\2, z0\.b)
**	ret
*/
TEST_UNIFORM_Z (lastp_inc_b8_u8_general_z0, svuint8_t,
		z0 = svadd_n_u8_x (svptrue_b8 (), z0, svlastp_b8 (p0, p1)),
		z0 = svadd_x (svptrue_b8 (), z0, svlastp_b8 (p0, p1)));

/*
** lastp_inc_b8_u8_general_z1:
**	lastp	x([0-9]+), p0, p1\.b
**	mov	(z[0-9]+\.b), w\1
**	add	z0\.b, (z1\.b, \2|\2, z1\.b)
**	ret
*/
TEST_UNIFORM_Z (lastp_inc_b8_u8_general_z1, svuint8_t,
		z0 = svadd_n_u8_x (svptrue_b8 (), z1, svlastp_b8 (p0, p1)),
		z0 = svadd_x (svptrue_b8 (), z1, svlastp_b8 (p0, p1)));

/*
** lastp_inc_b8_u8_ptrue_z0:
**	ptrue	(p[0-7])\.b, all
**	lastp	x([0-9]+), \1, p0\.b
**	mov	(z[0-9]+\.b), w\2
**	add	z0\.b, (z0\.b, \3|\3, z0\.b)
**	ret
*/
TEST_UNIFORM_Z (lastp_inc_b8_u8_ptrue_z0, svuint8_t,
		z0 = svadd_n_u8_x (svptrue_b8 (), z0, svlastp_b8 (svptrue_b8 (), p0)),
		z0 = svadd_x (svptrue_b8 (), z0, svlastp_b8 (svptrue_b8 (), p0)));

/*
** lastp_inc_b8_u8_ptrue_z1:
**	ptrue	(p[0-7])\.b, all
**	lastp	x([0-9]+), \1, p0\.b
**	mov	(z[0-9]+\.b), w\2
**	add	z0\.b, (z1\.b, \3|\3, z1\.b)
**	ret
*/
TEST_UNIFORM_Z (lastp_inc_b8_u8_ptrue_z1, svuint8_t,
		z0 = svadd_n_u8_x (svptrue_b8 (), z1, svlastp_b8 (svptrue_b8 (), p0)),
		z0 = svadd_x (svptrue_b8 (), z1, svlastp_b8 (svptrue_b8 (), p0)));

/*
** lastp_dec_b8_u8_general_z0:
**	lastp	x([0-9]+), p0, p1\.b
**	mov	(z[0-9]+\.b), w\1
**	sub	z0\.b, z0\.b, \2
**	ret
*/
TEST_UNIFORM_Z (lastp_dec_b8_u8_general_z0, svuint8_t,
		z0 = svsub_n_u8_x (svptrue_b8 (), z0, svlastp_b8 (p0, p1)),
		z0 = svsub_x (svptrue_b8 (), z0, svlastp_b8 (p0, p1)));

/*
** lastp_dec_b8_u8_general_z1:
**	lastp	x([0-9]+), p0, p1\.b
**	mov	(z[0-9]+\.b), w\1
**	sub	z0\.b, z1\.b, \2
**	ret
*/
TEST_UNIFORM_Z (lastp_dec_b8_u8_general_z1, svuint8_t,
		z0 = svsub_n_u8_x (svptrue_b8 (), z1, svlastp_b8 (p0, p1)),
		z0 = svsub_x (svptrue_b8 (), z1, svlastp_b8 (p0, p1)));

/*
** lastp_dec_b8_u8_ptrue_z0:
**	ptrue	(p[0-7])\.b, all
**	lastp	x([0-9]+), \1, p0\.b
**	mov	(z[0-9]+\.b), w\2
**	sub	z0\.b, z0\.b, \3
**	ret
*/
TEST_UNIFORM_Z (lastp_dec_b8_u8_ptrue_z0, svuint8_t,
		z0 = svsub_n_u8_x (svptrue_b8 (), z0, svlastp_b8 (svptrue_b8 (), p0)),
		z0 = svsub_x (svptrue_b8 (), z0, svlastp_b8 (svptrue_b8 (), p0)));

/*
** lastp_dec_b8_u8_ptrue_z1:
**	ptrue	(p[0-7])\.b, all
**	lastp	x([0-9]+), \1, p0\.b
**	mov	(z[0-9]+\.b), w\2
**	sub	z0\.b, z1\.b, \3
**	ret
*/
TEST_UNIFORM_Z (lastp_dec_b8_u8_ptrue_z1, svuint8_t,
		z0 = svsub_n_u8_x (svptrue_b8 (), z1, svlastp_b8 (svptrue_b8 (), p0)),
		z0 = svsub_x (svptrue_b8 (), z1, svlastp_b8 (svptrue_b8 (), p0)));
