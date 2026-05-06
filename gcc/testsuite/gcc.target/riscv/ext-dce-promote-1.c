/* { dg-do compile } */
/* { dg-options "-O2 -march=rv64gc_zba_zbb -mabi=lp64d -fdump-rtl-ext_dce" } */
/* { dg-skip-if "" { *-*-* } { "-flto" } } */

/* Verify that ext-dce promotes SI-mode operations to DI-mode when the
   sign-extended bits are dead, enabling combine to match sh2add.  */

unsigned short core_bench (int found, int missed, unsigned short retval)
{
  retval += found * 4 - missed;
  return retval;
}

/* { dg-final { scan-rtl-dump "Successfully promoted to:" "ext_dce" } } */
/* { dg-final { scan-assembler "sh2add\t" } } */
/* { dg-final { scan-assembler "zext\\.h\t" } } */
/* { dg-final { scan-assembler-not "slliw\t" } } */
