/* { dg-do compile } */
/* { dg-options "-O2 -mtune-ctrl=lcp_stall" } */
/* Keep labels and directives ('.cfi_startproc', '.cfi_endproc').  */
/* { dg-final { check-function-bodies "**" "" "" { target *-*-* } {^\t?\.} } } */

/*
**foo:
**...
**	movl	\$3, %[a-z0-9]+
**	movw	%[a-z0-9]+, \(%[a-z0-9]+\)
**	movw	%[a-z0-9]+, 2\(%[a-z0-9]+\)
**	movw	%[a-z0-9]+, 4\(%[a-z0-9]+\)
**	movw	%[a-z0-9]+, 6\(%[a-z0-9]+\)
**...
*/

void
foo (short *dst)
{
  dst[0] = 3;
  asm volatile ("" : : : "memory");
  dst[1] = 3;
  asm volatile ("" : : : "memory");
  dst[2] = 3;
  asm volatile ("" : : : "memory");
  dst[3] = 3;
}
