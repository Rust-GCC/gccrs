/* { dg-do compile } */
/* { dg-options "-O2 -march=x86-64 -minline-all-stringops" } */
/* Keep labels and directives ('.cfi_startproc', '.cfi_endproc').  */
/* { dg-final { check-function-bodies "**" "" "" { target lp64 } {^\t?\.} } } */

/*
**foo:
**.LFB0:
**	.cfi_startproc
**	cmpq	\$247, %rsi
**	jbe	.L16
**.L1:
**	ret
**	.p2align 4,,10
**	.p2align 3
**.L16:
**	movq	%rdi, %rax
**	cmpl	\$16, %esi
**	jnb	.L17
**	cmpl	\$8, %esi
**	jnb	.L6
**	cmpl	\$4, %esi
**	jnb	.L7
**	cmpl	\$1, %esi
**	ja	.L8
**	jb	.L1
**	movb	\$-1, \(%rdi\)
**	ret
**	.p2align 4,,10
**	.p2align 3
**.L17:
**	pcmpeqd	%xmm0, %xmm0
**	cmpl	\$32, %esi
**	ja	.L5
**	movl	%esi, %esi
**	movups	%xmm0, \(%rdi\)
**	movups	%xmm0, -16\(%rdi,%rsi\)
**	ret
**	.p2align 4,,10
**	.p2align 3
**.L5:
**	cmpl	\$128, %esi
**	ja	.L10
**	movups	%xmm0, \(%rdi\)
**	movups	%xmm0, 16\(%rdi\)
**	cmpl	\$64, %esi
**	jbe	.L11
**	movups	%xmm0, 32\(%rdi\)
**	movups	%xmm0, 48\(%rdi\)
**.L14:
**	movl	%esi, %esi
**	movups	%xmm0, -16\(%rdi,%rsi\)
**	movups	%xmm0, -32\(%rdi,%rsi\)
**	movups	%xmm0, -48\(%rdi,%rsi\)
**	movups	%xmm0, -64\(%rdi,%rsi\)
**	ret
**	.p2align 4,,10
**	.p2align 3
**.L6:
**	movl	%esi, %esi
**	movq	\$-1, \(%rdi\)
**	movq	\$-1, -8\(%rdi,%rsi\)
**	ret
**	.p2align 4,,10
**	.p2align 3
**.L10:
**	movl	%esi, %edx
**.L12:
**	subl	\$64, %edx
**	movups	%xmm0, \(%rax\)
**	addq	\$64, %rax
**	movups	%xmm0, -48\(%rax\)
**	movups	%xmm0, -32\(%rax\)
**	movups	%xmm0, -16\(%rax\)
**	cmpl	\$64, %edx
**	ja	.L12
**	jmp	.L14
**	.p2align 4,,10
**	.p2align 3
**.L7:
**	movl	%esi, %esi
**	movl	\$-1, \(%rdi\)
**	movl	\$-1, -4\(%rdi,%rsi\)
**	ret
**	.p2align 4,,10
**	.p2align 3
**.L11:
**	movl	%esi, %esi
**	movups	%xmm0, -16\(%rdi,%rsi\)
**	movups	%xmm0, -32\(%rdi,%rsi\)
**	ret
**	.p2align 4,,10
**	.p2align 3
**.L8:
**	movl	\$-1, %eax
**	movl	%esi, %esi
**	movw	%ax, \(%rdi\)
**	movw	%ax, -2\(%rdi,%rsi\)
**	ret
**...
*/

void
foo (char *dst, __SIZE_TYPE__ n)
{
  if (n <= 247)
    __builtin_memset(dst, -1, n);
}
