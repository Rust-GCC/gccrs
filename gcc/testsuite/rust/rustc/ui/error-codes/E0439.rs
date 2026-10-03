#![feature(platform_intrinsics)]

extern "platform-intrinsic" {
    fn simd_shuffle<A,B>(a: A, b: A, c: [u32; 8]) -> B; // { dg-error ".E0439." "" { target *-*-* } }
}

fn main () {
}

