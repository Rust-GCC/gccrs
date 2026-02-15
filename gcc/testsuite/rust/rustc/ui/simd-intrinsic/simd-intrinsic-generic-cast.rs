// build-fail

#![feature(repr_simd, platform_intrinsics)]

#[repr(simd)]
#[derive(Copy, Clone)]
#[allow(non_camel_case_types)]
struct i32x4(i32, i32, i32, i32);
#[repr(simd)]
#[derive(Copy, Clone)]
#[allow(non_camel_case_types)]
struct i32x8(i32, i32, i32, i32,
             i32, i32, i32, i32);

#[repr(simd)]
#[derive(Copy, Clone)]
#[allow(non_camel_case_types)]
struct f32x4(f32, f32, f32, f32);
#[repr(simd)]
#[derive(Copy, Clone)]
#[allow(non_camel_case_types)]
struct f32x8(f32, f32, f32, f32,
             f32, f32, f32, f32);


extern "platform-intrinsic" {
    fn simd_cast<T, U>(x: T) -> U;
}

fn main() {
    let x = i32x4(0, 0, 0, 0);

    unsafe {
        simd_cast::<i32, i32>(0);
// { dg-error ".E0511." "" { target *-*-* } .-1 }
        simd_cast::<i32, i32x4>(0);
// { dg-error ".E0511." "" { target *-*-* } .-1 }
        simd_cast::<i32x4, i32>(x);
// { dg-error ".E0511." "" { target *-*-* } .-1 }
        simd_cast::<_, i32x8>(x);
// { dg-error ".E0511." "" { target *-*-* } .-1 }
    }
}

