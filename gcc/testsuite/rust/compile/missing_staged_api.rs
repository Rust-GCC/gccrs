#![feature(no_core)]
#![feature(intrinsics)]
#![no_core]

pub mod intrinsics {

    extern "rust-intrinsic" {
        // { dg-error "stability attributes may not be used outside of the standard library" "" { target *-*-* } .+1 }
        #[rustc_const_stable(feature = "const_size_of", since = "1.40.0")]
        pub fn size_of<T>() -> usize;
    }
}
