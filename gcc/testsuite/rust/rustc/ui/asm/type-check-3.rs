// only-x86_64
// compile-flags: -C target-feature=+avx512f

#![feature(asm)]

use std::arch::x86_64::{_mm256_setzero_ps, _mm_setzero_ps};

fn main() {
    unsafe {
        // Types must be listed in the register class.

        asm!("{}", in(reg) 0i128);
// { dg-error "" "" { target *-*-* } .-1 }
        asm!("{}", in(reg) _mm_setzero_ps());
// { dg-error "" "" { target *-*-* } .-1 }
        asm!("{}", in(reg) _mm256_setzero_ps());
// { dg-error "" "" { target *-*-* } .-1 }
        asm!("{}", in(xmm_reg) 0u8);
// { dg-error "" "" { target *-*-* } .-1 }
        asm!("{:e}", in(reg) 0i32);
        asm!("{}", in(xmm_reg) 0i32);
        asm!("{:e}", in(reg) 0f32);
        asm!("{}", in(xmm_reg) 0f32);
        asm!("{}", in(xmm_reg) _mm_setzero_ps());
        asm!("{:x}", in(ymm_reg) _mm_setzero_ps());
        asm!("{}", in(kreg) 0u16);
        asm!("{}", in(kreg) 0u64);
// { dg-error "" "" { target *-*-* } .-1 }

        // Template modifier suggestions for sub-registers

        asm!("{0} {0}", in(reg) 0i16);
// { dg-warning "" "" { target *-*-* } .-1 }
        asm!("{0} {0:x}", in(reg) 0i16);
// { dg-warning "" "" { target *-*-* } .-1 }
        asm!("{}", in(reg) 0i32);
// { dg-warning "" "" { target *-*-* } .-1 }
        asm!("{}", in(reg) 0i64);
        asm!("{}", in(ymm_reg) 0i64);
// { dg-warning "" "" { target *-*-* } .-1 }
        asm!("{}", in(ymm_reg) _mm256_setzero_ps());
        asm!("{:l}", in(reg) 0i16);
        asm!("{:l}", in(reg) 0i32);
        asm!("{:l}", in(reg) 0i64);
        asm!("{:x}", in(ymm_reg) 0i64);
        asm!("{:x}", in(ymm_reg) _mm256_setzero_ps());

        // Suggest different register class for type

        asm!("{}", in(reg) 0i8);
// { dg-error "" "" { target *-*-* } .-1 }
        asm!("{}", in(reg_byte) 0i8);

        // Split inout operands must have compatible types

        let mut val_i16: i16;
        let mut val_f32: f32;
        let mut val_u32: u32;
        let mut val_u64: u64;
        let mut val_ptr: *mut u8;
        asm!("{:r}", inout(reg) 0u16 => val_i16);
        asm!("{:r}", inout(reg) 0u32 => val_f32);
// { dg-error "" "" { target *-*-* } .-1 }
        asm!("{:r}", inout(reg) 0u32 => val_ptr);
// { dg-error "" "" { target *-*-* } .-1 }
        asm!("{:r}", inout(reg) main => val_u32);
// { dg-error "" "" { target *-*-* } .-1 }
        asm!("{:r}", inout(reg) 0u64 => val_ptr);
        asm!("{:r}", inout(reg) main => val_u64);
    }
}

