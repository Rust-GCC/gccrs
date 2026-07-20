#![feature(no_core)]
#![feature(lang_items)]
#![feature(fundamental)]
#![feature(rustc_attrs)]
#![feature(arbitrary_self_types)]
#![feature(optin_builtin_traits)]
#![feature(prelude_import)]
#![feature(intrinsics)]
#![feature(staged_api)]
#![no_core]

#[macro_export]
macro_rules! panic {
    ($($arg:tt)*) => {
        $crate::begin_panic()
    };
}

#[macro_export]
#[rustc_builtin_macro]
macro_rules! assert {
    ($cond:expr $(,)?) => {{ /* compiler built-in */ }};
    ($cond:expr, $($arg:tt)+) => {{ /* compiler built-in */ }};
}

pub mod ffi {
    #[repr(u8)]
    pub enum c_void {
        __variant1,
    }
}

#[rustc_builtin_macro]
#[macro_export]
macro_rules! concat {
    ($($e:expr),* $(,)?) => {{ /* compiler built-in */ }};
}

#[rustc_builtin_macro]
#[macro_export]
macro_rules! stringify {
    ($($t:tt)*) => {
        /* compiler built-in */
    };
}

pub const fn begin_panic() {}

pub mod mem {

    use crate::intrinsics;

    #[inline(always)]
    #[rustc_promotable]
    pub const fn size_of<T>() -> usize {
        intrinsics::size_of::<T>()
    }
}

pub mod intrinsics {

    extern "rust-intrinsic" {
        #[rustc_const_stable(feature = "const_size_of", since = "1.40.0")]
        pub fn size_of<T>() -> usize;
    }
}

pub mod marker {
    #[lang = "sized"]
    #[fundamental] // for Default, for example, which requires that `[T]: !Default` be evaluatable
    #[rustc_specialization_trait]
    pub trait Sized {
        // Empty.
    }

    #[lang = "copy"]
    #[rustc_unsafe_specialization_marker]
    pub trait Copy: Clone {
        // Empty.
    }
}

pub mod clone {
    #[lang = "clone"]
    pub trait Clone: Sized {
        #[must_use = "cloning is often expensive and is not expected to have side effects"]
        fn clone(&self) -> Self;

        /// Performs copy-assignment from `source`.
        ///
        /// `a.clone_from(&b)` is equivalent to `a = b.clone()` in functionality,
        /// but can be overridden to reuse the resources of `a` to avoid unnecessary
        /// allocations.
        #[inline]
        fn clone_from(&mut self, source: &Self) {
            *self = source.clone()
        }
    }
}

#[lang = "sync"]
pub unsafe auto trait Sync {}

pub unsafe auto trait Send {
    // empty.
}

#[lang = "structural_peq"]
pub trait StructuralPartialEq {
    // Empty.
}

#[lang = "structural_teq"]
pub trait StructuralEq {
    // Empty.
}

#[lang = "receiver"]
#[doc(hidden)]
pub trait Receiver {
    // Empty.
}

#[prelude_import]
pub use prelude::*;

mod prelude {
    pub use crate::clone::Clone;
    pub use crate::concat;
    pub use crate::marker::{Copy, Sized};
    pub use crate::panic;
    pub use crate::stringify;
}
