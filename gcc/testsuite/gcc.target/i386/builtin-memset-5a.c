/* { dg-do compile } */
/* { dg-options "-O2 -march=x86-64 -minline-all-stringops" } */
/* Keep labels and directives ('.cfi_startproc', '.cfi_endproc').  */
/* { dg-final { check-function-bodies "**" "" "" { target lp64 } {^\t?\.} } } */

/*
**foo:
**.LFB0:
**	.cfi_startproc
**	cmpq	\$127, %rdx
**	jbe	.L12
**.L1:
**	ret
**	.p2align 4,,10
**	.p2align 3
**.L12:
**	movabsq	\$72340172838076673, %rcx
**	movzbl	%sil, %eax
**	imulq	%rcx, %rax
**	movq	%rax, %xmm0
**	punpcklqdq	%xmm0, %xmm0
**	cmpl	\$16, %edx
**	jnb	.L13
**	cmpl	\$8, %edx
**	jnb	.L6
**	cmpl	\$4, %edx
**	jnb	.L7
**	cmpl	\$1, %edx
**	ja	.L8
**	jb	.L1
**	movb	%sil, \(%rdi\)
**	ret
**	.p2align 4,,10
**	.p2align 3
**.L13:
**	movups	%xmm0, \(%rdi\)
**	cmpl	\$32, %edx
**	ja	.L5
**	movl	%edx, %edx
**	movups	%xmm0, -16\(%rdi,%rdx\)
**	ret
**	.p2align 4,,10
**	.p2align 3
**.L5:
**	movups	%xmm0, 16\(%rdi\)
**	cmpl	\$64, %edx
**	ja	.L14
**	movl	%edx, %edx
**	movups	%xmm0, -16\(%rdi,%rdx\)
**	movups	%xmm0, -32\(%rdi,%rdx\)
**	ret
**	.p2align 4,,10
**	.p2align 3
**.L6:
**	movl	%edx, %edx
**	movq	%rax, \(%rdi\)
**	movq	%rax, -8\(%rdi,%rdx\)
**	ret
**	.p2align 4,,10
**	.p2align 3
**.L14:
**	movl	%edx, %edx
**	movups	%xmm0, 32\(%rdi\)
**	movups	%xmm0, 48\(%rdi\)
**	movups	%xmm0, -16\(%rdi,%rdx\)
**	movups	%xmm0, -32\(%rdi,%rdx\)
**	movups	%xmm0, -48\(%rdi,%rdx\)
**	movups	%xmm0, -64\(%rdi,%rdx\)
**	ret
**	.p2align 4,,10
**	.p2align 3
**.L7:
**	movl	%edx, %edx
**	movl	%eax, \(%rdi\)
**	movl	%eax, -4\(%rdi,%rdx\)
**	ret
**	.p2align 4,,10
**	.p2align 3
**.L8:
**	movl	%edx, %edx
**	movw	%ax, \(%rdi\)
**	movw	%ax, -2\(%rdi,%rdx\)
**	ret
**	.cfi_endproc
**...
*/

void
foo (char *dest, int c, __SIZE_TYPE__ n)
{
  if (n < 128)
    __builtin_memset (dest, c, n);
}
