/* { dg-do compile { target { { ! ia32 } && fpic } } } */
/* { dg-options "-O2" } */
/* Keep labels and directives ('.cfi_startproc', '.cfi_endproc').  */
/* { dg-final { check-function-bodies "**" "" "" { target "*-*-*" } {^\t?\.} } } */

/*
**foo:
**.LFB[0-9]+:
**	.cfi_startproc
**	movabsq	\$4758053007424749568, %rax
**	movq	%rax, \(%[re]di\)
**	ret
**	.cfi_endproc
**...
*/

typedef float v2sf __attribute__((vector_size(8)));

void
foo (v2sf *c)
{
  *c =  __extension__(v2sf){34, 34};
}
