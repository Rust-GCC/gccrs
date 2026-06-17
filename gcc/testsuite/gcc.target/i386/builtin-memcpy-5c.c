/* { dg-do compile } */
/* { dg-options "-O2 -march=x86-64-v4 -minline-all-stringops" } */
/* Keep labels and directives ('.cfi_startproc', '.cfi_endproc').  */
/* { dg-final { check-function-bodies "**" "" "" { target { lp64 } } {^\t?\.} } } */

/*
**gcc_memcpy:
**.LFB0:
**	.cfi_startproc
**	cmpq	\$127, %rdx
**	jbe	.L13
**.L11:
**	ret
**	.p2align 4,,10
**	.p2align 3
**.L13:
**	cmpl	\$64, %edx
**	jnb	.L14
**	cmpl	\$32, %edx
**	jnb	.L5
**	cmpl	\$16, %edx
**	jnb	.L6
**	cmpl	\$8, %edx
**	jnb	.L7
**	cmpl	\$4, %edx
**	jnb	.L8
**	cmpl	\$1, %edx
**	ja	.L9
**	jb	.L11
**	movzbl	\(%rsi\), %eax
**	movb	%al, \(%rdi\)
**	ret
**	.p2align 4,,10
**	.p2align 3
**.L14:
**	movl	%edx, %edx
**	vmovdqu64	\(%rsi\), %zmm1
**	vmovdqu64	-64\(%rsi,%rdx\), %zmm0
**	vmovdqu64	%zmm1, \(%rdi\)
**	vmovdqu64	%zmm0, -64\(%rdi,%rdx\)
**	vzeroupper
**	ret
**	.p2align 4,,10
**	.p2align 3
**.L5:
**	movl	%edx, %edx
**	vmovdqu	\(%rsi\), %ymm1
**	vmovdqu	-32\(%rsi,%rdx\), %ymm0
**	vmovdqu	%ymm1, \(%rdi\)
**	vmovdqu	%ymm0, -32\(%rdi,%rdx\)
**	vzeroupper
**	ret
**	.p2align 4,,10
**	.p2align 3
**.L6:
**	movl	%edx, %edx
**	vmovdqu	\(%rsi\), %xmm1
**	vmovdqu	-16\(%rsi,%rdx\), %xmm0
**	vmovdqu	%xmm1, \(%rdi\)
**	vmovdqu	%xmm0, -16\(%rdi,%rdx\)
**	ret
**	.p2align 4,,10
**	.p2align 3
**.L7:
**	movl	%edx, %edx
**	movq	\(%rsi\), %rcx
**	movq	-8\(%rsi,%rdx\), %rax
**	movq	%rcx, \(%rdi\)
**	movq	%rax, -8\(%rdi,%rdx\)
**	ret
**	.p2align 4,,10
**	.p2align 3
**.L8:
**	movl	%edx, %edx
**	movl	\(%rsi\), %ecx
**	movl	-4\(%rsi,%rdx\), %eax
**	movl	%ecx, \(%rdi\)
**	movl	%eax, -4\(%rdi,%rdx\)
**	ret
**.L9:
**	movl	%edx, %edx
**	movzwl	\(%rsi\), %ecx
**	movzwl	-2\(%rsi,%rdx\), %eax
**	movw	%cx, \(%rdi\)
**	movw	%ax, -2\(%rdi,%rdx\)
**	ret
**	.cfi_endproc
**...
*/

#include "builtin-memcpy-5a.c"
