/* { dg-do compile } */
/* { dg-options "-O2 -mnarrow-gp-writes -fdump-rtl-narrow_gp_writes" } */
/* { dg-final { check-function-bodies "**" "" "" } } */
/* { dg-final { scan-rtl-dump-times "Successfully narrowed insn" 2 "narrow_gp_writes" } } */

/*
** test_phi1:
**	...
**	csel	w2, w2, w1, eq
**	add	w0, w2, 1
**	...
*/
unsigned long
test_phi1 (int cond, unsigned long a, unsigned long b)
{
  unsigned long x;
  if (cond)
    x = a & 0xFF;
  else
    x = b & 0xFFFF;
  
  return x + 1;
}
