/* Verify that ext-dce narrows sign-extending loads to zero-extending loads
   when the upper bits are dead.  */
/* { dg-do compile } */
/* { dg-options "-O2" } */
/* { dg-final { check-function-bodies "**" "" } } */

/*
** test_half:
**	...
**	ldrh	.*
**	...
*/
/* Positive: halfword load-modify-store — sign bits are dead.  */
void
test_half (signed short *p)
{
  *p = (*p & 0xff00) | (0x00ff & (*p >> 8));
}

/*
** test_byte:
**	...
**	ldrb	.*
**	...
*/
/* Positive: byte load-modify-store — sign bits are dead.  */
void
test_byte (signed char *p)
{
  *p = (*p & 0xf0) | (0x0f & (*p >> 4));
}

/*
** test_half_sign_needed:
**	...
**	ldrsb	.*
**	...
*/
/* Negative: arithmetic right shift needs the sign extension.  */
int
test_half_sign_needed (signed short *p)
{
  return *p >> 8;
}

/*
** test_half_compare:
**	...
**	ldrsh	.*
**	...
*/
/* Negative: sign-dependent comparison.  */
int
test_half_compare (signed short *p)
{
  return *p < 0;
}

