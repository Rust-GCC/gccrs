/* Verify ext-dce for word loads on RV64: lw sign-extends to 64 bits,
   so when the upper 32 bits are dead lw should be narrowed to lwu.  */
/* { dg-do compile } */
/* { dg-require-effective-target rv64 } */
/* { dg-options "-march=rv64gc -mabi=lp64d -O2" } */
/* { dg-final { check-function-bodies "**" "" } } */

/*
** test_word:
**	...
**	lwu	.*
**	...
*/
/* Positive: word load-modify-store — upper 32 bits are dead.  */
void
test_word (signed int *p)
{
  *p = (*p & 0xffff0000) | (0x0000ffff & ((unsigned int)*p >> 16));
}

/*
** test_word_sign_needed:
**	...
**	lw	.*
**	...
*/
/* Negative: return value is long — sign extension is needed.  */
long
test_word_sign_needed (signed int *p)
{
  return *p;
}

