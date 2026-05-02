/* { dg-options "-O2" } */
/* { dg-final { check-function-bodies "**" "" } } */

#define TEST(T) \
  int test_##T(T a, T b) \
    { return (a == b) ? 0 : (a < b) ? -1 : 1; } \
  int test_u##T(unsigned T a, unsigned T b) \
    { return (a == b) ? 0 : (a < b) ? -1 : 1; }

TEST(char)
TEST(short)
TEST(int)
TEST(long)

/*
** test_char:
**	slt	.*
**	slt	.*
**	sub\.d	.*
**	jr	\$r1
*/

/*
** test_uchar:
**	sltu	.*
**	sltu	.*
**	sub\.d	.*
**	jr	\$r1
*/

/*
** test_short:
**	slt	.*
**	slt	.*
**	sub\.d	.*
**	jr	\$r1
*/

/*
** test_ushort:
**	sltu	.*
**	sltu	.*
**	sub\.d	.*
**	jr	\$r1
*/

/*
** test_int:
**	slt	.*
**	slt	.*
**	sub\.d	.*
**	jr	\$r1
*/

/*
** test_uint:
**	bstrpick\.d	.*,31,0
**	bstrpick\.d	.*,31,0
**	sltu	.*
**	sltu	.*
**	sub\.d	.*
**	jr	\$r1
*/

/*
** test_long:
**	slt	.*
**	slt	.*
**	sub\.d	.*
**	jr	\$r1
*/

/*
** test_ulong:
**	sltu	.*
**	sltu	.*
**	sub\.d	.*
**	jr	\$r1
*/
