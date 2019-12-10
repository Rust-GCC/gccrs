/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" } } */

#include "test_sve_acle.h"

/*
** ld1sh_gather_s32_tied1:
**	ld1sh	z0\.s, p0/z, \[z0\.s\]
**	ret
*/
TEST_LOAD_GATHER_ZS (ld1sh_gather_s32_tied1, svint32_t, svuint32_t,
		     z0_res = svld1sh_gather_u32base_s32 (p0, z0),
		     z0_res = svld1sh_gather_s32 (p0, z0))

/*
** ld1sh_gather_s32_untied:
**	ld1sh	z0\.s, p0/z, \[z1\.s\]
**	ret
*/
TEST_LOAD_GATHER_ZS (ld1sh_gather_s32_untied, svint32_t, svuint32_t,
		     z0_res = svld1sh_gather_u32base_s32 (p0, z1),
		     z0_res = svld1sh_gather_s32 (p0, z1))

/*
** ld1sh_gather_x0_s32_offset:
**	ld1sh	z0\.s, p0/z, \[x0, z0\.s, uxtw\]
**	ret
*/
TEST_LOAD_GATHER_ZS (ld1sh_gather_x0_s32_offset, svint32_t, svuint32_t,
		     z0_res = svld1sh_gather_u32base_offset_s32 (p0, z0, x0),
		     z0_res = svld1sh_gather_offset_s32 (p0, z0, x0))

/*
** ld1sh_gather_m2_s32_offset:
**	mov	(x[0-9]+), #?-2
**	ld1sh	z0\.s, p0/z, \[\1, z0\.s, uxtw\]
**	ret
*/
TEST_LOAD_GATHER_ZS (ld1sh_gather_m2_s32_offset, svint32_t, svuint32_t,
		     z0_res = svld1sh_gather_u32base_offset_s32 (p0, z0, -2),
		     z0_res = svld1sh_gather_offset_s32 (p0, z0, -2))

/*
** ld1sh_gather_0_s32_offset:
**	ld1sh	z0\.s, p0/z, \[z0\.s\]
**	ret
*/
TEST_LOAD_GATHER_ZS (ld1sh_gather_0_s32_offset, svint32_t, svuint32_t,
		     z0_res = svld1sh_gather_u32base_offset_s32 (p0, z0, 0),
		     z0_res = svld1sh_gather_offset_s32 (p0, z0, 0))

/*
** ld1sh_gather_5_s32_offset:
**	mov	(x[0-9]+), #?5
**	ld1sh	z0\.s, p0/z, \[\1, z0\.s, uxtw\]
**	ret
*/
TEST_LOAD_GATHER_ZS (ld1sh_gather_5_s32_offset, svint32_t, svuint32_t,
		     z0_res = svld1sh_gather_u32base_offset_s32 (p0, z0, 5),
		     z0_res = svld1sh_gather_offset_s32 (p0, z0, 5))

/*
** ld1sh_gather_6_s32_offset:
**	ld1sh	z0\.s, p0/z, \[z0\.s, #6\]
**	ret
*/
TEST_LOAD_GATHER_ZS (ld1sh_gather_6_s32_offset, svint32_t, svuint32_t,
		     z0_res = svld1sh_gather_u32base_offset_s32 (p0, z0, 6),
		     z0_res = svld1sh_gather_offset_s32 (p0, z0, 6))

/*
** ld1sh_gather_62_s32_offset:
**	ld1sh	z0\.s, p0/z, \[z0\.s, #62\]
**	ret
*/
TEST_LOAD_GATHER_ZS (ld1sh_gather_62_s32_offset, svint32_t, svuint32_t,
		     z0_res = svld1sh_gather_u32base_offset_s32 (p0, z0, 62),
		     z0_res = svld1sh_gather_offset_s32 (p0, z0, 62))

/*
** ld1sh_gather_64_s32_offset:
**	mov	(x[0-9]+), #?64
**	ld1sh	z0\.s, p0/z, \[\1, z0\.s, uxtw\]
**	ret
*/
TEST_LOAD_GATHER_ZS (ld1sh_gather_64_s32_offset, svint32_t, svuint32_t,
		     z0_res = svld1sh_gather_u32base_offset_s32 (p0, z0, 64),
		     z0_res = svld1sh_gather_offset_s32 (p0, z0, 64))

/*
** ld1sh_gather_x0_s32_index:
**	lsl	(x[0-9]+), x0, #?1
**	ld1sh	z0\.s, p0/z, \[\1, z0\.s, uxtw\]
**	ret
*/
TEST_LOAD_GATHER_ZS (ld1sh_gather_x0_s32_index, svint32_t, svuint32_t,
		     z0_res = svld1sh_gather_u32base_index_s32 (p0, z0, x0),
		     z0_res = svld1sh_gather_index_s32 (p0, z0, x0))

/*
** ld1sh_gather_m1_s32_index:
**	mov	(x[0-9]+), #?-2
**	ld1sh	z0\.s, p0/z, \[\1, z0\.s, uxtw\]
**	ret
*/
TEST_LOAD_GATHER_ZS (ld1sh_gather_m1_s32_index, svint32_t, svuint32_t,
		     z0_res = svld1sh_gather_u32base_index_s32 (p0, z0, -1),
		     z0_res = svld1sh_gather_index_s32 (p0, z0, -1))

/*
** ld1sh_gather_0_s32_index:
**	ld1sh	z0\.s, p0/z, \[z0\.s\]
**	ret
*/
TEST_LOAD_GATHER_ZS (ld1sh_gather_0_s32_index, svint32_t, svuint32_t,
		     z0_res = svld1sh_gather_u32base_index_s32 (p0, z0, 0),
		     z0_res = svld1sh_gather_index_s32 (p0, z0, 0))

/*
** ld1sh_gather_5_s32_index:
**	ld1sh	z0\.s, p0/z, \[z0\.s, #10\]
**	ret
*/
TEST_LOAD_GATHER_ZS (ld1sh_gather_5_s32_index, svint32_t, svuint32_t,
		     z0_res = svld1sh_gather_u32base_index_s32 (p0, z0, 5),
		     z0_res = svld1sh_gather_index_s32 (p0, z0, 5))

/*
** ld1sh_gather_31_s32_index:
**	ld1sh	z0\.s, p0/z, \[z0\.s, #62\]
**	ret
*/
TEST_LOAD_GATHER_ZS (ld1sh_gather_31_s32_index, svint32_t, svuint32_t,
		     z0_res = svld1sh_gather_u32base_index_s32 (p0, z0, 31),
		     z0_res = svld1sh_gather_index_s32 (p0, z0, 31))

/*
** ld1sh_gather_32_s32_index:
**	mov	(x[0-9]+), #?64
**	ld1sh	z0\.s, p0/z, \[\1, z0\.s, uxtw\]
**	ret
*/
TEST_LOAD_GATHER_ZS (ld1sh_gather_32_s32_index, svint32_t, svuint32_t,
		     z0_res = svld1sh_gather_u32base_index_s32 (p0, z0, 32),
		     z0_res = svld1sh_gather_index_s32 (p0, z0, 32))

/*
** ld1sh_gather_x0_s32_s32offset:
**	ld1sh	z0\.s, p0/z, \[x0, z0\.s, sxtw\]
**	ret
*/
TEST_LOAD_GATHER_SZ (ld1sh_gather_x0_s32_s32offset, svint32_t, int16_t, svint32_t,
		     z0_res = svld1sh_gather_s32offset_s32 (p0, x0, z0),
		     z0_res = svld1sh_gather_offset_s32 (p0, x0, z0))

/*
** ld1sh_gather_tied1_s32_s32offset:
**	ld1sh	z0\.s, p0/z, \[x0, z0\.s, sxtw\]
**	ret
*/
TEST_LOAD_GATHER_SZ (ld1sh_gather_tied1_s32_s32offset, svint32_t, int16_t, svint32_t,
		     z0_res = svld1sh_gather_s32offset_s32 (p0, x0, z0),
		     z0_res = svld1sh_gather_offset_s32 (p0, x0, z0))

/*
** ld1sh_gather_untied_s32_s32offset:
**	ld1sh	z0\.s, p0/z, \[x0, z1\.s, sxtw\]
**	ret
*/
TEST_LOAD_GATHER_SZ (ld1sh_gather_untied_s32_s32offset, svint32_t, int16_t, svint32_t,
		     z0_res = svld1sh_gather_s32offset_s32 (p0, x0, z1),
		     z0_res = svld1sh_gather_offset_s32 (p0, x0, z1))

/*
** ld1sh_gather_x0_s32_u32offset:
**	ld1sh	z0\.s, p0/z, \[x0, z0\.s, uxtw\]
**	ret
*/
TEST_LOAD_GATHER_SZ (ld1sh_gather_x0_s32_u32offset, svint32_t, int16_t, svuint32_t,
		     z0_res = svld1sh_gather_u32offset_s32 (p0, x0, z0),
		     z0_res = svld1sh_gather_offset_s32 (p0, x0, z0))

/*
** ld1sh_gather_tied1_s32_u32offset:
**	ld1sh	z0\.s, p0/z, \[x0, z0\.s, uxtw\]
**	ret
*/
TEST_LOAD_GATHER_SZ (ld1sh_gather_tied1_s32_u32offset, svint32_t, int16_t, svuint32_t,
		     z0_res = svld1sh_gather_u32offset_s32 (p0, x0, z0),
		     z0_res = svld1sh_gather_offset_s32 (p0, x0, z0))

/*
** ld1sh_gather_untied_s32_u32offset:
**	ld1sh	z0\.s, p0/z, \[x0, z1\.s, uxtw\]
**	ret
*/
TEST_LOAD_GATHER_SZ (ld1sh_gather_untied_s32_u32offset, svint32_t, int16_t, svuint32_t,
		     z0_res = svld1sh_gather_u32offset_s32 (p0, x0, z1),
		     z0_res = svld1sh_gather_offset_s32 (p0, x0, z1))

/*
** ld1sh_gather_x0_s32_s32index:
**	ld1sh	z0\.s, p0/z, \[x0, z0\.s, sxtw 1\]
**	ret
*/
TEST_LOAD_GATHER_SZ (ld1sh_gather_x0_s32_s32index, svint32_t, int16_t, svint32_t,
		     z0_res = svld1sh_gather_s32index_s32 (p0, x0, z0),
		     z0_res = svld1sh_gather_index_s32 (p0, x0, z0))

/*
** ld1sh_gather_tied1_s32_s32index:
**	ld1sh	z0\.s, p0/z, \[x0, z0\.s, sxtw 1\]
**	ret
*/
TEST_LOAD_GATHER_SZ (ld1sh_gather_tied1_s32_s32index, svint32_t, int16_t, svint32_t,
		     z0_res = svld1sh_gather_s32index_s32 (p0, x0, z0),
		     z0_res = svld1sh_gather_index_s32 (p0, x0, z0))

/*
** ld1sh_gather_untied_s32_s32index:
**	ld1sh	z0\.s, p0/z, \[x0, z1\.s, sxtw 1\]
**	ret
*/
TEST_LOAD_GATHER_SZ (ld1sh_gather_untied_s32_s32index, svint32_t, int16_t, svint32_t,
		     z0_res = svld1sh_gather_s32index_s32 (p0, x0, z1),
		     z0_res = svld1sh_gather_index_s32 (p0, x0, z1))

/*
** ld1sh_gather_x0_s32_u32index:
**	ld1sh	z0\.s, p0/z, \[x0, z0\.s, uxtw 1\]
**	ret
*/
TEST_LOAD_GATHER_SZ (ld1sh_gather_x0_s32_u32index, svint32_t, int16_t, svuint32_t,
		     z0_res = svld1sh_gather_u32index_s32 (p0, x0, z0),
		     z0_res = svld1sh_gather_index_s32 (p0, x0, z0))

/*
** ld1sh_gather_tied1_s32_u32index:
**	ld1sh	z0\.s, p0/z, \[x0, z0\.s, uxtw 1\]
**	ret
*/
TEST_LOAD_GATHER_SZ (ld1sh_gather_tied1_s32_u32index, svint32_t, int16_t, svuint32_t,
		     z0_res = svld1sh_gather_u32index_s32 (p0, x0, z0),
		     z0_res = svld1sh_gather_index_s32 (p0, x0, z0))

/*
** ld1sh_gather_untied_s32_u32index:
**	ld1sh	z0\.s, p0/z, \[x0, z1\.s, uxtw 1\]
**	ret
*/
TEST_LOAD_GATHER_SZ (ld1sh_gather_untied_s32_u32index, svint32_t, int16_t, svuint32_t,
		     z0_res = svld1sh_gather_u32index_s32 (p0, x0, z1),
		     z0_res = svld1sh_gather_index_s32 (p0, x0, z1))
