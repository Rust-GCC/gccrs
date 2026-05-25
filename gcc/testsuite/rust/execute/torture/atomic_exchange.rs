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

    macro_rules !impl_clone
  {
    ($ ($t : ty) *) =>
    {
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
      usize u8 u16 u32 u64   // u128
        isize i8 i16 i32 i64 // i128
      f32 f64 bool char
    }
}

#[lang = "copy"]
pub trait Copy: Clone {
    // Empty.
}

mod copy_impls {
    use super::Copy;

    macro_rules !impl_copy
  {
    ($ ($t : ty) *) => {$ (impl Copy for $t{}) * }
  }

    impl_copy! {
      usize u8 u16 u32 u64   // u128
        isize i8 i16 i32 i64 // i128
      f32 f64 bool char
    }
}

extern "rust-intrinsic" {
    pub fn atomic_xchg<T: Copy>(dst: *mut T, val: T) -> T;
    pub fn atomic_xchg_acq<T: Copy>(dst: *mut T, val: T) -> T;
    pub fn atomic_xchg_rel<T: Copy>(dst: *mut T, val: T) -> T;
    pub fn atomic_xchg_acqrel<T: Copy>(dst: *mut T, val: T) -> T;
    pub fn atomic_xchg_relaxed<T: Copy>(dst: *mut T, val: T) -> T;
}

fn main() -> u32 {
    let mut dst = 15u32;
    let init = dst;
    let one;
    let two;
    let three;
    let four;
    let five;

    unsafe {
        let mut ret = atomic_xchg(&mut dst, 1);
        one = dst;

        if ret != init {
            return ret;
        }

        ret = atomic_xchg_acq(&mut dst, 2);
        two = dst;

        if ret != one {
            return ret;
        }

        ret = atomic_xchg_rel(&mut dst, 3);
        three = dst;

        if ret != two {
            return ret;
        }

        ret = atomic_xchg_acqrel(&mut dst, 4);
        four = dst;

        if ret != three {
            return ret;
        }

        ret = atomic_xchg_relaxed(&mut dst, 5);
        five = dst;

        if ret != four {
            return ret;
        }
    }

    (five + four + three + two + one) - init
}
