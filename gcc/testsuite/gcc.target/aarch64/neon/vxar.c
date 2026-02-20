/* { dg-do compile } */
/* { dg-final { check-function-bodies "**" "" } } */

#include "arm_neon_test.h"

/*
** test_vxarq_u64_0:
** eor	v0\.16b, (v0\.16b, v1\.16b|v1\.16b, v0\.16b)
** ret
*/
uint64x2_t test_vxarq_u64_0 (uint64x2_t a, uint64x2_t b) { return vxarq_u64 (a, b, 0); }

/*
** test_vxarq_u64_1:
** xar	v0\.2d, v0\.2d, v1\.2d, #?1
** ret
*/
uint64x2_t test_vxarq_u64_1 (uint64x2_t a, uint64x2_t b) { return vxarq_u64 (a, b, 1); }

/*
** test_vxarq_u64_31:
** xar	v0\.2d, v0\.2d, v1\.2d, #?31
** ret
*/
uint64x2_t test_vxarq_u64_31 (uint64x2_t a, uint64x2_t b) { return vxarq_u64 (a, b, 31); }

/*
** test_vxarq_u64_63:
** xar	v0\.2d, v0\.2d, v1\.2d, #?63
** ret
*/
uint64x2_t test_vxarq_u64_63 (uint64x2_t a, uint64x2_t b) { return vxarq_u64 (a, b, 63); }