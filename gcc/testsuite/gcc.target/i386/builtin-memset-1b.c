/* { dg-do compile } */
/* { dg-options "-O2 -march=x86-64 -mtune=znver3 -minline-all-stringops" } */
/* Keep labels and directives ('.cfi_startproc', '.cfi_endproc').  */
/* { dg-final { check-function-bodies "**" "" "" { target lp64 } {^\t?\.} } } */

/*
**foo:
**.LFB0:
**	.cfi_startproc
**	movzbl	%sil, %eax
**	movabsq	\$72340172838076673, %rdx
**	movq	%rdi, %rcx
**	leaq	8\(%rdi\), %rdi
**	imulq	%rdx, %rax
**	movq	%rax, -8\(%rdi\)
**	movq	%rax, 113\(%rdi\)
**	andq	\$-8, %rdi
**	subq	%rdi, %rcx
**	addl	\$129, %ecx
**	shrl	\$3, %ecx
**	rep stosq
**	ret
**	.cfi_endproc
**...
*/

#include "builtin-memset-1a.c"
