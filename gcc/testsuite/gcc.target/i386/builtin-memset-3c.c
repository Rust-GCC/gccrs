/* { dg-do compile } */
/* { dg-options "-O2 -march=x86-64-v4 -minline-all-stringops" } */
/* Keep labels and directives ('.cfi_startproc', '.cfi_endproc').  */
/* { dg-final { check-function-bodies "**" "" "" { target lp64 } {^\t?\.} } } */

/*
**foo:
**...
*/

#include "builtin-memset-3a.c"
