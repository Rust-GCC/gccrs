/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" } } */

#include "test_sve_acle.h"

/*
** dup_lane_w0_u8_tied1:
**	mov	(z[0-9]+\.b), w0
**	tbl	z0\.b, z0\.b, \1
**	ret
*/
TEST_UNIFORM_ZX (dup_lane_w0_u8_tied1, svuint8_t, uint8_t,
		 z0 = svdup_lane_u8 (z0, x0),
		 z0 = svdup_lane (z0, x0))

/*
** dup_lane_w0_u8_untied:
**	mov	(z[0-9]+\.b), w0
**	tbl	z0\.b, z1\.b, \1
**	ret
*/
TEST_UNIFORM_ZX (dup_lane_w0_u8_untied, svuint8_t, uint8_t,
		 z0 = svdup_lane_u8 (z1, x0),
		 z0 = svdup_lane (z1, x0))

/*
** dup_lane_0_u8_tied1:
**	dup	z0\.b, z0\.b\[0\]
**	ret
*/
TEST_UNIFORM_Z (dup_lane_0_u8_tied1, svuint8_t,
		z0 = svdup_lane_u8 (z0, 0),
		z0 = svdup_lane (z0, 0))

/*
** dup_lane_0_u8_untied:
**	dup	z0\.b, z1\.b\[0\]
**	ret
*/
TEST_UNIFORM_Z (dup_lane_0_u8_untied, svuint8_t,
		z0 = svdup_lane_u8 (z1, 0),
		z0 = svdup_lane (z1, 0))

/*
** dup_lane_7_u8:
**	dup	z0\.b, z0\.b\[7\]
**	ret
*/
TEST_UNIFORM_Z (dup_lane_7_u8, svuint8_t,
		z0 = svdup_lane_u8 (z0, 7),
		z0 = svdup_lane (z0, 7))

/*
** dup_lane_8_u8:
**	dup	z0\.b, z0\.b\[8\]
**	ret
*/
TEST_UNIFORM_Z (dup_lane_8_u8, svuint8_t,
		z0 = svdup_lane_u8 (z0, 8),
		z0 = svdup_lane (z0, 8))

/*
** dup_lane_15_u8:
**	dup	z0\.b, z0\.b\[15\]
**	ret
*/
TEST_UNIFORM_Z (dup_lane_15_u8, svuint8_t,
		z0 = svdup_lane_u8 (z0, 15),
		z0 = svdup_lane (z0, 15))

/*
** dup_lane_16_u8:
**	dup	z0\.b, z0\.b\[16\]
**	ret
*/
TEST_UNIFORM_Z (dup_lane_16_u8, svuint8_t,
		z0 = svdup_lane_u8 (z0, 16),
		z0 = svdup_lane (z0, 16))

/*
** dup_lane_31_u8:
**	dup	z0\.b, z0\.b\[31\]
**	ret
*/
TEST_UNIFORM_Z (dup_lane_31_u8, svuint8_t,
		z0 = svdup_lane_u8 (z0, 31),
		z0 = svdup_lane (z0, 31))

/*
** dup_lane_32_u8:
**	dup	z0\.b, z0\.b\[32\]
**	ret
*/
TEST_UNIFORM_Z (dup_lane_32_u8, svuint8_t,
		z0 = svdup_lane_u8 (z0, 32),
		z0 = svdup_lane (z0, 32))

/*
** dup_lane_63_u8:
**	dup	z0\.b, z0\.b\[63\]
**	ret
*/
TEST_UNIFORM_Z (dup_lane_63_u8, svuint8_t,
		z0 = svdup_lane_u8 (z0, 63),
		z0 = svdup_lane (z0, 63))

/*
** dup_lane_64_u8:
**	mov	(z[0-9]+\.b), #64
**	tbl	z0\.b, z0\.b, \1
**	ret
*/
TEST_UNIFORM_Z (dup_lane_64_u8, svuint8_t,
		z0 = svdup_lane_u8 (z0, 64),
		z0 = svdup_lane (z0, 64))

/*
** dup_lane_255_u8:
**	mov	(z[0-9]+\.b), #-1
**	tbl	z0\.b, z0\.b, \1
**	ret
*/
TEST_UNIFORM_Z (dup_lane_255_u8, svuint8_t,
		z0 = svdup_lane_u8 (z0, 255),
		z0 = svdup_lane (z0, 255))
