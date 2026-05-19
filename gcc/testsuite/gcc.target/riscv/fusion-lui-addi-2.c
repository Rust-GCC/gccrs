/* Verify LUI_ADDI fusion still fires after table-driven refactor
   with a different constant than fusion-lui-addi.c.  */
/* { dg-do compile } */
/* { dg-skip-if "" { *-*-* } { "-O0" "-O1" "-Og" "-Os" "-Oz" "-flto" } } */
/* { dg-options "-march=rv64gc -mabi=lp64d -mtune=sifive-p600-series -fdump-rtl-sched1" } */
/* { dg-final { scan-rtl-dump "RISCV_FUSE_LUI_ADDI" "sched1" } } */

long
bar (void)
{
  return 0x7BCD1234L;
}
