/* { dg-do compile { target { ! riscv_abi_e } } } */
/* { dg-require-effective-target rv64 } */
/* { dg-skip-if "" { *-*-* } { "-O0" "-Og" } } */
/* { dg-options "-march=rv64gc -mtune=sifive-7-series -mbranch-cost=1 -fdump-rtl-ce1" } */

typedef int __attribute__ ((mode (DI))) int_t;

int_t
movdifne (double w, double x, int_t y, int_t z)
{
  return w != x ? y : z;
}

/* Expect short forward branch assembly like:

	feq.d	a5,fa0,fa1
	bne	a5,zero,1f	# movcc
	mv	a1,a0
1:
	mv	a0,a1
 */

/* { dg-final { scan-rtl-dump-times "Conversion succeeded on pass 1\\." 1 "ce1" } } */
/* { dg-final { scan-rtl-dump-times "if-conversion succeeded through noce_try_cmove" 1 "ce1" } } */
/* { dg-final { scan-assembler-times "\\sfeq\\.d\\s" 1 } } */
/* { dg-final { scan-assembler-times "\\s(?:beq|bne)\\s\[^\\s\]+\\s# movcc\\s" 1 } } */
/* { dg-final { scan-assembler-not "\\s(?:seqz|snez)\\s" } } */
