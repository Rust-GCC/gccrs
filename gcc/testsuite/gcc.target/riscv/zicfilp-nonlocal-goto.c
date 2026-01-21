/* Test RISC-V Zicfilp non-local goto LPAD with proper alignment.  */
/* { dg-do compile { target { riscv64*-*-* } } } */
/* { dg-options "-march=rv64gc_zicfilp -mabi=lp64d -fcf-protection=none -fcf-protection=branch" } */
/* { dg-skip-if "" { *-*-* } { "-O0" "-O2" "-O3" "-Os" "-Oz" "-Og" "-g" "-flto" } } */
/* { dg-final { check-function-bodies "**" "" {\.align|\.L[0-9]+:} } } */

extern void do_something (void);

/*
** outer_function:
**	lpad	\d+
**	...
**	\.align	2
**	\.L[0-9]+:
**	lpad	\d+
**	...
*/
void outer_function (void *label_addr)
{
  __label__ target_label;

  if (label_addr)
    goto *label_addr;

  do_something ();

target_label:
  do_something ();

  outer_function (&&target_label);
}
