/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" } } */

#include "test_sve_acle.h"

/*
** ldff1sb_gather_u64_tied1:
**	ldff1sb	z0\.d, p0/z, \[z0\.d\]
**	ret
*/
TEST_LOAD_GATHER_ZS (ldff1sb_gather_u64_tied1, svuint64_t, svuint64_t,
		     z0_res = svldff1sb_gather_u64base_u64 (p0, z0),
		     z0_res = svldff1sb_gather_u64 (p0, z0))

/*
** ldff1sb_gather_u64_untied:
**	ldff1sb	z0\.d, p0/z, \[z1\.d\]
**	ret
*/
TEST_LOAD_GATHER_ZS (ldff1sb_gather_u64_untied, svuint64_t, svuint64_t,
		     z0_res = svldff1sb_gather_u64base_u64 (p0, z1),
		     z0_res = svldff1sb_gather_u64 (p0, z1))

/*
** ldff1sb_gather_x0_u64_offset:
**	ldff1sb	z0\.d, p0/z, \[x0, z0\.d\]
**	ret
*/
TEST_LOAD_GATHER_ZS (ldff1sb_gather_x0_u64_offset, svuint64_t, svuint64_t,
		     z0_res = svldff1sb_gather_u64base_offset_u64 (p0, z0, x0),
		     z0_res = svldff1sb_gather_offset_u64 (p0, z0, x0))

/*
** ldff1sb_gather_m1_u64_offset:
**	mov	(x[0-9]+), #?-1
**	ldff1sb	z0\.d, p0/z, \[\1, z0\.d\]
**	ret
*/
TEST_LOAD_GATHER_ZS (ldff1sb_gather_m1_u64_offset, svuint64_t, svuint64_t,
		     z0_res = svldff1sb_gather_u64base_offset_u64 (p0, z0, -1),
		     z0_res = svldff1sb_gather_offset_u64 (p0, z0, -1))

/*
** ldff1sb_gather_0_u64_offset:
**	ldff1sb	z0\.d, p0/z, \[z0\.d\]
**	ret
*/
TEST_LOAD_GATHER_ZS (ldff1sb_gather_0_u64_offset, svuint64_t, svuint64_t,
		     z0_res = svldff1sb_gather_u64base_offset_u64 (p0, z0, 0),
		     z0_res = svldff1sb_gather_offset_u64 (p0, z0, 0))

/*
** ldff1sb_gather_5_u64_offset:
**	ldff1sb	z0\.d, p0/z, \[z0\.d, #5\]
**	ret
*/
TEST_LOAD_GATHER_ZS (ldff1sb_gather_5_u64_offset, svuint64_t, svuint64_t,
		     z0_res = svldff1sb_gather_u64base_offset_u64 (p0, z0, 5),
		     z0_res = svldff1sb_gather_offset_u64 (p0, z0, 5))

/*
** ldff1sb_gather_31_u64_offset:
**	ldff1sb	z0\.d, p0/z, \[z0\.d, #31\]
**	ret
*/
TEST_LOAD_GATHER_ZS (ldff1sb_gather_31_u64_offset, svuint64_t, svuint64_t,
		     z0_res = svldff1sb_gather_u64base_offset_u64 (p0, z0, 31),
		     z0_res = svldff1sb_gather_offset_u64 (p0, z0, 31))

/*
** ldff1sb_gather_32_u64_offset:
**	mov	(x[0-9]+), #?32
**	ldff1sb	z0\.d, p0/z, \[\1, z0\.d\]
**	ret
*/
TEST_LOAD_GATHER_ZS (ldff1sb_gather_32_u64_offset, svuint64_t, svuint64_t,
		     z0_res = svldff1sb_gather_u64base_offset_u64 (p0, z0, 32),
		     z0_res = svldff1sb_gather_offset_u64 (p0, z0, 32))

/*
** ldff1sb_gather_x0_u64_s64offset:
**	ldff1sb	z0\.d, p0/z, \[x0, z0\.d\]
**	ret
*/
TEST_LOAD_GATHER_SZ (ldff1sb_gather_x0_u64_s64offset, svuint64_t, int8_t, svint64_t,
		     z0_res = svldff1sb_gather_s64offset_u64 (p0, x0, z0),
		     z0_res = svldff1sb_gather_offset_u64 (p0, x0, z0))

/*
** ldff1sb_gather_tied1_u64_s64offset:
**	ldff1sb	z0\.d, p0/z, \[x0, z0\.d\]
**	ret
*/
TEST_LOAD_GATHER_SZ (ldff1sb_gather_tied1_u64_s64offset, svuint64_t, int8_t, svint64_t,
		     z0_res = svldff1sb_gather_s64offset_u64 (p0, x0, z0),
		     z0_res = svldff1sb_gather_offset_u64 (p0, x0, z0))

/*
** ldff1sb_gather_untied_u64_s64offset:
**	ldff1sb	z0\.d, p0/z, \[x0, z1\.d\]
**	ret
*/
TEST_LOAD_GATHER_SZ (ldff1sb_gather_untied_u64_s64offset, svuint64_t, int8_t, svint64_t,
		     z0_res = svldff1sb_gather_s64offset_u64 (p0, x0, z1),
		     z0_res = svldff1sb_gather_offset_u64 (p0, x0, z1))

/*
** ldff1sb_gather_ext_u64_s64offset:
**	ldff1sb	z0\.d, p0/z, \[x0, z1\.d, sxtw\]
**	ret
*/
TEST_LOAD_GATHER_SZ (ldff1sb_gather_ext_u64_s64offset, svuint64_t, int8_t, svint64_t,
		     z0_res = svldff1sb_gather_s64offset_u64 (p0, x0, svextw_s64_x (p0, z1)),
		     z0_res = svldff1sb_gather_offset_u64 (p0, x0, svextw_x (p0, z1)))

/*
** ldff1sb_gather_x0_u64_u64offset:
**	ldff1sb	z0\.d, p0/z, \[x0, z0\.d\]
**	ret
*/
TEST_LOAD_GATHER_SZ (ldff1sb_gather_x0_u64_u64offset, svuint64_t, int8_t, svuint64_t,
		     z0_res = svldff1sb_gather_u64offset_u64 (p0, x0, z0),
		     z0_res = svldff1sb_gather_offset_u64 (p0, x0, z0))

/*
** ldff1sb_gather_tied1_u64_u64offset:
**	ldff1sb	z0\.d, p0/z, \[x0, z0\.d\]
**	ret
*/
TEST_LOAD_GATHER_SZ (ldff1sb_gather_tied1_u64_u64offset, svuint64_t, int8_t, svuint64_t,
		     z0_res = svldff1sb_gather_u64offset_u64 (p0, x0, z0),
		     z0_res = svldff1sb_gather_offset_u64 (p0, x0, z0))

/*
** ldff1sb_gather_untied_u64_u64offset:
**	ldff1sb	z0\.d, p0/z, \[x0, z1\.d\]
**	ret
*/
TEST_LOAD_GATHER_SZ (ldff1sb_gather_untied_u64_u64offset, svuint64_t, int8_t, svuint64_t,
		     z0_res = svldff1sb_gather_u64offset_u64 (p0, x0, z1),
		     z0_res = svldff1sb_gather_offset_u64 (p0, x0, z1))

/*
** ldff1sb_gather_ext_u64_u64offset:
**	ldff1sb	z0\.d, p0/z, \[x0, z1\.d, uxtw\]
**	ret
*/
TEST_LOAD_GATHER_SZ (ldff1sb_gather_ext_u64_u64offset, svuint64_t, int8_t, svuint64_t,
		     z0_res = svldff1sb_gather_u64offset_u64 (p0, x0, svextw_u64_x (p0, z1)),
		     z0_res = svldff1sb_gather_offset_u64 (p0, x0, svextw_x (p0, z1)))
