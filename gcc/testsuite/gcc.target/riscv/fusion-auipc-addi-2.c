/* Verify AUIPC_ADDI fusion still fires after table-driven refactor.  */
/* { dg-do compile } */
/* { dg-skip-if "" { *-*-* } { "-O0" "-O1" "-Og" "-Os" "-Oz" "-flto" } } */
/* { dg-options "-march=rv64gc -mabi=lp64d -mtune=sifive-p600-series -mcmodel=medany -mexplicit-relocs -fno-section-anchors -fdump-rtl-sched1" } */
/* { dg-final { scan-rtl-dump "RISCV_FUSE_AUIPC_ADDI" "sched1" } } */

extern int arr[];

int *
get_arr_addr (void)
{
  return arr;
}
