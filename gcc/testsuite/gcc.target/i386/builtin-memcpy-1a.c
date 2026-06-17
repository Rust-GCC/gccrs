/* { dg-do compile } */
/* { dg-options "-O2 -march=x86-64 -minline-all-stringops" } */
/* Keep labels and directives ('.cfi_startproc', '.cfi_endproc').  */
/* { dg-final { check-function-bodies "**" "" "" { target lp64 } {^\t?\.} } } */

/*
**foo:
**.LFB0:
**	.cfi_startproc
**	movdqu	\(%rsi\), %xmm0
**	movups	%xmm0, \(%rdi\)
**	movdqu	16\(%rsi\), %xmm0
**	movups	%xmm0, 16\(%rdi\)
**	movdqu	32\(%rsi\), %xmm0
**	movups	%xmm0, 32\(%rdi\)
**	movdqu	48\(%rsi\), %xmm0
**	movups	%xmm0, 48\(%rdi\)
**	movdqu	64\(%rsi\), %xmm0
**	movups	%xmm0, 64\(%rdi\)
**	movdqu	80\(%rsi\), %xmm0
**	movups	%xmm0, 80\(%rdi\)
**	movdqu	96\(%rsi\), %xmm0
**	movups	%xmm0, 96\(%rdi\)
**	movdqu	112\(%rsi\), %xmm0
**	movups	%xmm0, 112\(%rdi\)
**	movzbl	128\(%rsi\), %eax
**	movb	%al, 128\(%rdi\)
**	ret
**	.cfi_endproc
**...
*/

void
foo (char *dest, const char *src)
{
  __builtin_memcpy (dest, src, 129);
}
