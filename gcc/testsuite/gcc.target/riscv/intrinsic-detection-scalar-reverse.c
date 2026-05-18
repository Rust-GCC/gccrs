/* { dg-do preprocess } */
/* { dg-options "-march=rv64gc -mabi=lp64d" } */

#include <riscv_crypto.h>
#include <riscv_bitmanip.h>

#if !defined (__riscv_intrinsic_zkn) || !defined (__riscv_intrinsic_zks)
#error "missing scalar composite intrinsic detection macro"
#endif

#if __riscv_intrinsic_zkn != 1 || __riscv_intrinsic_zks != 1
#error "bad scalar composite intrinsic detection macro value"
#endif
