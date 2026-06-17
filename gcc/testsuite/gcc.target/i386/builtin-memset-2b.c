/* { dg-do compile } */
/* { dg-options "-O2 -march=x86-64-v3 -minline-all-stringops" } */
/* Keep labels and directives ('.cfi_startproc', '.cfi_endproc').  */
/* { dg-final { check-function-bodies "**" "" "" { target lp64 } {^\t?\.} } } */

/*
**foo:
**.LFB0:
**	.cfi_startproc
**	cmpq	\$15, %rdx
**	jbe	.L10
**.L8:
**	ret
**	.p2align 4,,10
**	.p2align 3
**.L10:
**	movabsq	\$72340172838076673, %rcx
**	movzbl	%sil, %eax
**	imulq	%rcx, %rax
**	cmpl	\$8, %edx
**	jnb	.L11
**	cmpl	\$4, %edx
**	jnb	.L5
**	cmpl	\$1, %edx
**	ja	.L6
**	jb	.L8
**	movb	%sil, \(%rdi\)
**	ret
**	.p2align 4,,10
**	.p2align 3
**.L11:
**	movl	%edx, %edx
**	movq	%rax, \(%rdi\)
**	movq	%rax, -8\(%rdi,%rdx\)
**	ret
**	.p2align 4,,10
**	.p2align 3
**.L5:
**	movl	%edx, %edx
**	movl	%eax, \(%rdi\)
**	movl	%eax, -4\(%rdi,%rdx\)
**	ret
**	.p2align 4,,10
**	.p2align 3
**.L6:
**	movl	%edx, %edx
**	movw	%ax, \(%rdi\)
**	movw	%ax, -2\(%rdi,%rdx\)
**	ret
**	.cfi_endproc
**...
*/

#include "builtin-memset-2a.c"
