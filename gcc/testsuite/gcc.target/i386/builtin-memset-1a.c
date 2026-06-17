/* { dg-do compile } */
/* { dg-options "-O2 -march=x86-64 -minline-all-stringops" } */
/* Keep labels and directives ('.cfi_startproc', '.cfi_endproc').  */
/* { dg-final { check-function-bodies "**" "" "" { target lp64 } {^\t?\.} } } */

/*
**foo:
**.LFB0:
**	.cfi_startproc
**	movd	%esi, %xmm0
**	movb	%sil, 128\(%rdi\)
**	punpcklbw	%xmm0, %xmm0
**	punpcklwd	%xmm0, %xmm0
**	pshufd	\$0, %xmm0, %xmm0
**	movups	%xmm0, \(%rdi\)
**	movups	%xmm0, 16\(%rdi\)
**	movups	%xmm0, 32\(%rdi\)
**	movups	%xmm0, 48\(%rdi\)
**	movups	%xmm0, 64\(%rdi\)
**	movups	%xmm0, 80\(%rdi\)
**	movups	%xmm0, 96\(%rdi\)
**	movups	%xmm0, 112\(%rdi\)
**	ret
**	.cfi_endproc
**...
*/

void
foo (char *dest, int c)
{
  __builtin_memset (dest, c, 129);
}
