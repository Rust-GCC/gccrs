/* { dg-do compile } */
/* { dg-options "-O2 -fno-schedule-insns2" } */
/* { dg-final { check-function-bodies "**" "" "" } } */
/* { dg-skip-if "" { *-*-mingw* } } */

#include <stdarg.h>
int foo [[gnu::preserve_none]] (...);

/* Check preserve_none falls back to AAPCS for variadic functions. */

/*
** bar:
** ...
**	mov	w0, 23
**	mov	w7, 7
**	mov	w6, 6
**	mov	w5, 5
**	mov	w4, 4
**	mov	w3, 3
**	mov	w2, 2
**	mov	w1, 1
**	str	w0, \[sp, 120\]
**	mov	w0, 22
**	str	w0, \[sp, 112\]
**	mov	w0, 21
**	str	w0, \[sp, 104\]
**	mov	w0, 20
**	str	w0, \[sp, 96\]
**	mov	w0, 19
**	str	w0, \[sp, 88\]
**	mov	w0, 18
**	str	w0, \[sp, 80\]
**	mov	w0, 17
**	str	w0, \[sp, 72\]
**	mov	w0, 16
**	str	w0, \[sp, 64\]
**	mov	w0, 15
**	str	w0, \[sp, 56\]
**	mov	w0, 14
**	str	w0, \[sp, 48\]
**	mov	w0, 13
**	str	w0, \[sp, 40\]
**	mov	w0, 12
**	str	w0, \[sp, 32\]
**	mov	w0, 11
**	str	w0, \[sp, 24\]
**	mov	w0, 10
**	str	w0, \[sp, 16\]
**	mov	w0, 9
**	str	w0, \[sp, 8\]
**	mov	w0, 8
**	str	w0, \[sp\]
**	mov	w0, 0
**	bl	foo
**	add	w0, w0, 1
** ...
*/
int bar [[gnu::preserve_none]] ()
{
  return foo (0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18,
	      19, 20, 21, 22, 23)
	 + 1;
}
