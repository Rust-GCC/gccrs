/* { dg-do compile } */
/* { dg-options "-O2 -march=x86-64-v3 -minline-all-stringops" } */
/* Keep labels and directives ('.cfi_startproc', '.cfi_endproc').  */
/* { dg-final { check-function-bodies "**" "" "" { target lp64 } {^\t?\.} } } */

/*
**foo:
**.LFB0:
**	.cfi_startproc
**	cmpq	\$63, %rdx
**	jbe	.L13
**	ret
**	.p2align 4,,10
**	.p2align 3
**.L13:
**	movabsq	\$72340172838076673, %rcx
**	movzbl	%sil, %eax
**	imulq	%rcx, %rax
**	vmovq	%rax, %xmm1
**	vpbroadcastq	%xmm1, %ymm0
**	cmpl	\$32, %edx
**	jnb	.L14
**	cmpl	\$16, %edx
**	jnb	.L5
**	cmpl	\$8, %edx
**	jnb	.L6
**	cmpl	\$4, %edx
**	jnb	.L7
**	cmpl	\$1, %edx
**	ja	.L8
**	jb	.L10
**	movb	%sil, \(%rdi\)
**	vzeroupper
**	ret
**	.p2align 4,,10
**	.p2align 3
**.L14:
**	movl	%edx, %edx
**	vmovdqu	%ymm0, \(%rdi\)
**	vmovdqu	%ymm0, -32\(%rdi,%rdx\)
**	vzeroupper
**	ret
**	.p2align 4,,10
**	.p2align 3
**.L5:
**	movl	%edx, %edx
**	vmovdqu	%xmm0, \(%rdi\)
**	vmovdqu	%xmm0, -16\(%rdi,%rdx\)
**	vzeroupper
**	ret
**	.p2align 4,,10
**	.p2align 3
**.L6:
**	movl	%edx, %edx
**	movq	%rax, \(%rdi\)
**	movq	%rax, -8\(%rdi,%rdx\)
**	vzeroupper
**	ret
**	.p2align 4,,10
**	.p2align 3
**.L7:
**	movl	%edx, %edx
**	movl	%eax, \(%rdi\)
**	movl	%eax, -4\(%rdi,%rdx\)
**	vzeroupper
**	ret
**	.p2align 4,,10
**	.p2align 3
**.L8:
**	movl	%edx, %edx
**	movw	%ax, \(%rdi\)
**	movw	%ax, -2\(%rdi,%rdx\)
**	vzeroupper
**	ret
**.L10:
**	vzeroupper
**	ret
**	.cfi_endproc
**...
*/

#include "builtin-memset-4a.c"
