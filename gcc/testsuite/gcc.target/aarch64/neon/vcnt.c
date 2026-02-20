/* { dg-do compile } */
/* { dg-final { check-function-bodies "**" "" } } */

#include "arm_neon_test.h"

/*
** test_vcnt_u8:
** cnt	v0\.8b, v0\.8b
** ret
*/
TEST_UNARY (vcnt_u8, uint8x8_t, uint8x8_t)

/*
** test_vcnt_s8:
** cnt	v0\.8b, v0\.8b
** ret
*/
TEST_UNARY (vcnt_s8, int8x8_t, int8x8_t)

/*
** test_vcnt_p8:
** cnt	v0\.8b, v0\.8b
** ret
*/
TEST_UNARY (vcnt_p8, poly8x8_t, poly8x8_t)
