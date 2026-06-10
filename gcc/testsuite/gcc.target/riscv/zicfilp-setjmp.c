/* Test RISC-V Zicfilp setjmp call protection functionality.  */
/* { dg-do compile { target { riscv64*-*-* } } } */
/* { dg-options "-O2 -march=rv64gc_zicfilp -mabi=lp64d -fcf-protection=none -fcf-protection=branch" } */
/* { dg-skip-if "" { *-*-* } { "-O0" "-g" "-flto" } } */
/* { dg-final { check-function-bodies "**" "" {\.p2align|\.option|call|lpad} } } */

#include <setjmp.h>

jmp_buf test_env;

/*
** test_setjmp_call:
**	...
**	.p2align	2
**	.option push
**	.option norelax
**	.option norvc
**	call	_?setjmp.*
**	.option pop
**	lpad	\d+
**	...
*/
int test_setjmp_call (void)
{
  return setjmp (test_env);
}
