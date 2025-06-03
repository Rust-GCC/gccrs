/* { dg-do compile { target { ! riscv_abi_e } } } */
/* { dg-require-effective-target rv64 } */
/* { dg-skip-if "" { *-*-* } { "-O0" "-Og" "-Os" "-Oz" } } */
/* { dg-options "-march=rv64gc -mtune=sifive-5-series -mbranch-cost=3 -mmovcc -ffinite-math-only -fdump-rtl-ce1" } */

typedef int __attribute__ ((mode (DI))) int_t;

int_t
adddiflt (double w, double x, int_t y, int_t z)
{
  return w < x ? y + z : y;
}

/* Expect branchless assembly like:

	flt.d	a5,fa0,fa1
	neg	a5,a5
	and	a5,a5,a1
	add	a0,a5,a0
 */

/* { dg-final { scan-rtl-dump-times "Conversion succeeded on pass 1\\." 1 "ce1" } } */
/* { dg-final { scan-rtl-dump-times "if-conversion succeeded through noce_try_addcc" 1 "ce1" } } */
/* { dg-final { scan-assembler-times "\\s(?:fge\\.d|fgt\\.d|fle\\.d|flt\\.d)\\s" 1 } } */
/* { dg-final { scan-assembler-not "\\s(?:seqz|snez)\\s" } } */
/* { dg-final { scan-assembler-not "\\s(?:beq|bne)\\s" } } */
