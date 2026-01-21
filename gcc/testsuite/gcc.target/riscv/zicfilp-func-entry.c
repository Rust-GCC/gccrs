/* Test RISC-V Zicfilp function entry LPAD.  */
/* { dg-do compile { target { riscv64*-*-* } } } */
/* { dg-options "-O2 -march=rv64gc_zicfilp -mabi=lp64d -fcf-protection=none -fcf-protection=branch" } */
/* { dg-skip-if "" { *-*-* } { "-O0" "-g" "-flto" } } */
/* { dg-final { check-function-bodies "**" "" } } */

/*
** public_function:
**	lpad	\d+
**	...
*/
int public_function (int a, int b)
{
  return a + b;
}
