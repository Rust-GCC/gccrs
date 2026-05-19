/* Verify RISCV_FUSE_CACHE_ALIGNED_STD correctly matches consecutive
   stores to the same cache line.  This exercises the fix for base
   register comparison (changed from != to ==).  */
/* { dg-do compile } */
/* { dg-skip-if "" { *-*-* } { "-O0" "-O1" "-Og" "-Os" "-Oz" "-flto" } } */
/* { dg-options "-march=rv64gc -mabi=lp64d -mtune=sifive-p600-series -fdump-rtl-sched1" } */
/* No upstream mtune currently enables RISCV_FUSE_CACHE_ALIGNED_STD.  */
/* { dg-final { scan-rtl-dump "RISCV_FUSE_CACHE_ALIGNED_STD" "sched1" { xfail *-*-* } } } */

struct pair
{
  long a;
  long b;
} __attribute__ ((aligned (16)));

void
store_pair (struct pair *p, long x, long y)
{
  p->a = x;
  p->b = y;
}
