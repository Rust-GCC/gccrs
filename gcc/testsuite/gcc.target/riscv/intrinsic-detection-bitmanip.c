/* { dg-do preprocess } */
/* { dg-options "-march=rv64gc -mabi=lp64d" } */

#include <riscv_bitmanip.h>

#if defined (__riscv_zbb) || defined (__riscv_zbc) \
    || defined (__riscv_zbkb) || defined (__riscv_zbkc) \
    || defined (__riscv_zbkx)
#error "unexpected scalar bitmanip ISA extension macro"
#endif

#if !defined (__riscv_intrinsic_zbb) \
    || !defined (__riscv_intrinsic_zbc) \
    || !defined (__riscv_intrinsic_zbkb) \
    || !defined (__riscv_intrinsic_zbkc) \
    || !defined (__riscv_intrinsic_zbkx)
#error "missing scalar bitmanip intrinsic detection macro"
#endif

#if defined (__riscv_intrinsic_zkn) || defined (__riscv_intrinsic_zks)
#error "unexpected scalar crypto composite intrinsic detection macro"
#endif

#if __riscv_intrinsic_zbb != 1 \
    || __riscv_intrinsic_zbc != 1 \
    || __riscv_intrinsic_zbkb != 1 \
    || __riscv_intrinsic_zbkc != 1 \
    || __riscv_intrinsic_zbkx != 1
#error "bad scalar bitmanip intrinsic detection macro value"
#endif
