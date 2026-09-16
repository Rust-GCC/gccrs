// build-fail

#![feature(platform_intrinsics)]

extern "platform-intrinsic" {
    fn simd_add<T>(a: T, b: T) -> T;
}

fn main() {
    unsafe { simd_add(0, 1); } // { dg-error ".E0511." "" { target *-*-* } }
}

