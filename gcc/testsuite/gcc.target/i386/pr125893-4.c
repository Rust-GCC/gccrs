/* { dg-do compile } */
/* { dg-options "-O2 -mtune-ctrl=lcp_stall" } */
/* Keep labels and directives ('.cfi_startproc', '.cfi_endproc').  */
/* { dg-final { check-function-bodies "**" "" "" { target *-*-* } {^\t?\.} } } */

/*
**foo:
**...
**	xorl	%[a-z0-9]+, %[a-z0-9]+
**...
**	movw	%[a-z0-9]+, \(%[a-z0-9]+\)
**	movw	%[a-z0-9]+, 4\(%[a-z0-9]+\)
**	call	_?bar
**	xorl	%[a-z0-9]+, %[a-z0-9]+
**	movw	%[a-z0-9]+, 2\(%[a-z0-9]+\)
**	movw	%[a-z0-9]+, 8\(%[a-z0-9]+\)
**...
*/


extern void bar (void);

void
foo (short *dst)
{
  dst[0] = 0;
  asm volatile ("" : : : "memory");
  dst[2] = 0;
  bar ();
  dst[1] = 0;
  asm volatile ("" : : : "memory");
  dst[4] = 0;
}
