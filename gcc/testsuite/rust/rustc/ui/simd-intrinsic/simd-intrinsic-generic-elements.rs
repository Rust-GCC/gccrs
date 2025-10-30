// build-fail

#![feature(repr_simd, platform_intrinsics, rustc_attrs)]

#[repr(simd)]
#[derive(Copy, Clone)]
#[allow(non_camel_case_types)]
struct i32x2(i32, i32);
#[repr(simd)]
#[derive(Copy, Clone)]
#[allow(non_camel_case_types)]
struct i32x3(i32, i32, i32);
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
struct f32x2(f32, f32);
#[repr(simd)]
#[derive(Copy, Clone)]
#[allow(non_camel_case_types)]
struct f32x3(f32, f32, f32);
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
    fn simd_insert<T, E>(x: T, idx: u32, y: E) -> T;
    fn simd_extract<T, E>(x: T, idx: u32) -> E;

    fn simd_shuffle2<T, U>(x: T, y: T, idx: [u32; 2]) -> U;
    fn simd_shuffle3<T, U>(x: T, y: T, idx: [u32; 3]) -> U;
    fn simd_shuffle4<T, U>(x: T, y: T, idx: [u32; 4]) -> U;
    fn simd_shuffle8<T, U>(x: T, y: T, idx: [u32; 8]) -> U;
}

fn main() {
    let x = i32x4(0, 0, 0, 0);

    unsafe {
        simd_insert(0, 0, 0);
// { dg-error ".E0511." "" { target *-*-* } .-1 }
        simd_insert(x, 0, 1.0);
// { dg-error ".E0511." "" { target *-*-* } .-1 }
        simd_extract::<_, f32>(x, 0);
// { dg-error ".E0511." "" { target *-*-* } .-1 }

        simd_shuffle2::<i32, i32>(0, 0, [0; 2]);
// { dg-error ".E0511." "" { target *-*-* } .-1 }
        simd_shuffle3::<i32, i32>(0, 0, [0; 3]);
// { dg-error ".E0511." "" { target *-*-* } .-1 }
        simd_shuffle4::<i32, i32>(0, 0, [0; 4]);
// { dg-error ".E0511." "" { target *-*-* } .-1 }
        simd_shuffle8::<i32, i32>(0, 0, [0; 8]);
// { dg-error ".E0511." "" { target *-*-* } .-1 }

        simd_shuffle2::<_, f32x2>(x, x, [0; 2]);
// { dg-error ".E0511." "" { target *-*-* } .-1 }
        simd_shuffle3::<_, f32x3>(x, x, [0; 3]);
// { dg-error ".E0511." "" { target *-*-* } .-1 }
        simd_shuffle4::<_, f32x4>(x, x, [0; 4]);
// { dg-error ".E0511." "" { target *-*-* } .-1 }
        simd_shuffle8::<_, f32x8>(x, x, [0; 8]);
// { dg-error ".E0511." "" { target *-*-* } .-1 }

        simd_shuffle2::<_, i32x8>(x, x, [0; 2]);
// { dg-error ".E0511." "" { target *-*-* } .-1 }
        simd_shuffle3::<_, i32x4>(x, x, [0; 3]);
// { dg-error ".E0511." "" { target *-*-* } .-1 }
        simd_shuffle4::<_, i32x3>(x, x, [0; 4]);
// { dg-error ".E0511." "" { target *-*-* } .-1 }
        simd_shuffle8::<_, i32x2>(x, x, [0; 8]);
// { dg-error ".E0511." "" { target *-*-* } .-1 }
    }
}

