/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" } } */

#include "test_sve_acle.h"

/*
** cnt_f16_m_tied1:
**	cnt	z0\.h, p0/m, z4\.h
**	ret
*/
TEST_DUAL_Z (cnt_f16_m_tied1, svuint16_t, svfloat16_t,
	     z0 = svcnt_f16_m (z0, p0, z4),
	     z0 = svcnt_m (z0, p0, z4))

/*
** cnt_f16_m_untied:
**	movprfx	z0, z1
**	cnt	z0\.h, p0/m, z4\.h
**	ret
*/
TEST_DUAL_Z (cnt_f16_m_untied, svuint16_t, svfloat16_t,
	     z0 = svcnt_f16_m (z1, p0, z4),
	     z0 = svcnt_m (z1, p0, z4))

/*
** cnt_f16_z:
**	movprfx	z0\.h, p0/z, z4\.h
**	cnt	z0\.h, p0/m, z4\.h
**	ret
*/
TEST_DUAL_Z (cnt_f16_z, svuint16_t, svfloat16_t,
	     z0 = svcnt_f16_z (p0, z4),
	     z0 = svcnt_z (p0, z4))

/*
** cnt_f16_x:
**	cnt	z0\.h, p0/m, z4\.h
**	ret
*/
TEST_DUAL_Z (cnt_f16_x, svuint16_t, svfloat16_t,
	     z0 = svcnt_f16_x (p0, z4),
	     z0 = svcnt_x (p0, z4))

/*
** ptrue_cnt_f16_x:
**	...
**	ptrue	p[0-9]+\.b[^\n]*
**	...
**	ret
*/
TEST_DUAL_Z (ptrue_cnt_f16_x, svuint16_t, svfloat16_t,
	     z0 = svcnt_f16_x (svptrue_b16 (), z4),
	     z0 = svcnt_x (svptrue_b16 (), z4))
