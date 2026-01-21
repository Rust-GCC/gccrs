/* Test RISC-V Zicfilp gpr_save LPAD alignment with .option norelax.  */
/* { dg-do compile { target { riscv64*-*-* } } } */
/* { dg-options "-O2 -msave-restore -fomit-frame-pointer -march=rv64gc_zicfilp -mabi=lp64d -fcf-protection=none -fcf-protection=branch" } */
/* { dg-skip-if "" { *-*-* } { "-O0" "-g" "-flto" } } */
/* { dg-final { check-function-bodies "**" "" {\.p2align|\.option|call|lpad} } } */

extern void bar (void);

/*
** foo:
**	...
**	.p2align	2
**	.option push
**	.option norelax
**	.option norvc
**	call	t0,__riscv_save_[0-9]+
**	.option pop
**	lpad	\d+
**	...
*/
void foo (void)
{
  register long s0 asm ("s0");
  asm volatile ("li s0, 0x87654321" : "=r"(s0));
  bar ();
  asm volatile ("" : : "r"(s0));
}
