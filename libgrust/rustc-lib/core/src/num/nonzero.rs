//! Definitions of integer that is known not to equal zero.

use crate::fmt;
use crate::ops::{BitOr, BitOrAssign};
use crate::str::FromStr;

use super::from_str_radix;
use super::{IntErrorKind, ParseIntError};

macro_rules! doc_comment {
    ($x:expr, $($tt:tt)*) => {
        #[doc = $x]
        $($tt)*
    };
}

macro_rules! impl_nonzero_fmt {
    ( #[$stability: meta] ( $( $Trait: ident ),+ ) for $Ty: ident ) => {
        $(
            #[$stability]
            impl fmt::$Trait for $Ty {
                #[inline]
                fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
                    self.get().fmt(f)
                }
            }
        )+
    }
}

macro_rules! nonzero_integers {
    ( $( #[$stability: meta] $Ty: ident($Int: ty); )+ ) => {
        $(
            doc_comment! {
                concat!("An integer that is known not to equal zero.

This enables some memory layout optimization.
For example, `Option<", stringify!($Ty), ">` is the same size as `", stringify!($Int), "`:

```rust
use std::mem::size_of;
assert_eq!(size_of::<Option<core::num::", stringify!($Ty), ">>(), size_of::<", stringify!($Int),
">());
```"),
                #[$stability]
                #[derive(Copy, Clone, Eq, PartialEq, Ord, PartialOrd, Hash)]
                #[repr(transparent)]
                #[rustc_layout_scalar_valid_range_start(1)]
                #[rustc_nonnull_optimization_guaranteed]
                pub struct $Ty($Int);
            }

            impl $Ty {
                /// Creates a non-zero without checking the value.
                ///
                /// # Safety
                ///
                /// The value must not be zero.
                #[$stability]
                #[rustc_const_stable(feature = "nonzero", since = "1.34.0")]
                #[inline]
                pub const unsafe fn new_unchecked(n: $Int) -> Self {
                    // SAFETY: this is guaranteed to be safe by the caller.
                    unsafe { Self(n) }
                }

                /// Creates a non-zero if the given value is not zero.
                #[$stability]
                #[rustc_const_stable(feature = "const_nonzero_int_methods", since = "1.47.0")]
                #[inline]
                pub const fn new(n: $Int) -> Option<Self> {
                    if n != 0 {
                        // SAFETY: we just checked that there's no `0`
                        Some(unsafe { Self(n) })
                    } else {
                        None
                    }
                }

                /// Returns the value as a primitive type.
                #[$stability]
                #[inline]
                #[rustc_const_stable(feature = "nonzero", since = "1.34.0")]
                pub const fn get(self) -> $Int {
                    self.0
                }

            }

            #[stable(feature = "from_nonzero", since = "1.31.0")]
            impl From<$Ty> for $Int {
                doc_comment! {
                    concat!(
"Converts a `", stringify!($Ty), "` into an `", stringify!($Int), "`"),
                    #[inline]
                    fn from(nonzero: $Ty) -> Self {
                        nonzero.0
                    }
                }
            }

            #[stable(feature = "nonzero_bitor", since = "1.45.0")]
            impl BitOr for $Ty {
                type Output = Self;
                #[inline]
                fn bitor(self, rhs: Self) -> Self::Output {
                    // SAFETY: since `self` and `rhs` are both nonzero, the
                    // result of the bitwise-or will be nonzero.
                    unsafe { $Ty::new_unchecked(self.get() | rhs.get()) }
                }
            }

            #[stable(feature = "nonzero_bitor", since = "1.45.0")]
            impl BitOr<$Int> for $Ty {
                type Output = Self;
                #[inline]
                fn bitor(self, rhs: $Int) -> Self::Output {
                    // SAFETY: since `self` is nonzero, the result of the
                    // bitwise-or will be nonzero regardless of the value of
                    // `rhs`.
                    unsafe { $Ty::new_unchecked(self.get() | rhs) }
                }
            }

            #[stable(feature = "nonzero_bitor", since = "1.45.0")]
            impl BitOr<$Ty> for $Int {
                type Output = $Ty;
                #[inline]
                fn bitor(self, rhs: $Ty) -> Self::Output {
                    // SAFETY: since `rhs` is nonzero, the result of the
                    // bitwise-or will be nonzero regardless of the value of
                    // `self`.
                    unsafe { $Ty::new_unchecked(self | rhs.get()) }
                }
            }

            #[stable(feature = "nonzero_bitor", since = "1.45.0")]
            impl BitOrAssign for $Ty {
                #[inline]
                fn bitor_assign(&mut self, rhs: Self) {
                    *self = *self | rhs;
                }
            }

            #[stable(feature = "nonzero_bitor", since = "1.45.0")]
            impl BitOrAssign<$Int> for $Ty {
                #[inline]
                fn bitor_assign(&mut self, rhs: $Int) {
                    *self = *self | rhs;
                }
            }

            impl_nonzero_fmt! {
                #[$stability] (Debug, Display, Binary, Octal, LowerHex, UpperHex) for $Ty
            }
        )+
    }
}

nonzero_integers! {
    #[stable(feature = "nonzero", since = "1.28.0")] NonZeroU8(u8);
    #[stable(feature = "nonzero", since = "1.28.0")] NonZeroU16(u16);
    #[stable(feature = "nonzero", since = "1.28.0")] NonZeroU32(u32);
    #[stable(feature = "nonzero", since = "1.28.0")] NonZeroU64(u64);
    #[stable(feature = "nonzero", since = "1.28.0")] NonZeroU128(u128);
    #[stable(feature = "nonzero", since = "1.28.0")] NonZeroUsize(usize);
    #[stable(feature = "signed_nonzero", since = "1.34.0")] NonZeroI8(i8);
    #[stable(feature = "signed_nonzero", since = "1.34.0")] NonZeroI16(i16);
    #[stable(feature = "signed_nonzero", since = "1.34.0")] NonZeroI32(i32);
    #[stable(feature = "signed_nonzero", since = "1.34.0")] NonZeroI64(i64);
    #[stable(feature = "signed_nonzero", since = "1.34.0")] NonZeroI128(i128);
    #[stable(feature = "signed_nonzero", since = "1.34.0")] NonZeroIsize(isize);
}

macro_rules! from_str_radix_nzint_impl {
    ($($t:ty)*) => {$(
        #[stable(feature = "nonzero_parse", since = "1.35.0")]
        impl FromStr for $t {
            type Err = ParseIntError;
            fn from_str(src: &str) -> Result<Self, Self::Err> {
                Self::new(from_str_radix(src, 10)?)
                    .ok_or(ParseIntError {
                        kind: IntErrorKind::Zero
                    })
            }
        }
    )*}
}

from_str_radix_nzint_impl! { NonZeroU8 NonZeroU16 NonZeroU32 NonZeroU64 NonZeroU128 NonZeroUsize
NonZeroI8 NonZeroI16 NonZeroI32 NonZeroI64 NonZeroI128 NonZeroIsize }
