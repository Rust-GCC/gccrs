/* { dg-do compile { target { ! riscv_abi_e } } } */
/* { dg-require-effective-target rv64 } */
/* { dg-skip-if "" { *-*-* } { "-O0" "-Og" "-Os" "-Oz" } } */
/* { dg-options "-march=rv64gc -mtune=sifive-5-series -mbranch-cost=6 -mmovcc -fdump-rtl-ce1" } */

typedef int __attribute__ ((mode (DI))) int_t;

int_t
movdigt (int_t w, int_t x, int_t y, int_t z)
{
  return w > x ? y : z;
}

/* Expect branchless assembly like:

	sgt	a1,a0,a1
	neg	a1,a1
	and	a3,a1,a3
	not	a1,a1
	and	a1,a1,a2
	or	a0,a3,a1
 */

/* { dg-final { scan-rtl-dump-times "Conversion succeeded on pass 1\\." 1 "ce1" } } */
/* { dg-final { scan-rtl-dump-times "if-conversion succeeded through noce_try_cmove" 1 "ce1" } } */
/* { dg-final { scan-assembler-times "\\s(?:sgt|slt)\\s" 1 } } */
/* { dg-final { scan-assembler-not "\\s(?:seqz|snez)\\s" } } */
/* { dg-final { scan-assembler-not "\\s(?:bge|bgt|ble|blt)\\s" } } */
