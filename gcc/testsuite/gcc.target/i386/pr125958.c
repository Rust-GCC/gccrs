/* { dg-do compile { target fpic } } */
/* { dg-options "-O2 -fno-pic -march=x86-64" } */
/* Keep labels and directives ('.cfi_startproc', '.cfi_endproc').  */
/* { dg-final { check-function-bodies "**" "" "" { target *-*-* } {^\t?\.} } } */

/*
**vcn_init_session_buf_fc:
**.LFB0:
**...
**	pxor	%xmm0, %xmm0
**	xorl	%[a-z0-9]+, %[a-z0-9]+
**	movw	%[a-z0-9]+, 16\(%[a-z0-9]+\)
**	movups	%xmm0, \(%[a-z0-9]+\)
**...
*/

const short ar[9] = {};
void
vcn_init_session_buf_fc (void * r)
{
  __builtin_memcpy (r, ar,sizeof(ar));
}
