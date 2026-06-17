/* { dg-do compile } */
/* { dg-options "-O2 -march=x86-64-v3 -minline-all-stringops" } */
/* Keep labels and directives ('.cfi_startproc', '.cfi_endproc').  */
/* { dg-final { check-function-bodies "**" "" "" { target lp64 } {^\t?\.} } } */

/*
**foo:
**.LFB0:
**	.cfi_startproc
**	cmpq	\$247, %rsi
**	jbe	.L15
**	ret
**	.p2align 4,,10
**	.p2align 3
**.L15:
**	vpcmpeqd	%ymm0, %ymm0, %ymm0
**	cmpl	\$32, %esi
**	jnb	.L16
**	cmpl	\$16, %esi
**	jnb	.L6
**	cmpl	\$8, %esi
**	jnb	.L7
**	cmpl	\$4, %esi
**	jnb	.L8
**	cmpl	\$1, %esi
**	ja	.L9
**	jb	.L12
**	movb	\$-1, \(%rdi\)
**	vzeroupper
**	ret
**	.p2align 4,,10
**	.p2align 3
**.L16:
**	vmovdqu	%ymm0, \(%rdi\)
**	cmpl	\$64, %esi
**	ja	.L5
**	movl	%esi, %esi
**	vmovdqu	%ymm0, -32\(%rdi,%rsi\)
**	vzeroupper
**	ret
**	.p2align 4,,10
**	.p2align 3
**.L5:
**	vmovdqu	%ymm0, 32\(%rdi\)
**	cmpl	\$128, %esi
**	ja	.L17
**	movl	%esi, %esi
**	vmovdqu	%ymm0, -32\(%rdi,%rsi\)
**	vmovdqu	%ymm0, -64\(%rdi,%rsi\)
**	vzeroupper
**	ret
**	.p2align 4,,10
**	.p2align 3
**.L6:
**	movl	%esi, %esi
**	vmovdqu	%xmm0, \(%rdi\)
**	vmovdqu	%xmm0, -16\(%rdi,%rsi\)
**	vzeroupper
**	ret
**	.p2align 4,,10
**	.p2align 3
**.L17:
**	movl	%esi, %esi
**	vmovdqu	%ymm0, 64\(%rdi\)
**	vmovdqu	%ymm0, 96\(%rdi\)
**	vmovdqu	%ymm0, -32\(%rdi,%rsi\)
**	vmovdqu	%ymm0, -64\(%rdi,%rsi\)
**	vmovdqu	%ymm0, -96\(%rdi,%rsi\)
**	vmovdqu	%ymm0, -128\(%rdi,%rsi\)
**	vzeroupper
**	ret
**	.p2align 4,,10
**	.p2align 3
**.L7:
**	movl	%esi, %esi
**	movq	\$-1, \(%rdi\)
**	movq	\$-1, -8\(%rdi,%rsi\)
**	vzeroupper
**	ret
**	.p2align 4,,10
**	.p2align 3
**.L8:
**	movl	%esi, %esi
**	movl	\$-1, \(%rdi\)
**	movl	\$-1, -4\(%rdi,%rsi\)
**	vzeroupper
**	ret
**	.p2align 4,,10
**	.p2align 3
**.L9:
**	movl	\$-1, %eax
**	movl	%esi, %esi
**	movw	%ax, \(%rdi\)
**	movw	%ax, -2\(%rdi,%rsi\)
**	vzeroupper
**	ret
**.L12:
**	vzeroupper
**	ret
**...
*/

#include "builtin-memset-7a.c"
