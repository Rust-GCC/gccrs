#![feature(no_core)]
#![no_core]

#![feature(intrinsics)]

#![feature(lang_items)]
#[lang = "sized"]
pub trait Sized {}

#[lang = "clone"]
pub trait Clone: Sized {
    fn clone(&self) -> Self;

    fn clone_from(&mut self, source: &Self) {
        *self = source.clone()
    }
}

mod impls {
    use super::Clone;

    macro_rules! impl_clone {
        ($($t:ty)*) => {
            $(
                impl Clone for $t {
                    fn clone(&self) -> Self {
                        *self
                    }
                }
            )*
        }
    }

    impl_clone! {
        usize u8 u16 u32 u64 // u128
        isize i8 i16 i32 i64 // i128
        f32 f64
        bool char
    }
}

#[lang = "copy"]
pub trait Copy: Clone {
    // Empty.
}

mod copy_impls {
    use super::Copy;

    macro_rules! impl_copy {
        ($($t:ty)*) => {
            $(
                impl Copy for $t {}
            )*
        }
    }

    impl_copy! {
        usize u8 u16 u32 u64 // u128
        isize i8 i16 i32 i64 // i128
        f32 f64
        bool char
    }
}

extern "rust-intrinsic" {
    pub fn atomic_cxchg<T: Copy>(dst: *mut T, old: T, new: T) -> (T, bool);
    pub fn atomic_cxchg_acq<T: Copy>(dst: *mut T, old: T, new: T) -> (T, bool);
    pub fn atomic_cxchg_rel<T: Copy>(dst: *mut T, old: T, new: T) -> (T, bool);
    pub fn atomic_cxchg_acqrel<T: Copy>(dst: *mut T, old: T, new: T) -> (T, bool);
    pub fn atomic_cxchg_relaxed<T: Copy>(dst: *mut T, old: T, new: T) -> (T, bool);
    pub fn atomic_cxchgweak<T: Copy>(dst: *mut T, old: T, new: T) -> (T, bool);
    pub fn atomic_cxchgweak_acq<T: Copy>(dst: *mut T, old: T, new: T) -> (T, bool);
    pub fn atomic_cxchgweak_rel<T: Copy>(dst: *mut T, old: T, new: T) -> (T, bool);
    pub fn atomic_cxchgweak_acqrel<T: Copy>(dst: *mut T, old: T, new: T) -> (T, bool);
    pub fn atomic_cxchgweak_relaxed<T: Copy>(dst: *mut T, old: T, new: T) -> (T, bool);
}

// macro helper to check both tuple fields and memory after running the cxchg funcs
macro_rules! check {
    ($ty:ty, $strong:ident, $weak:ident) => {{
        let mut src = -7i64 as $ty;
        let initial = src;
        let result = $strong(&mut src, initial, 9 as $ty);
        if result.0 != initial || !result.1 || src != 9 as $ty {
            return 1;
        }
        let result = $strong(&mut src, 3 as $ty, 11 as $ty);
        if result.0 != 9 as $ty || result.1 || src != 9 as $ty {
            return 1;
        }

        let result = $weak(&mut src, 3 as $ty, 11 as $ty);
        if result.0 != 9 as $ty || result.1 || src != 9 as $ty {
            return 1;
        }
        loop {
            let result = $weak(&mut src, 9 as $ty, initial);
            if result.0 != 9 as $ty {
                return 1;
            }
            if result.1 {
                if src != initial {
                    return 1;
                }
                break;
            }
            // weak compare may fail spuriously which can leave memory unchanged
            if src != 9 as $ty {
                return 1;
            }
        }
    }};
}

macro_rules! check_type {
    ($ty:ty) => {{
        check!($ty, atomic_cxchg, atomic_cxchgweak);
        check!($ty, atomic_cxchg_acq, atomic_cxchgweak_acq);
        check!($ty, atomic_cxchg_rel, atomic_cxchgweak_rel);
        check!($ty, atomic_cxchg_acqrel, atomic_cxchgweak_acqrel);
        check!($ty, atomic_cxchg_relaxed, atomic_cxchgweak_relaxed);
    }};
}

fn main() -> i32 {
    unsafe {
        check_type!(u8);
        check_type!(u16);
        check_type!(u32);
        check_type!(u64);
        check_type!(usize);
        check_type!(i8);
        check_type!(i16);
        check_type!(i32);
        check_type!(i64);
        check_type!(isize);
    }
    0
}
