/* Verify RISCV_FUSE_LDINDEXED correctly matches add+ld pairs.
   This exercises the removal of the duplicate REG_P check.  */
/* { dg-do compile } */
/* { dg-skip-if "" { *-*-* } { "-O0" "-O1" "-Og" "-Os" "-Oz" "-flto" } } */
/* { dg-options "-march=rv64gc -mabi=lp64d -mtune=sifive-p600-series -fdump-rtl-sched1" } */
/* No upstream mtune currently enables RISCV_FUSE_LDINDEXED.  */
/* { dg-final { scan-rtl-dump "RISCV_FUSE_LDINDEXED" "sched1" { xfail *-*-* } } } */

long
load_indexed (long *base, long idx)
{
  return base[idx];
}
