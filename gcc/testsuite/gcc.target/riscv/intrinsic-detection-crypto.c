/* { dg-do preprocess } */
/* { dg-options "-march=rv64gc -mabi=lp64d" } */

#include <riscv_crypto.h>

#if defined (__riscv_zkne) || defined (__riscv_zknd) \
    || defined (__riscv_zknh) || defined (__riscv_zksed) \
    || defined (__riscv_zksh)
#error "unexpected scalar crypto ISA extension macro"
#endif

#if !defined (__riscv_intrinsic_zknd) \
    || !defined (__riscv_intrinsic_zkne) \
    || !defined (__riscv_intrinsic_zknh) \
    || !defined (__riscv_intrinsic_zksed) \
    || !defined (__riscv_intrinsic_zksh)
#error "missing scalar crypto intrinsic detection macro"
#endif

#if defined (__riscv_intrinsic_zkn) || defined (__riscv_intrinsic_zks)
#error "unexpected scalar crypto composite intrinsic detection macro"
#endif

#if __riscv_intrinsic_zknd != 1 \
    || __riscv_intrinsic_zkne != 1 \
    || __riscv_intrinsic_zknh != 1 \
    || __riscv_intrinsic_zksed != 1 \
    || __riscv_intrinsic_zksh != 1
#error "bad scalar crypto intrinsic detection macro value"
#endif
