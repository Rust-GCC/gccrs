// build-fail
// ignore-emscripten

// Test that the simd_reduce_{op} intrinsics produce ok-ish error
// messages when misused.

#![feature(repr_simd, platform_intrinsics)]
#![allow(non_camel_case_types)]

#[repr(simd)]
#[derive(Copy, Clone)]
pub struct f32x4(pub f32, pub f32, pub f32, pub f32);

#[repr(simd)]
#[derive(Copy, Clone)]
pub struct u32x4(pub u32, pub u32, pub u32, pub u32);


extern "platform-intrinsic" {
    fn simd_reduce_add_ordered<T, U>(x: T, y: U) -> U;
    fn simd_reduce_mul_ordered<T, U>(x: T, y: U) -> U;
    fn simd_reduce_and<T, U>(x: T) -> U;
    fn simd_reduce_or<T, U>(x: T) -> U;
    fn simd_reduce_xor<T, U>(x: T) -> U;
    fn simd_reduce_all<T>(x: T) -> bool;
    fn simd_reduce_any<T>(x: T) -> bool;
}

fn main() {
    let x = u32x4(0, 0, 0, 0);
    let z = f32x4(0.0, 0.0, 0.0, 0.0);

    unsafe {
        simd_reduce_add_ordered(z, 0);
// { dg-error ".E0511." "" { target *-*-* } .-1 }
        simd_reduce_mul_ordered(z, 1);
// { dg-error ".E0511." "" { target *-*-* } .-1 }

        let _: f32 = simd_reduce_and(x);
// { dg-error ".E0511." "" { target *-*-* } .-1 }
        let _: f32 = simd_reduce_or(x);
// { dg-error ".E0511." "" { target *-*-* } .-1 }
        let _: f32 = simd_reduce_xor(x);
// { dg-error ".E0511." "" { target *-*-* } .-1 }

        let _: f32 = simd_reduce_and(z);
// { dg-error ".E0511." "" { target *-*-* } .-1 }
        let _: f32 = simd_reduce_or(z);
// { dg-error ".E0511." "" { target *-*-* } .-1 }
        let _: f32 = simd_reduce_xor(z);
// { dg-error ".E0511." "" { target *-*-* } .-1 }

        let _: bool = simd_reduce_all(z);
// { dg-error ".E0511." "" { target *-*-* } .-1 }
        let _: bool = simd_reduce_any(z);
// { dg-error ".E0511." "" { target *-*-* } .-1 }
    }
}

