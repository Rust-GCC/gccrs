/* { dg-do compile { target ia32 } } */
/* { dg-options "-O2 -fomit-frame-pointer -momit-leaf-frame-pointer -mtune=pentiumpro"  } */
/* Keep labels and directives ('.cfi_startproc', '.cfi_endproc').  */
/* { dg-final { check-function-bodies "**" "" "" { target *-*-* } {^\t?\.} } } */

/*
**foo:
**...
**	xorl	%eax, %eax
**...
**	movl	%eax, 24\(%esp\)
**	movl	%eax, 28\(%esp\)
**	leal	24\(%esp\), %ebx
**	movl	%ebx, \(%esp\)
**	call	bar
**	xorl	%ecx, %ecx
**	movl	%ecx, 32\(%esp\)
**	movl	%ebx, \(%esp\)
**	movl	%ecx, 36\(%esp\)
**	call	bar
**...
*/

extern void bar (int *dst);

void
foo (void)
{
  int dst[10];
  dst[0] = 0;
  asm volatile ("" : : : "memory");
  dst[1] = 0;
  bar (dst);
  dst[2] = 0;
  asm volatile ("" : : : "memory");
  dst[3] = 0;
  bar (dst);
}
