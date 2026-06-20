/* { dg-do compile { target ia32 } } */
/* { dg-options "-O2 -fomit-frame-pointer -momit-leaf-frame-pointer -mtune=pentiumpro"  } */
/* Keep labels and directives ('.cfi_startproc', '.cfi_endproc').  */
/* { dg-final { check-function-bodies "**" "" "" { target *-*-* } {^\t?\.} } } */

/*
**foo:
**...
**	movl	\$3, %eax
**	movl	%eax, 16\(%esp\)
**	movl	%eax, 20\(%esp\)
**	movl	%eax, 24\(%esp\)
**	movl	%eax, 28\(%esp\)
**...
*/

extern void bar (int *dst);

void
foo (void)
{
  int dst[16];
  dst[0] = 3;
  asm volatile ("" : : : "memory");
  dst[1] = 3;
  asm volatile ("" : : : "memory");
  dst[2] = 3;
  asm volatile ("" : : : "memory");
  dst[3] = 3;
  bar (dst);
}
