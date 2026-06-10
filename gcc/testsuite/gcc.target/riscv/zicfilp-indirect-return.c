/* Test RISC-V Zicfilp indirect_return attribute functionality.  */
/* { dg-do compile { target { riscv64*-*-* } } } */
/* { dg-options "-O2 -march=rv64gc_zicfilp -mabi=lp64d -fcf-protection=none -fcf-protection=branch -fno-inline" } */
/* { dg-skip-if "" { *-*-* } { "-O0" "-g" "-flto" } } */
/* { dg-final { check-function-bodies "**" "" {\.p2align|\.option|call|lpad} } } */

/* Function marked with indirect_return attribute.  */
int __attribute__((indirect_return, noinline)) indirect_func (int x)
{
  return x * 2 + 1;
}

/*
** test_indirect_call:
**	...
**	.p2align	2
**	.option push
**	.option norelax
**	.option norvc
**	call	indirect_func.*
**	.option pop
**	lpad	\d+
**	...
*/
int test_indirect_call (int x)
{
  /* Use the result to prevent tail call optimization.  */
  int result = indirect_func (x);
  return result + 1;
}
