/* { dg-do compile } */
/* { dg-final { check-function-bodies "**" "" } } */

#include "arm_neon_test.h"

/*
** test_vrbit_u8:
** rbit	v0\.8b, v0\.8b
** ret
*/
TEST_UNIFORM_UNARY (vrbit_u8, uint8x8_t)

/*
** test_vrbit_s8:
** rbit	v0\.8b, v0\.8b
** ret
*/
TEST_UNIFORM_UNARY (vrbit_s8, int8x8_t)

/*
** test_vrbit_p8:
** rbit	v0\.8b, v0\.8b
** ret
*/
TEST_UNIFORM_UNARY (vrbit_p8, poly8x8_t)

/*
** test_vrbitq_u8:
** rbit	v0\.16b, v0\.16b
** ret
*/
TEST_UNIFORM_UNARY (vrbitq_u8, uint8x16_t)

/*
** test_vrbitq_s8:
** rbit	v0\.16b, v0\.16b
** ret
*/
TEST_UNIFORM_UNARY (vrbitq_s8, int8x16_t)

/*
** test_vrbitq_p8:
** rbit	v0\.16b, v0\.16b
** ret
*/
TEST_UNIFORM_UNARY (vrbitq_p8, poly8x16_t)
