/* { dg-do compile { target { ! riscv_abi_e } } } */
/* { dg-require-effective-target rv64 } */
/* { dg-skip-if "" { *-*-* } { "-O0" "-Og" } } */
/* { dg-options "-march=rv64gc_xtheadcondmov -mtune=thead-c906 -mbranch-cost=1 -fdump-rtl-ce1" } */

typedef int __attribute__ ((mode (DI))) int_t;

int_t
movdifgt (double w, double x, int_t y, int_t z)
{
  return w > x ? y : z;
}

/* Expect branchless assembly like:

	fgt.d	a5,fa0,fa1
	th.mveqz	a0,a1,a5
 */

/* { dg-final { scan-rtl-dump-times "Conversion succeeded on pass 1\\." 1 "ce1" } } */
/* { dg-final { scan-rtl-dump-not "if-conversion succeeded through" "ce1" } } */
/* { dg-final { scan-assembler-times "\\s(?:fgt\\.d|flt\\.d)\\s" 1 } } */
/* { dg-final { scan-assembler-times "\\s(?:th\\.mveqz|th\\.mvnez)\\s" 1 } } */
/* { dg-final { scan-assembler-not "\\s(?:seqz|snez)\\s" } } */
/* { dg-final { scan-assembler-not "\\s(?:beq|bne)\\s" } } */
