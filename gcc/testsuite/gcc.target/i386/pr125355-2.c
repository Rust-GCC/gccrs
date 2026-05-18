/* { dg-do compile } */
/* { dg-options "-O2 -mtune=generic" } */
/* { dg-add-options check_function_bodies } */
/* Keep labels and directives ('.cfi_startproc', '.cfi_endproc').  */
/* { dg-final { check-function-bodies "**" "" "" { target "*-*-*" } {^\t?\.} } } */

/*
**foo.cold:
**...
**	call	memmove
**	call	abort
**	.cfi_endproc
**...
*/

#include <stddef.h>
#include <string.h>

void
foo (char *d, char *s, size_t n)
{
  if (n < 80)
    {
      memmove (d, s, n);
      __builtin_abort ();
    }
}
