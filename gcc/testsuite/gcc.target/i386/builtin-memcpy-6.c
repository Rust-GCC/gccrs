/* { dg-do compile { target { maybe_x32 && lp64 } } } */
/* { dg-options "-O2 -mx32 -march=x86-64 -minline-all-stringops" } */
/* Keep labels and directives ('.cfi_startproc', '.cfi_endproc').  */
/* { dg-final { check-function-bodies "**" "" "" { target lp64 } {^\t?\.} } } */


/*
**gcc_memcpy:
**.LFB0:
**	.cfi_startproc
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
**	movdqu	-64\(%edx,%esi\), %xmm0
**	subl	\$1, %edx
**	movups	%xmm0, -63\(%edx,%edi\)
**	movdqu	-47\(%edx,%esi\), %xmm0
**	movups	%xmm0, -47\(%edx,%edi\)
**	movdqu	-31\(%edx,%esi\), %xmm0
**	movups	%xmm0, -31\(%edx,%edi\)
**	movdqu	-15\(%edx,%esi\), %xmm0
**	movups	%xmm0, -15\(%edx,%edi\)
**	cmpl	\$64, %edx
**	jb	.L1
**	andl	\$-64, %edx
**	xorl	%eax, %eax
**.L9:
**	movdqu	\(%eax,%esi\), %xmm3
**	movdqu	16\(%eax,%esi\), %xmm2
**	addl	\$64, %eax
**	movdqu	-32\(%eax,%esi\), %xmm1
**	movdqu	-16\(%eax,%esi\), %xmm0
**	movups	%xmm3, -64\(%eax,%edi\)
**	movups	%xmm2, -48\(%eax,%edi\)
**	movups	%xmm1, -32\(%eax,%edi\)
**	movups	%xmm0, -16\(%eax,%edi\)
**	cmpl	%edx, %eax
**	jb	.L9
**	ret
**	.p2align 4,,10
**	.p2align 3
**.L23:
**	movzbl	\(%esi\), %eax
**	movb	%al, \(%edi\)
**	testb	\$2, %dl
**	je	.L1
**	movzwl	-2\(%edx,%esi\), %eax
**	movw	%ax, -2\(%edx,%edi\)
**	ret
**	.p2align 4,,10
**	.p2align 3
**.L19:
**	movdqu	\(%esi\), %xmm0
**	movups	%xmm0, \(%edi\)
**	movdqu	16\(%esi\), %xmm0
**	movups	%xmm0, 16\(%edi\)
**	movdqu	-32\(%edx,%esi\), %xmm0
**	movups	%xmm0, -32\(%edx,%edi\)
**	movdqu	-16\(%edx,%esi\), %xmm0
**	movups	%xmm0, -16\(%edx,%edi\)
**	ret
**	.p2align 4,,10
**	.p2align 3
**.L20:
**	movdqu	\(%esi\), %xmm0
**	movups	%xmm0, \(%edi\)
**	movdqu	-16\(%edx,%esi\), %xmm0
**	movups	%xmm0, -16\(%edx,%edi\)
**	ret
**	.p2align 4,,10
**	.p2align 3
**.L21:
**	movq	\(%esi\), %rax
**	movq	%rax, \(%edi\)
**	movq	-8\(%edx,%esi\), %rax
**	movq	%rax, -8\(%edx,%edi\)
**	ret
**	.p2align 4,,10
**	.p2align 3
**.L22:
**	movl	\(%esi\), %eax
**	movl	%eax, \(%edi\)
**	movl	-4\(%edx,%esi\), %eax
**	movl	%eax, -4\(%edx,%edi\)
**	ret
**	.cfi_endproc
**...
*/

void
gcc_memcpy (void *a, void *b, __SIZE_TYPE__ n)
{
  if (n <= (__SIZE_TYPE__) -1)
    __builtin_memcpy (a, b, n);
}
