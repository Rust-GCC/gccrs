/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" } } */

#include "test_sve_acle.h"

/*
** ldff1sw_gather_s64_tied1:
**	ldff1sw	z0\.d, p0/z, \[z0\.d\]
**	ret
*/
TEST_LOAD_GATHER_ZS (ldff1sw_gather_s64_tied1, svint64_t, svuint64_t,
		     z0_res = svldff1sw_gather_u64base_s64 (p0, z0),
		     z0_res = svldff1sw_gather_s64 (p0, z0))

/*
** ldff1sw_gather_s64_untied:
**	ldff1sw	z0\.d, p0/z, \[z1\.d\]
**	ret
*/
TEST_LOAD_GATHER_ZS (ldff1sw_gather_s64_untied, svint64_t, svuint64_t,
		     z0_res = svldff1sw_gather_u64base_s64 (p0, z1),
		     z0_res = svldff1sw_gather_s64 (p0, z1))

/*
** ldff1sw_gather_x0_s64_offset:
**	ldff1sw	z0\.d, p0/z, \[x0, z0\.d\]
**	ret
*/
TEST_LOAD_GATHER_ZS (ldff1sw_gather_x0_s64_offset, svint64_t, svuint64_t,
		     z0_res = svldff1sw_gather_u64base_offset_s64 (p0, z0, x0),
		     z0_res = svldff1sw_gather_offset_s64 (p0, z0, x0))

/*
** ldff1sw_gather_m4_s64_offset:
**	mov	(x[0-9]+), #?-4
**	ldff1sw	z0\.d, p0/z, \[\1, z0\.d\]
**	ret
*/
TEST_LOAD_GATHER_ZS (ldff1sw_gather_m4_s64_offset, svint64_t, svuint64_t,
		     z0_res = svldff1sw_gather_u64base_offset_s64 (p0, z0, -4),
		     z0_res = svldff1sw_gather_offset_s64 (p0, z0, -4))

/*
** ldff1sw_gather_0_s64_offset:
**	ldff1sw	z0\.d, p0/z, \[z0\.d\]
**	ret
*/
TEST_LOAD_GATHER_ZS (ldff1sw_gather_0_s64_offset, svint64_t, svuint64_t,
		     z0_res = svldff1sw_gather_u64base_offset_s64 (p0, z0, 0),
		     z0_res = svldff1sw_gather_offset_s64 (p0, z0, 0))

/*
** ldff1sw_gather_5_s64_offset:
**	mov	(x[0-9]+), #?5
**	ldff1sw	z0\.d, p0/z, \[\1, z0\.d\]
**	ret
*/
TEST_LOAD_GATHER_ZS (ldff1sw_gather_5_s64_offset, svint64_t, svuint64_t,
		     z0_res = svldff1sw_gather_u64base_offset_s64 (p0, z0, 5),
		     z0_res = svldff1sw_gather_offset_s64 (p0, z0, 5))

/*
** ldff1sw_gather_6_s64_offset:
**	mov	(x[0-9]+), #?6
**	ldff1sw	z0\.d, p0/z, \[\1, z0\.d\]
**	ret
*/
TEST_LOAD_GATHER_ZS (ldff1sw_gather_6_s64_offset, svint64_t, svuint64_t,
		     z0_res = svldff1sw_gather_u64base_offset_s64 (p0, z0, 6),
		     z0_res = svldff1sw_gather_offset_s64 (p0, z0, 6))

/*
** ldff1sw_gather_7_s64_offset:
**	mov	(x[0-9]+), #?7
**	ldff1sw	z0\.d, p0/z, \[\1, z0\.d\]
**	ret
*/
TEST_LOAD_GATHER_ZS (ldff1sw_gather_7_s64_offset, svint64_t, svuint64_t,
		     z0_res = svldff1sw_gather_u64base_offset_s64 (p0, z0, 7),
		     z0_res = svldff1sw_gather_offset_s64 (p0, z0, 7))

/*
** ldff1sw_gather_8_s64_offset:
**	ldff1sw	z0\.d, p0/z, \[z0\.d, #8\]
**	ret
*/
TEST_LOAD_GATHER_ZS (ldff1sw_gather_8_s64_offset, svint64_t, svuint64_t,
		     z0_res = svldff1sw_gather_u64base_offset_s64 (p0, z0, 8),
		     z0_res = svldff1sw_gather_offset_s64 (p0, z0, 8))

/*
** ldff1sw_gather_124_s64_offset:
**	ldff1sw	z0\.d, p0/z, \[z0\.d, #124\]
**	ret
*/
TEST_LOAD_GATHER_ZS (ldff1sw_gather_124_s64_offset, svint64_t, svuint64_t,
		     z0_res = svldff1sw_gather_u64base_offset_s64 (p0, z0, 124),
		     z0_res = svldff1sw_gather_offset_s64 (p0, z0, 124))

/*
** ldff1sw_gather_128_s64_offset:
**	mov	(x[0-9]+), #?128
**	ldff1sw	z0\.d, p0/z, \[\1, z0\.d\]
**	ret
*/
TEST_LOAD_GATHER_ZS (ldff1sw_gather_128_s64_offset, svint64_t, svuint64_t,
		     z0_res = svldff1sw_gather_u64base_offset_s64 (p0, z0, 128),
		     z0_res = svldff1sw_gather_offset_s64 (p0, z0, 128))

/*
** ldff1sw_gather_x0_s64_index:
**	lsl	(x[0-9]+), x0, #?2
**	ldff1sw	z0\.d, p0/z, \[\1, z0\.d\]
**	ret
*/
TEST_LOAD_GATHER_ZS (ldff1sw_gather_x0_s64_index, svint64_t, svuint64_t,
		     z0_res = svldff1sw_gather_u64base_index_s64 (p0, z0, x0),
		     z0_res = svldff1sw_gather_index_s64 (p0, z0, x0))

/*
** ldff1sw_gather_m1_s64_index:
**	mov	(x[0-9]+), #?-4
**	ldff1sw	z0\.d, p0/z, \[\1, z0\.d\]
**	ret
*/
TEST_LOAD_GATHER_ZS (ldff1sw_gather_m1_s64_index, svint64_t, svuint64_t,
		     z0_res = svldff1sw_gather_u64base_index_s64 (p0, z0, -1),
		     z0_res = svldff1sw_gather_index_s64 (p0, z0, -1))

/*
** ldff1sw_gather_0_s64_index:
**	ldff1sw	z0\.d, p0/z, \[z0\.d\]
**	ret
*/
TEST_LOAD_GATHER_ZS (ldff1sw_gather_0_s64_index, svint64_t, svuint64_t,
		     z0_res = svldff1sw_gather_u64base_index_s64 (p0, z0, 0),
		     z0_res = svldff1sw_gather_index_s64 (p0, z0, 0))

/*
** ldff1sw_gather_5_s64_index:
**	ldff1sw	z0\.d, p0/z, \[z0\.d, #20\]
**	ret
*/
TEST_LOAD_GATHER_ZS (ldff1sw_gather_5_s64_index, svint64_t, svuint64_t,
		     z0_res = svldff1sw_gather_u64base_index_s64 (p0, z0, 5),
		     z0_res = svldff1sw_gather_index_s64 (p0, z0, 5))

/*
** ldff1sw_gather_31_s64_index:
**	ldff1sw	z0\.d, p0/z, \[z0\.d, #124\]
**	ret
*/
TEST_LOAD_GATHER_ZS (ldff1sw_gather_31_s64_index, svint64_t, svuint64_t,
		     z0_res = svldff1sw_gather_u64base_index_s64 (p0, z0, 31),
		     z0_res = svldff1sw_gather_index_s64 (p0, z0, 31))

/*
** ldff1sw_gather_32_s64_index:
**	mov	(x[0-9]+), #?128
**	ldff1sw	z0\.d, p0/z, \[\1, z0\.d\]
**	ret
*/
TEST_LOAD_GATHER_ZS (ldff1sw_gather_32_s64_index, svint64_t, svuint64_t,
		     z0_res = svldff1sw_gather_u64base_index_s64 (p0, z0, 32),
		     z0_res = svldff1sw_gather_index_s64 (p0, z0, 32))

/*
** ldff1sw_gather_x0_s64_s64offset:
**	ldff1sw	z0\.d, p0/z, \[x0, z0\.d\]
**	ret
*/
TEST_LOAD_GATHER_SZ (ldff1sw_gather_x0_s64_s64offset, svint64_t, int32_t, svint64_t,
		     z0_res = svldff1sw_gather_s64offset_s64 (p0, x0, z0),
		     z0_res = svldff1sw_gather_offset_s64 (p0, x0, z0))

/*
** ldff1sw_gather_tied1_s64_s64offset:
**	ldff1sw	z0\.d, p0/z, \[x0, z0\.d\]
**	ret
*/
TEST_LOAD_GATHER_SZ (ldff1sw_gather_tied1_s64_s64offset, svint64_t, int32_t, svint64_t,
		     z0_res = svldff1sw_gather_s64offset_s64 (p0, x0, z0),
		     z0_res = svldff1sw_gather_offset_s64 (p0, x0, z0))

/*
** ldff1sw_gather_untied_s64_s64offset:
**	ldff1sw	z0\.d, p0/z, \[x0, z1\.d\]
**	ret
*/
TEST_LOAD_GATHER_SZ (ldff1sw_gather_untied_s64_s64offset, svint64_t, int32_t, svint64_t,
		     z0_res = svldff1sw_gather_s64offset_s64 (p0, x0, z1),
		     z0_res = svldff1sw_gather_offset_s64 (p0, x0, z1))

/*
** ldff1sw_gather_ext_s64_s64offset:
**	ldff1sw	z0\.d, p0/z, \[x0, z1\.d, sxtw\]
**	ret
*/
TEST_LOAD_GATHER_SZ (ldff1sw_gather_ext_s64_s64offset, svint64_t, int32_t, svint64_t,
		     z0_res = svldff1sw_gather_s64offset_s64 (p0, x0, svextw_s64_x (p0, z1)),
		     z0_res = svldff1sw_gather_offset_s64 (p0, x0, svextw_x (p0, z1)))

/*
** ldff1sw_gather_x0_s64_u64offset:
**	ldff1sw	z0\.d, p0/z, \[x0, z0\.d\]
**	ret
*/
TEST_LOAD_GATHER_SZ (ldff1sw_gather_x0_s64_u64offset, svint64_t, int32_t, svuint64_t,
		     z0_res = svldff1sw_gather_u64offset_s64 (p0, x0, z0),
		     z0_res = svldff1sw_gather_offset_s64 (p0, x0, z0))

/*
** ldff1sw_gather_tied1_s64_u64offset:
**	ldff1sw	z0\.d, p0/z, \[x0, z0\.d\]
**	ret
*/
TEST_LOAD_GATHER_SZ (ldff1sw_gather_tied1_s64_u64offset, svint64_t, int32_t, svuint64_t,
		     z0_res = svldff1sw_gather_u64offset_s64 (p0, x0, z0),
		     z0_res = svldff1sw_gather_offset_s64 (p0, x0, z0))

/*
** ldff1sw_gather_untied_s64_u64offset:
**	ldff1sw	z0\.d, p0/z, \[x0, z1\.d\]
**	ret
*/
TEST_LOAD_GATHER_SZ (ldff1sw_gather_untied_s64_u64offset, svint64_t, int32_t, svuint64_t,
		     z0_res = svldff1sw_gather_u64offset_s64 (p0, x0, z1),
		     z0_res = svldff1sw_gather_offset_s64 (p0, x0, z1))

/*
** ldff1sw_gather_ext_s64_u64offset:
**	ldff1sw	z0\.d, p0/z, \[x0, z1\.d, uxtw\]
**	ret
*/
TEST_LOAD_GATHER_SZ (ldff1sw_gather_ext_s64_u64offset, svint64_t, int32_t, svuint64_t,
		     z0_res = svldff1sw_gather_u64offset_s64 (p0, x0, svextw_u64_x (p0, z1)),
		     z0_res = svldff1sw_gather_offset_s64 (p0, x0, svextw_x (p0, z1)))

/*
** ldff1sw_gather_x0_s64_s64index:
**	ldff1sw	z0\.d, p0/z, \[x0, z0\.d, lsl 2\]
**	ret
*/
TEST_LOAD_GATHER_SZ (ldff1sw_gather_x0_s64_s64index, svint64_t, int32_t, svint64_t,
		     z0_res = svldff1sw_gather_s64index_s64 (p0, x0, z0),
		     z0_res = svldff1sw_gather_index_s64 (p0, x0, z0))

/*
** ldff1sw_gather_tied1_s64_s64index:
**	ldff1sw	z0\.d, p0/z, \[x0, z0\.d, lsl 2\]
**	ret
*/
TEST_LOAD_GATHER_SZ (ldff1sw_gather_tied1_s64_s64index, svint64_t, int32_t, svint64_t,
		     z0_res = svldff1sw_gather_s64index_s64 (p0, x0, z0),
		     z0_res = svldff1sw_gather_index_s64 (p0, x0, z0))

/*
** ldff1sw_gather_untied_s64_s64index:
**	ldff1sw	z0\.d, p0/z, \[x0, z1\.d, lsl 2\]
**	ret
*/
TEST_LOAD_GATHER_SZ (ldff1sw_gather_untied_s64_s64index, svint64_t, int32_t, svint64_t,
		     z0_res = svldff1sw_gather_s64index_s64 (p0, x0, z1),
		     z0_res = svldff1sw_gather_index_s64 (p0, x0, z1))

/*
** ldff1sw_gather_ext_s64_s64index:
**	ldff1sw	z0\.d, p0/z, \[x0, z1\.d, sxtw 2\]
**	ret
*/
TEST_LOAD_GATHER_SZ (ldff1sw_gather_ext_s64_s64index, svint64_t, int32_t, svint64_t,
		     z0_res = svldff1sw_gather_s64index_s64 (p0, x0, svextw_s64_x (p0, z1)),
		     z0_res = svldff1sw_gather_index_s64 (p0, x0, svextw_x (p0, z1)))

/*
** ldff1sw_gather_x0_s64_u64index:
**	ldff1sw	z0\.d, p0/z, \[x0, z0\.d, lsl 2\]
**	ret
*/
TEST_LOAD_GATHER_SZ (ldff1sw_gather_x0_s64_u64index, svint64_t, int32_t, svuint64_t,
		     z0_res = svldff1sw_gather_u64index_s64 (p0, x0, z0),
		     z0_res = svldff1sw_gather_index_s64 (p0, x0, z0))

/*
** ldff1sw_gather_tied1_s64_u64index:
**	ldff1sw	z0\.d, p0/z, \[x0, z0\.d, lsl 2\]
**	ret
*/
TEST_LOAD_GATHER_SZ (ldff1sw_gather_tied1_s64_u64index, svint64_t, int32_t, svuint64_t,
		     z0_res = svldff1sw_gather_u64index_s64 (p0, x0, z0),
		     z0_res = svldff1sw_gather_index_s64 (p0, x0, z0))

/*
** ldff1sw_gather_untied_s64_u64index:
**	ldff1sw	z0\.d, p0/z, \[x0, z1\.d, lsl 2\]
**	ret
*/
TEST_LOAD_GATHER_SZ (ldff1sw_gather_untied_s64_u64index, svint64_t, int32_t, svuint64_t,
		     z0_res = svldff1sw_gather_u64index_s64 (p0, x0, z1),
		     z0_res = svldff1sw_gather_index_s64 (p0, x0, z1))

/*
** ldff1sw_gather_ext_s64_u64index:
**	ldff1sw	z0\.d, p0/z, \[x0, z1\.d, uxtw 2\]
**	ret
*/
TEST_LOAD_GATHER_SZ (ldff1sw_gather_ext_s64_u64index, svint64_t, int32_t, svuint64_t,
		     z0_res = svldff1sw_gather_u64index_s64 (p0, x0, svextw_u64_x (p0, z1)),
		     z0_res = svldff1sw_gather_index_s64 (p0, x0, svextw_x (p0, z1)))
