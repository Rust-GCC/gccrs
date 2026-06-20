/* { dg-do compile { target ia32 } } */
/* { dg-options "-O2 -fomit-frame-pointer -momit-leaf-frame-pointer -mtune=pentiumpro"  } */
/* Keep labels and directives ('.cfi_startproc', '.cfi_endproc').  */
/* { dg-final { check-function-bodies "**" "" "" { target *-*-* } {^\t?\.} } } */

/*
**foo:
**...
**	movl	\$3, %eax
**...
**	movl	%eax, 24\(%esp\)
**	movl	%eax, 28\(%esp\)
**	leal	24\(%esp\), %ebx
**	movl	%ebx, \(%esp\)
**	call	bar
**	movl	\$3, %ecx
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
  dst[0] = 3;
  asm volatile ("" : : : "memory");
  dst[1] = 3;
  bar (dst);
  dst[2] = 3;
  asm volatile ("" : : : "memory");
  dst[3] = 3;
  bar (dst);
}
