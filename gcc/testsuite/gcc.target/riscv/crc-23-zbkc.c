/* { dg-do run { target riscv_zbkc_ok } } */
/* { dg-options "-march=rv64gc_zbkc -fdump-tree-crc -fdump-rtl-dfinish " { target { rv64 } } } */
/* { dg-options "-march=rv32gc_zbkc -fdump-tree-crc -fdump-rtl-dfinish " { target { rv32 } } } */
/* { dg-skip-if "" { *-*-* } { "-O0" "-O1" "-Og" } } */

#include "../../gcc.dg/torture/crc-23.c"

/* { dg-final { scan-tree-dump "calculates CRC!" "crc"} } */
/* { dg-final { scan-tree-dump-times "Couldn't generate faster CRC code." 0 "crc"} } */
/* { dg-final { scan-rtl-dump "clmul" "dfinish"} } */
