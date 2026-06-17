/* { dg-do compile } */
/* { dg-options "-O2 -march=x86-64-v4 -minline-all-stringops" } */
/* Keep labels and directives ('.cfi_startproc', '.cfi_endproc').  */
/* { dg-final { check-function-bodies "**" "" "" { target { lp64 } } {^\t?\.} } } */

/*
**gcc_memcpy:
**.LFB0:
**	.cfi_startproc
**	cmpq	\$63, %rdx
**	jbe	.L12
**.L10:
**	ret
**	.p2align 4,,10
**	.p2align 3
**.L12:
**	cmpl	\$32, %edx
**	jnb	.L13
**	cmpl	\$16, %edx
**	jnb	.L5
**	cmpl	\$8, %edx
**	jnb	.L6
**	cmpl	\$4, %edx
**	jnb	.L7
**	cmpl	\$1, %edx
**	ja	.L8
**	jb	.L10
**	movzbl	\(%rsi\), %eax
**	movb	%al, \(%rdi\)
**	ret
**	.p2align 4,,10
**	.p2align 3
**.L13:
**	movl	%edx, %edx
**	vmovdqu	\(%rsi\), %ymm1
**	vmovdqu	-32\(%rsi,%rdx\), %ymm0
**	vmovdqu	%ymm1, \(%rdi\)
**	vmovdqu	%ymm0, -32\(%rdi,%rdx\)
**	vzeroupper
**	ret
**	.p2align 4,,10
**	.p2align 3
**.L5:
**	movl	%edx, %edx
**	vmovdqu	\(%rsi\), %xmm1
**	vmovdqu	-16\(%rsi,%rdx\), %xmm0
**	vmovdqu	%xmm1, \(%rdi\)
**	vmovdqu	%xmm0, -16\(%rdi,%rdx\)
**	ret
**	.p2align 4,,10
**	.p2align 3
**.L6:
**	movl	%edx, %edx
**	movq	\(%rsi\), %rcx
**	movq	-8\(%rsi,%rdx\), %rax
**	movq	%rcx, \(%rdi\)
**	movq	%rax, -8\(%rdi,%rdx\)
**	ret
**	.p2align 4,,10
**	.p2align 3
**.L7:
**	movl	%edx, %edx
**	movl	\(%rsi\), %ecx
**	movl	-4\(%rsi,%rdx\), %eax
**	movl	%ecx, \(%rdi\)
**	movl	%eax, -4\(%rdi,%rdx\)
**	ret
**	.p2align 4,,10
**	.p2align 3
**.L8:
**	movl	%edx, %edx
**	movzwl	\(%rsi\), %ecx
**	movzwl	-2\(%rsi,%rdx\), %eax
**	movw	%cx, \(%rdi\)
**	movw	%ax, -2\(%rdi,%rdx\)
**	ret
**...
*/

#include "builtin-memcpy-4a.c"
