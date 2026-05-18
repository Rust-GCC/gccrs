/* { dg-do preprocess } */
/* { dg-options "-march=rv64gc -mabi=lp64d" } */

#include <riscv_bitmanip.h>
#include <riscv_crypto.h>

#if defined (__riscv_zbb) || defined (__riscv_zbc) \
    || defined (__riscv_zbkb) || defined (__riscv_zbkc) \
    || defined (__riscv_zbkx) || defined (__riscv_zkne) \
    || defined (__riscv_zknd) || defined (__riscv_zknh) \
    || defined (__riscv_zkn) || defined (__riscv_zks) \
    || defined (__riscv_zksed) || defined (__riscv_zksh)
#error "unexpected scalar ISA extension macro"
#endif

#if !defined (__riscv_intrinsic_zbb) \
    || !defined (__riscv_intrinsic_zbc) \
    || !defined (__riscv_intrinsic_zbkb) \
    || !defined (__riscv_intrinsic_zbkc) \
    || !defined (__riscv_intrinsic_zbkx) \
    || !defined (__riscv_intrinsic_zkn) \
    || !defined (__riscv_intrinsic_zknd) \
    || !defined (__riscv_intrinsic_zkne) \
    || !defined (__riscv_intrinsic_zknh) \
    || !defined (__riscv_intrinsic_zks) \
    || !defined (__riscv_intrinsic_zksed) \
    || !defined (__riscv_intrinsic_zksh)
#error "missing scalar intrinsic detection macro"
#endif

#if __riscv_intrinsic_zbb != 1 \
    || __riscv_intrinsic_zbc != 1 \
    || __riscv_intrinsic_zbkb != 1 \
    || __riscv_intrinsic_zbkc != 1 \
    || __riscv_intrinsic_zbkx != 1 \
    || __riscv_intrinsic_zkn != 1 \
    || __riscv_intrinsic_zknd != 1 \
    || __riscv_intrinsic_zkne != 1 \
    || __riscv_intrinsic_zknh != 1 \
    || __riscv_intrinsic_zks != 1 \
    || __riscv_intrinsic_zksed != 1 \
    || __riscv_intrinsic_zksh != 1
#error "bad scalar intrinsic detection macro value"
#endif
