/* { dg-do compile } */
/* { dg-options "-O2 -march=x86-64 -mtune=znver3 -minline-all-stringops" } */
/* Keep labels and directives ('.cfi_startproc', '.cfi_endproc').  */
/* { dg-final { check-function-bodies "**" "" "" { target lp64 } {^\t?\.} } } */

/*
**foo:
**.LFB0:
**	.cfi_startproc
**	movl	\(%rsi\), %eax
**	movq	%rdi, %rcx
**	leaq	4\(%rdi\), %rdi
**	movl	%eax, -4\(%rdi\)
**	movl	125\(%rsi\), %eax
**	movl	%eax, 121\(%rdi\)
**	andq	\$-4, %rdi
**	subq	%rdi, %rcx
**	subq	%rcx, %rsi
**	addl	\$129, %ecx
**	shrl	\$2, %ecx
**	rep movsl
**	ret
**	.cfi_endproc
**...
*/

#include "builtin-memcpy-1a.c"
