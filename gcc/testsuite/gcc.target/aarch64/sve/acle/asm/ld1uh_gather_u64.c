/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" } } */

#include "test_sve_acle.h"

/*
** ld1uh_gather_u64_tied1:
**	ld1h	z0\.d, p0/z, \[z0\.d\]
**	ret
*/
TEST_LOAD_GATHER_ZS (ld1uh_gather_u64_tied1, svuint64_t, svuint64_t,
		     z0_res = svld1uh_gather_u64base_u64 (p0, z0),
		     z0_res = svld1uh_gather_u64 (p0, z0))

/*
** ld1uh_gather_u64_untied:
**	ld1h	z0\.d, p0/z, \[z1\.d\]
**	ret
*/
TEST_LOAD_GATHER_ZS (ld1uh_gather_u64_untied, svuint64_t, svuint64_t,
		     z0_res = svld1uh_gather_u64base_u64 (p0, z1),
		     z0_res = svld1uh_gather_u64 (p0, z1))

/*
** ld1uh_gather_x0_u64_offset:
**	ld1h	z0\.d, p0/z, \[x0, z0\.d\]
**	ret
*/
TEST_LOAD_GATHER_ZS (ld1uh_gather_x0_u64_offset, svuint64_t, svuint64_t,
		     z0_res = svld1uh_gather_u64base_offset_u64 (p0, z0, x0),
		     z0_res = svld1uh_gather_offset_u64 (p0, z0, x0))

/*
** ld1uh_gather_m2_u64_offset:
**	mov	(x[0-9]+), #?-2
**	ld1h	z0\.d, p0/z, \[\1, z0\.d\]
**	ret
*/
TEST_LOAD_GATHER_ZS (ld1uh_gather_m2_u64_offset, svuint64_t, svuint64_t,
		     z0_res = svld1uh_gather_u64base_offset_u64 (p0, z0, -2),
		     z0_res = svld1uh_gather_offset_u64 (p0, z0, -2))

/*
** ld1uh_gather_0_u64_offset:
**	ld1h	z0\.d, p0/z, \[z0\.d\]
**	ret
*/
TEST_LOAD_GATHER_ZS (ld1uh_gather_0_u64_offset, svuint64_t, svuint64_t,
		     z0_res = svld1uh_gather_u64base_offset_u64 (p0, z0, 0),
		     z0_res = svld1uh_gather_offset_u64 (p0, z0, 0))

/*
** ld1uh_gather_5_u64_offset:
**	mov	(x[0-9]+), #?5
**	ld1h	z0\.d, p0/z, \[\1, z0\.d\]
**	ret
*/
TEST_LOAD_GATHER_ZS (ld1uh_gather_5_u64_offset, svuint64_t, svuint64_t,
		     z0_res = svld1uh_gather_u64base_offset_u64 (p0, z0, 5),
		     z0_res = svld1uh_gather_offset_u64 (p0, z0, 5))

/*
** ld1uh_gather_6_u64_offset:
**	ld1h	z0\.d, p0/z, \[z0\.d, #6\]
**	ret
*/
TEST_LOAD_GATHER_ZS (ld1uh_gather_6_u64_offset, svuint64_t, svuint64_t,
		     z0_res = svld1uh_gather_u64base_offset_u64 (p0, z0, 6),
		     z0_res = svld1uh_gather_offset_u64 (p0, z0, 6))

/*
** ld1uh_gather_62_u64_offset:
**	ld1h	z0\.d, p0/z, \[z0\.d, #62\]
**	ret
*/
TEST_LOAD_GATHER_ZS (ld1uh_gather_62_u64_offset, svuint64_t, svuint64_t,
		     z0_res = svld1uh_gather_u64base_offset_u64 (p0, z0, 62),
		     z0_res = svld1uh_gather_offset_u64 (p0, z0, 62))

/*
** ld1uh_gather_64_u64_offset:
**	mov	(x[0-9]+), #?64
**	ld1h	z0\.d, p0/z, \[\1, z0\.d\]
**	ret
*/
TEST_LOAD_GATHER_ZS (ld1uh_gather_64_u64_offset, svuint64_t, svuint64_t,
		     z0_res = svld1uh_gather_u64base_offset_u64 (p0, z0, 64),
		     z0_res = svld1uh_gather_offset_u64 (p0, z0, 64))

/*
** ld1uh_gather_x0_u64_index:
**	lsl	(x[0-9]+), x0, #?1
**	ld1h	z0\.d, p0/z, \[\1, z0\.d\]
**	ret
*/
TEST_LOAD_GATHER_ZS (ld1uh_gather_x0_u64_index, svuint64_t, svuint64_t,
		     z0_res = svld1uh_gather_u64base_index_u64 (p0, z0, x0),
		     z0_res = svld1uh_gather_index_u64 (p0, z0, x0))

/*
** ld1uh_gather_m1_u64_index:
**	mov	(x[0-9]+), #?-2
**	ld1h	z0\.d, p0/z, \[\1, z0\.d\]
**	ret
*/
TEST_LOAD_GATHER_ZS (ld1uh_gather_m1_u64_index, svuint64_t, svuint64_t,
		     z0_res = svld1uh_gather_u64base_index_u64 (p0, z0, -1),
		     z0_res = svld1uh_gather_index_u64 (p0, z0, -1))

/*
** ld1uh_gather_0_u64_index:
**	ld1h	z0\.d, p0/z, \[z0\.d\]
**	ret
*/
TEST_LOAD_GATHER_ZS (ld1uh_gather_0_u64_index, svuint64_t, svuint64_t,
		     z0_res = svld1uh_gather_u64base_index_u64 (p0, z0, 0),
		     z0_res = svld1uh_gather_index_u64 (p0, z0, 0))

/*
** ld1uh_gather_5_u64_index:
**	ld1h	z0\.d, p0/z, \[z0\.d, #10\]
**	ret
*/
TEST_LOAD_GATHER_ZS (ld1uh_gather_5_u64_index, svuint64_t, svuint64_t,
		     z0_res = svld1uh_gather_u64base_index_u64 (p0, z0, 5),
		     z0_res = svld1uh_gather_index_u64 (p0, z0, 5))

/*
** ld1uh_gather_31_u64_index:
**	ld1h	z0\.d, p0/z, \[z0\.d, #62\]
**	ret
*/
TEST_LOAD_GATHER_ZS (ld1uh_gather_31_u64_index, svuint64_t, svuint64_t,
		     z0_res = svld1uh_gather_u64base_index_u64 (p0, z0, 31),
		     z0_res = svld1uh_gather_index_u64 (p0, z0, 31))

/*
** ld1uh_gather_32_u64_index:
**	mov	(x[0-9]+), #?64
**	ld1h	z0\.d, p0/z, \[\1, z0\.d\]
**	ret
*/
TEST_LOAD_GATHER_ZS (ld1uh_gather_32_u64_index, svuint64_t, svuint64_t,
		     z0_res = svld1uh_gather_u64base_index_u64 (p0, z0, 32),
		     z0_res = svld1uh_gather_index_u64 (p0, z0, 32))

/*
** ld1uh_gather_x0_u64_s64offset:
**	ld1h	z0\.d, p0/z, \[x0, z0\.d\]
**	ret
*/
TEST_LOAD_GATHER_SZ (ld1uh_gather_x0_u64_s64offset, svuint64_t, uint16_t, svint64_t,
		     z0_res = svld1uh_gather_s64offset_u64 (p0, x0, z0),
		     z0_res = svld1uh_gather_offset_u64 (p0, x0, z0))

/*
** ld1uh_gather_tied1_u64_s64offset:
**	ld1h	z0\.d, p0/z, \[x0, z0\.d\]
**	ret
*/
TEST_LOAD_GATHER_SZ (ld1uh_gather_tied1_u64_s64offset, svuint64_t, uint16_t, svint64_t,
		     z0_res = svld1uh_gather_s64offset_u64 (p0, x0, z0),
		     z0_res = svld1uh_gather_offset_u64 (p0, x0, z0))

/*
** ld1uh_gather_untied_u64_s64offset:
**	ld1h	z0\.d, p0/z, \[x0, z1\.d\]
**	ret
*/
TEST_LOAD_GATHER_SZ (ld1uh_gather_untied_u64_s64offset, svuint64_t, uint16_t, svint64_t,
		     z0_res = svld1uh_gather_s64offset_u64 (p0, x0, z1),
		     z0_res = svld1uh_gather_offset_u64 (p0, x0, z1))

/*
** ld1uh_gather_ext_u64_s64offset:
**	ld1h	z0\.d, p0/z, \[x0, z1\.d, sxtw\]
**	ret
*/
TEST_LOAD_GATHER_SZ (ld1uh_gather_ext_u64_s64offset, svuint64_t, uint16_t, svint64_t,
		     z0_res = svld1uh_gather_s64offset_u64 (p0, x0, svextw_s64_x (p0, z1)),
		     z0_res = svld1uh_gather_offset_u64 (p0, x0, svextw_x (p0, z1)))

/*
** ld1uh_gather_x0_u64_u64offset:
**	ld1h	z0\.d, p0/z, \[x0, z0\.d\]
**	ret
*/
TEST_LOAD_GATHER_SZ (ld1uh_gather_x0_u64_u64offset, svuint64_t, uint16_t, svuint64_t,
		     z0_res = svld1uh_gather_u64offset_u64 (p0, x0, z0),
		     z0_res = svld1uh_gather_offset_u64 (p0, x0, z0))

/*
** ld1uh_gather_tied1_u64_u64offset:
**	ld1h	z0\.d, p0/z, \[x0, z0\.d\]
**	ret
*/
TEST_LOAD_GATHER_SZ (ld1uh_gather_tied1_u64_u64offset, svuint64_t, uint16_t, svuint64_t,
		     z0_res = svld1uh_gather_u64offset_u64 (p0, x0, z0),
		     z0_res = svld1uh_gather_offset_u64 (p0, x0, z0))

/*
** ld1uh_gather_untied_u64_u64offset:
**	ld1h	z0\.d, p0/z, \[x0, z1\.d\]
**	ret
*/
TEST_LOAD_GATHER_SZ (ld1uh_gather_untied_u64_u64offset, svuint64_t, uint16_t, svuint64_t,
		     z0_res = svld1uh_gather_u64offset_u64 (p0, x0, z1),
		     z0_res = svld1uh_gather_offset_u64 (p0, x0, z1))

/*
** ld1uh_gather_ext_u64_u64offset:
**	ld1h	z0\.d, p0/z, \[x0, z1\.d, uxtw\]
**	ret
*/
TEST_LOAD_GATHER_SZ (ld1uh_gather_ext_u64_u64offset, svuint64_t, uint16_t, svuint64_t,
		     z0_res = svld1uh_gather_u64offset_u64 (p0, x0, svextw_u64_x (p0, z1)),
		     z0_res = svld1uh_gather_offset_u64 (p0, x0, svextw_x (p0, z1)))

/*
** ld1uh_gather_x0_u64_s64index:
**	ld1h	z0\.d, p0/z, \[x0, z0\.d, lsl 1\]
**	ret
*/
TEST_LOAD_GATHER_SZ (ld1uh_gather_x0_u64_s64index, svuint64_t, uint16_t, svint64_t,
		     z0_res = svld1uh_gather_s64index_u64 (p0, x0, z0),
		     z0_res = svld1uh_gather_index_u64 (p0, x0, z0))

/*
** ld1uh_gather_tied1_u64_s64index:
**	ld1h	z0\.d, p0/z, \[x0, z0\.d, lsl 1\]
**	ret
*/
TEST_LOAD_GATHER_SZ (ld1uh_gather_tied1_u64_s64index, svuint64_t, uint16_t, svint64_t,
		     z0_res = svld1uh_gather_s64index_u64 (p0, x0, z0),
		     z0_res = svld1uh_gather_index_u64 (p0, x0, z0))

/*
** ld1uh_gather_untied_u64_s64index:
**	ld1h	z0\.d, p0/z, \[x0, z1\.d, lsl 1\]
**	ret
*/
TEST_LOAD_GATHER_SZ (ld1uh_gather_untied_u64_s64index, svuint64_t, uint16_t, svint64_t,
		     z0_res = svld1uh_gather_s64index_u64 (p0, x0, z1),
		     z0_res = svld1uh_gather_index_u64 (p0, x0, z1))

/*
** ld1uh_gather_ext_u64_s64index:
**	ld1h	z0\.d, p0/z, \[x0, z1\.d, sxtw 1\]
**	ret
*/
TEST_LOAD_GATHER_SZ (ld1uh_gather_ext_u64_s64index, svuint64_t, uint16_t, svint64_t,
		     z0_res = svld1uh_gather_s64index_u64 (p0, x0, svextw_s64_x (p0, z1)),
		     z0_res = svld1uh_gather_index_u64 (p0, x0, svextw_x (p0, z1)))

/*
** ld1uh_gather_x0_u64_u64index:
**	ld1h	z0\.d, p0/z, \[x0, z0\.d, lsl 1\]
**	ret
*/
TEST_LOAD_GATHER_SZ (ld1uh_gather_x0_u64_u64index, svuint64_t, uint16_t, svuint64_t,
		     z0_res = svld1uh_gather_u64index_u64 (p0, x0, z0),
		     z0_res = svld1uh_gather_index_u64 (p0, x0, z0))

/*
** ld1uh_gather_tied1_u64_u64index:
**	ld1h	z0\.d, p0/z, \[x0, z0\.d, lsl 1\]
**	ret
*/
TEST_LOAD_GATHER_SZ (ld1uh_gather_tied1_u64_u64index, svuint64_t, uint16_t, svuint64_t,
		     z0_res = svld1uh_gather_u64index_u64 (p0, x0, z0),
		     z0_res = svld1uh_gather_index_u64 (p0, x0, z0))

/*
** ld1uh_gather_untied_u64_u64index:
**	ld1h	z0\.d, p0/z, \[x0, z1\.d, lsl 1\]
**	ret
*/
TEST_LOAD_GATHER_SZ (ld1uh_gather_untied_u64_u64index, svuint64_t, uint16_t, svuint64_t,
		     z0_res = svld1uh_gather_u64index_u64 (p0, x0, z1),
		     z0_res = svld1uh_gather_index_u64 (p0, x0, z1))

/*
** ld1uh_gather_ext_u64_u64index:
**	ld1h	z0\.d, p0/z, \[x0, z1\.d, uxtw 1\]
**	ret
*/
TEST_LOAD_GATHER_SZ (ld1uh_gather_ext_u64_u64index, svuint64_t, uint16_t, svuint64_t,
		     z0_res = svld1uh_gather_u64index_u64 (p0, x0, svextw_u64_x (p0, z1)),
		     z0_res = svld1uh_gather_index_u64 (p0, x0, svextw_x (p0, z1)))
