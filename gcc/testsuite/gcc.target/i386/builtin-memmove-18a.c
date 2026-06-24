/* { dg-do compile } */
/* { dg-options "-O2 -mno-avx -msse2 -mtune=generic" } */
/* Keep labels and directives ('.cfi_startproc', '.cfi_endproc').  */
/* { dg-final { check-function-bodies "**" "" "" { target { lp64 } } {^\t?\.} } } */

/*
**gcc_memmove:
**.LFB0:
**	.cfi_startproc
**	cmpq	\$1, %rdx
**	je	.L4
**	ret
**	.p2align 4,,10
**	.p2align 3
**.L4:
**	movzbl	\(%rsi\), %eax
**	movb	%al, \(%rdi\)
**	ret
**	.cfi_endproc
**...
*/

void
gcc_memmove (void *a, void *b, __SIZE_TYPE__ n)
{
  if (n >= 1 && n <= 1)
    __builtin_memmove (a, b, n);
}
