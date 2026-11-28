// build-fail

#![feature(repr_simd, platform_intrinsics)]

#[repr(simd)]
#[derive(Copy, Clone)]
#[allow(non_camel_case_types)]
struct i32x4(i32, i32, i32, i32);
#[repr(simd)]
#[derive(Copy, Clone)]
#[allow(non_camel_case_types)]
struct i16x8(i16, i16, i16, i16,
             i16, i16, i16, i16);

extern "platform-intrinsic" {
    fn simd_eq<T, U>(x: T, y: T) -> U;
    fn simd_ne<T, U>(x: T, y: T) -> U;
    fn simd_lt<T, U>(x: T, y: T) -> U;
    fn simd_le<T, U>(x: T, y: T) -> U;
    fn simd_gt<T, U>(x: T, y: T) -> U;
    fn simd_ge<T, U>(x: T, y: T) -> U;
}

fn main() {
    let x = i32x4(0, 0, 0, 0);

    unsafe {
        simd_eq::<i32, i32>(0, 0);
// { dg-error ".E0511." "" { target *-*-* } .-1 }
        simd_ne::<i32, i32>(0, 0);
// { dg-error ".E0511." "" { target *-*-* } .-1 }
        simd_lt::<i32, i32>(0, 0);
// { dg-error ".E0511." "" { target *-*-* } .-1 }
        simd_le::<i32, i32>(0, 0);
// { dg-error ".E0511." "" { target *-*-* } .-1 }
        simd_gt::<i32, i32>(0, 0);
// { dg-error ".E0511." "" { target *-*-* } .-1 }
        simd_ge::<i32, i32>(0, 0);
// { dg-error ".E0511." "" { target *-*-* } .-1 }

        simd_eq::<_, i32>(x, x);
// { dg-error ".E0511." "" { target *-*-* } .-1 }
        simd_ne::<_, i32>(x, x);
// { dg-error ".E0511." "" { target *-*-* } .-1 }
        simd_lt::<_, i32>(x, x);
// { dg-error ".E0511." "" { target *-*-* } .-1 }
        simd_le::<_, i32>(x, x);
// { dg-error ".E0511." "" { target *-*-* } .-1 }
        simd_gt::<_, i32>(x, x);
// { dg-error ".E0511." "" { target *-*-* } .-1 }
        simd_ge::<_, i32>(x, x);
// { dg-error ".E0511." "" { target *-*-* } .-1 }

        simd_eq::<_, i16x8>(x, x);
// { dg-error ".E0511." "" { target *-*-* } .-1 }
        simd_ne::<_, i16x8>(x, x);
// { dg-error ".E0511." "" { target *-*-* } .-1 }
        simd_lt::<_, i16x8>(x, x);
// { dg-error ".E0511." "" { target *-*-* } .-1 }
        simd_le::<_, i16x8>(x, x);
// { dg-error ".E0511." "" { target *-*-* } .-1 }
        simd_gt::<_, i16x8>(x, x);
// { dg-error ".E0511." "" { target *-*-* } .-1 }
        simd_ge::<_, i16x8>(x, x);
// { dg-error ".E0511." "" { target *-*-* } .-1 }
    }
}

