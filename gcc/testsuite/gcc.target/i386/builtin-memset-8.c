/* { dg-do compile { target { maybe_x32 && lp64 } } } */
/* { dg-options "-O2 -mx32 -march=x86-64 -minline-all-stringops" } */
/* Keep labels and directives ('.cfi_startproc', '.cfi_endproc').  */
/* { dg-final { check-function-bodies "**" "" "" { target lp64 } {^\t?\.} } } */

/*
**foo:
**.LFB0:
**	.cfi_startproc
**	movabsq	\$72340172838076673, %rax
**	movzbl	%sil, %esi
**	imulq	%rax, %rsi
**	movq	%rsi, %xmm0
**	punpcklqdq	%xmm0, %xmm0
**	cmpl	\$64, %edx
**	jnb	.L2
**	testb	\$32, %dl
**	jne	.L19
**	testb	\$16, %dl
**	jne	.L20
**	testb	\$8, %dl
**	jne	.L21
**	testb	\$4, %dl
**	jne	.L22
**	testl	%edx, %edx
**	jne	.L23
**.L1:
**	ret
**	.p2align 4,,10
**	.p2align 3
**.L2:
**	movups	%xmm0, -64\(%edx,%edi\)
**	subl	\$1, %edx
**	movups	%xmm0, -47\(%edx,%edi\)
**	movups	%xmm0, -31\(%edx,%edi\)
**	movups	%xmm0, -15\(%edx,%edi\)
**	cmpl	\$64, %edx
**	jb	.L1
**	andl	\$-64, %edx
**	xorl	%eax, %eax
**.L9:
**	movups	%xmm0, \(%eax,%edi\)
**	addl	\$64, %eax
**	movups	%xmm0, -48\(%eax,%edi\)
**	movups	%xmm0, -32\(%eax,%edi\)
**	movups	%xmm0, -16\(%eax,%edi\)
**	cmpl	%edx, %eax
**	jb	.L9
**	ret
**	.p2align 4,,10
**	.p2align 3
**.L23:
**	movb	%sil, \(%edi\)
**	testb	\$2, %dl
**	je	.L1
**	movw	%si, -2\(%edx,%edi\)
**	ret
**	.p2align 4,,10
**	.p2align 3
**.L19:
**	movups	%xmm0, \(%edi\)
**	movups	%xmm0, 16\(%edi\)
**	movups	%xmm0, -32\(%edx,%edi\)
**	movups	%xmm0, -16\(%edx,%edi\)
**	ret
**	.p2align 4,,10
**	.p2align 3
**.L20:
**	movups	%xmm0, \(%edi\)
**	movups	%xmm0, -16\(%edx,%edi\)
**	ret
**	.p2align 4,,10
**	.p2align 3
**.L21:
**	movq	%rsi, \(%edi\)
**	movq	%rsi, -8\(%edx,%edi\)
**	ret
**	.p2align 4,,10
**	.p2align 3
**.L22:
**	movl	%esi, \(%edi\)
**	movl	%esi, -4\(%edx,%edi\)
**	ret
**	.cfi_endproc
**...
*/

void
foo (char *dst, int c, __SIZE_TYPE__ n)
{
  if (n <= (__SIZE_TYPE__) -1)
    __builtin_memset (dst, c, n);
}
