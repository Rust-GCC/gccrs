//! # Mini standard library
//!
//! This library is intended to replace the standard library during the
//! experimental phase of GCC's rust frontend.
//! When GCC will be able to compile the whole standard library this will
//! be deprecated and deleted.
//!
//! The overall goal of this library is to bring as many features as
//! possible from the rust standard library whilst staying compilable using
//! GCC.
//!
//! The API should be kept as close as possible to the standard library but
//! some things may slightly change to accomodate the capabilities of GCC.
//! The API may break without any warning, this library is only intended
//! for experimental purpose. If you need a production ready library this
//! one is not what you're looking for. Take a look at the official
//! standard library instead.
#![feature(rustc_attrs)]

use Option::{None, Some};
use Ordering::{Equal, Greater, Less};

#[macro_export]
macro_rules! matches {
    ($expression:expr, $( $pattern:pat )|+ $( if $guard: expr )? $(,)?) => {
        match $expression {
            $( $pattern )|+ $( if $guard )? => true,
            _ => false
        }
    }
}

#[rustc_builtin_macro]
#[macro_export]
macro_rules! line {
    () => {
        /* compiler built-in */
    };
}

#[rustc_builtin_macro]
#[macro_export]
macro_rules! column {
    () => {
        /* compiler built-in */
    };
}

#[rustc_builtin_macro]
#[macro_export]
macro_rules! file {
    () => {
        /* compiler built-in */
    };
}

#[rustc_builtin_macro]
#[macro_export]
macro_rules! stringify {
    ($($t:tt)*) => {
        /* compiler built-in */
    };
}

#[rustc_builtin_macro]
#[macro_export]
macro_rules! compile_error {
    ($msg:expr $(,)?) => {{ /* compiler built-in */ }};
}

pub unsafe auto trait Send {
    // empty
}

impl<T: ?Sized> !Send for *const T {}

impl<T: ?Sized> !Send for *mut T {}

pub unsafe auto trait Sync {
    // Empty
}

impl<T: ?Sized> !Sync for *const T {}

impl<T: ?Sized> !Sync for *mut T {}

#[lang = "sized"]
pub trait Sized {
    // Empty.
}

// #[lang = "unsize"] // Not implemented yet
pub trait Unsize<T: ?Sized> {
    // Empty.
}

// #[lang = "structural_peq"]
pub trait StructuralPartialEq {
    // Empty.
}

// #[lang = "structural_teq"]
pub trait StructuralEq {
    // Empty.
}

#[rustc_builtin_macro]
pub macro Copy($item:item) {
    /* compiler built-in */
}

#[lang = "clone"]
pub trait Clone: Sized {
    fn clone(&self) -> Self;

    fn clone_from(&mut self, source: &Self) {
        *self = source.clone()
    }
}

#[lang = "copy"]
pub trait Copy: Clone {
    // Empty.
}

#[rustc_builtin_macro]
pub macro Clone($item:item) {
    /* compiler built-in */
}

mod impls {

    macro_rules! impl_clone {
        ($($t:ty)*) => {
            $(
                impl Clone for $t {
                    #[inline]
                    fn clone(&self) -> Self {
                        *self
                    }
                }
            )*
        }
    }

    impl_clone! {
        usize u8 u16 u32 u64 u128
        isize i8 i16 i32 i64 i128
        f32 f64
        bool char
    }
}

impl<T: ?Sized> Clone for *const T {
    #[inline]
    fn clone(&self) -> Self {
        *self
    }
}

impl<T: ?Sized> Clone for *mut T {
    #[inline]
    fn clone(&self) -> Self {
        *self
    }
}

/// Shared references can be cloned, but mutable references *cannot*!
impl<T: ?Sized> Clone for &T {
    #[inline]
    fn clone(&self) -> Self {
        *self
    }
}

// /// Shared references can be cloned, but mutable references *cannot*!
// impl<T: ?Sized> !Clone for &mut T {}

pub trait PartialEq<Rhs: ?Sized = Self> {
    /// This method tests for `self` and `other` values to be equal, and is used
    /// by `==`.
    fn eq(&self, other: &Rhs) -> bool;

    /// This method tests for `!=`.
    #[inline]
    fn ne(&self, other: &Rhs) -> bool {
        !self.eq(other)
    }
}

#[rustc_builtin_macro]
pub macro PartialEq($item:item) {
    /* compiler built-in */
}

pub trait Eq: PartialEq<Self> {
    fn assert_receiver_is_total_eq(&self) {}
}

#[rustc_builtin_macro]
pub macro Eq($item:item) {
    /* compiler built-in */
}

macro_rules! impls {
    ($t: ident) => {
        // impl<T: ?Sized> PartialEq for $t<T> {
        //     fn eq(&self, _other: &$t<T>) -> bool {
        //         true
        //     }
        // }

        impl<T: ?Sized> Eq for $t<T> {}

        // impl<T: ?Sized> PartialOrd for $t<T> {
        //     fn partial_cmp(&self, _other: &$t<T>) -> Option<Ordering> {
        //         Option::Some(Ordering::Equal)
        //     }
        // }

        // impl<T: ?Sized> Ord for $t<T> {
        //     fn cmp(&self, _other: &$t<T>) -> Ordering {
        //         Ordering::Equal
        //     }
        // }

        impl<T: ?Sized> Copy for $t<T> {}

        impl<T: ?Sized> Clone for $t<T> {
            fn clone(&self) -> Self {
                Self
            }
        }

        impl<T: ?Sized> Default for $t<T> {
            fn default() -> Self {
                Self
            }
        }

        impl<T: ?Sized> StructuralPartialEq for $t<T> {}

        impl<T: ?Sized> StructuralEq for $t<T> {}
    };
}

// #[derive(Clone, Copy, PartialEq, Debug, Hash)]
pub enum Ordering {
    /// An ordering where a compared value is less than another.
    Less = -1,
    /// An ordering where a compared value is equal to another.
    Equal = 0,
    /// An ordering where a compared value is greater than another.
    Greater = 1,
}

impl PartialEq<Ordering> for Ordering {
    fn eq(&self, other: &Ordering) -> bool {
        self == other
    }
}

// impl PartialOrd for Ordering {
//     #[inline]
//     fn partial_cmp(&self, other: &Ordering) -> Option<Ordering> {
//         (*self as i32).partial_cmp(&(*other as i32))
//     }
// }

impl Ordering {
    pub const fn reverse(self) -> Ordering {
        match self {
            Ordering::Less => Ordering::Greater,
            Ordering::Equal => Ordering::Equal,
            Ordering::Greater => Ordering::Less,
        }
    }

    pub const fn then(self, other: Ordering) -> Ordering {
        match self {
            Ordering::Equal => other,
            _ => self,
        }
    }

    // pub fn then_with<F: FnOnce() -> Ordering>(self, f: F) -> Ordering {
    //     match self {
    //         Equal => f(),
    //         _ => self,
    //     }
    // }
}

pub struct Reverse<T>(pub T);

// impl<T: PartialOrd> PartialOrd for Reverse<T> {
//     fn partial_cmp(&self, other: &Reverse<T>) -> Option<Ordering> {
//         other.0.partial_cmp(&self.0)
//     }

//     fn lt(&self, other: &Self) -> bool {
//         other.0 < self.0
//     }

//     fn le(&self, other: &Self) -> bool {
//         other.0 <= self.0
//     }

//     fn gt(&self, other: &Self) -> bool {
//         other.0 > self.0
//     }

//     fn ge(&self, other: &Self) -> bool {
//         other.0 >= self.0
//     }
// }

// impl<T: Ord> Ord for Reverse<T> {
//     fn cmp(&self, other: &Reverse<T>) -> Ordering {
//         other.0.cmp(&self.0)
//     }
// }

pub trait Ord: Eq + PartialOrd<Self> {
    /// This method returns an [`Ordering`] between `self` and `other`.
    ///
    /// By convention, `self.cmp(&other)` returns the ordering matching the expression
    /// `self <operator> other` if true.
    ///
    /// # Examples
    ///
    /// ```
    /// use std::cmp::Ordering;
    ///
    /// assert_eq!(5.cmp(&10), Ordering::Less);
    /// assert_eq!(10.cmp(&5), Ordering::Greater);
    /// assert_eq!(5.cmp(&5), Ordering::Equal);
    /// ```
    fn cmp(&self, other: &Self) -> Ordering;

    /// Compares and returns the maximum of two values.
    ///
    /// Returns the second argument if the comparison determines them to be equal.
    ///
    /// # Examples
    ///
    /// ```
    /// assert_eq!(2, 1.max(2));
    /// assert_eq!(2, 2.max(2));
    /// ```
    #[inline]
    #[must_use]
    fn max(self, other: Self) -> Self
    where
        Self: Sized,
    {
        match Self::cmp(&self, &other) {
            Ordering::Less | Ordering::Equal => other,
            Ordering::Greater => self,
        }
    }

    /// Compares and returns the minimum of two values.
    ///
    /// Returns the first argument if the comparison determines them to be equal.
    ///
    /// # Examples
    ///
    /// ```
    /// assert_eq!(1, 1.min(2));
    /// assert_eq!(2, 2.min(2));
    /// ```
    fn min(self, other: Self) -> Self
    where
        Self: Sized,
    {
        match Self::cmp(&self, &other) {
            Ordering::Less | Ordering::Equal => self,
            Ordering::Greater => other,
        }
    }
}

#[rustc_builtin_macro]
pub macro Ord($item:item) {
    /* compiler built-in */
}

// impl Eq for Ordering {}

// impl Ord for Ordering {
//     fn cmp(&self, other: &Ordering) -> Ordering {
//         (*self as i32).cmp(&(*other as i32))
//     }
// }

pub trait PartialOrd<Rhs: ?Sized = Self>: PartialEq<Rhs> {
    /// This method returns an ordering between `self` and `other` values if one exists.
    ///
    /// # Examples
    ///
    /// ```
    /// use std::cmp::Ordering;
    ///
    /// let result = 1.0.partial_cmp(&2.0);
    /// assert_eq!(result, Some(Ordering::Less));
    ///
    /// let result = 1.0.partial_cmp(&1.0);
    /// assert_eq!(result, Some(Ordering::Equal));
    ///
    /// let result = 2.0.partial_cmp(&1.0);
    /// assert_eq!(result, Some(Ordering::Greater));
    /// ```
    ///
    /// When comparison is impossible:
    ///
    /// ```
    /// let result = f64::NAN.partial_cmp(&1.0);
    /// assert_eq!(result, None);
    /// ```
    fn partial_cmp(&self, other: &Rhs) -> Option<Ordering>;

    /// This method tests less than (for `self` and `other`) and is used by the `<` operator.
    ///
    /// # Examples
    ///
    /// ```
    /// let result = 1.0 < 2.0;
    /// assert_eq!(result, true);
    ///
    /// let result = 2.0 < 1.0;
    /// assert_eq!(result, false);
    /// ```
    fn lt(&self, other: &Rhs) -> bool {
        matches!(self.partial_cmp(other), Some(Ordering::Less))
    }

    /// This method tests less than or equal to (for `self` and `other`) and is used by the `<=`
    /// operator.
    ///
    /// # Examples
    ///
    /// ```
    /// let result = 1.0 <= 2.0;
    /// assert_eq!(result, true);
    ///
    /// let result = 2.0 <= 2.0;
    /// assert_eq!(result, true);
    /// ```
    fn le(&self, other: &Rhs) -> bool {
        matches!(
            self.partial_cmp(other),
            Some(Ordering::Less | Ordering::Equal)
        )
    }

    /// This method tests greater than (for `self` and `other`) and is used by the `>` operator.
    ///
    /// # Examples
    ///
    /// ```
    /// let result = 1.0 > 2.0;
    /// assert_eq!(result, false);
    ///
    /// let result = 2.0 > 2.0;
    /// assert_eq!(result, false);
    /// ```
    fn gt(&self, other: &Rhs) -> bool {
        matches!(self.partial_cmp(other), Some(Ordering::Greater))
    }

    /// This method tests greater than or equal to (for `self` and `other`) and is used by the `>=`
    /// operator.
    ///
    /// # Examples
    ///
    /// ```
    /// let result = 2.0 >= 1.0;
    /// assert_eq!(result, true);
    ///
    /// let result = 2.0 >= 2.0;
    /// assert_eq!(result, true);
    /// ```
    fn ge(&self, other: &Rhs) -> bool {
        matches!(
            self.partial_cmp(other),
            Some(Ordering::Greater | Ordering::Equal)
        )
    }
}

// pub macro PartialOrd($item:item) {
//     /* compiler built-in */
// }

pub trait Default: Sized {
    fn default() -> Self;
}

#[lang = "phantom_data"]
pub struct PhantomData<T: ?Sized>;

impls! {PhantomData}

pub enum Option<T> {
    /// No value
    #[lang = "None"]
    None,
    /// Some value `T`
    #[lang = "Some"]
    Some(T),
}

#[lang = "add"]
pub trait Add<Rhs = Self> {
    /// The resulting type after applying the `+` operator.
    type Output;

    /// Performs the `+` operation.
    ///
    /// # Example
    ///
    /// ```
    /// assert_eq!(12 + 1, 13);
    /// ```
    #[must_use]
    fn add(self, rhs: Rhs) -> Self::Output;
}

#[lang = "sub"]
pub trait Sub<Rhs = Self> {
    /// The resulting type after applying the `-` operator.
    type Output;

    /// Performs the `-` operation.
    ///
    /// # Example
    ///
    /// ```
    /// assert_eq!(12 - 1, 11);
    /// ```
    #[must_use]
    fn sub(self, rhs: Rhs) -> Self::Output;
}

#[lang = "mul"]
pub trait Mul<Rhs = Self> {
    /// The resulting type after applying the `*` operator.
    #[stable(feature = "rust1", since = "1.0.0")]
    type Output;

    /// Performs the `*` operation.
    ///
    /// # Example
    ///
    /// ```
    /// assert_eq!(12 * 2, 24);
    /// ```
    #[must_use]
    #[stable(feature = "rust1", since = "1.0.0")]
    fn mul(self, rhs: Rhs) -> Self::Output;
}

#[lang = "div"]
pub trait Div<Rhs = Self> {
    /// The resulting type after applying the `/` operator.
    type Output;

    /// Performs the `/` operation.
    ///
    /// # Example
    ///
    /// ```
    /// assert_eq!(12 / 2, 6);
    /// ```
    #[must_use]
    fn div(self, rhs: Rhs) -> Self::Output;
}

macro_rules! forward_ref_unop {
    (impl $imp:ident, $method:ident for $t:ty) => {
        forward_ref_unop!(impl $imp, $method for $t,
                #[stable(feature = "rust1", since = "1.0.0")]);
    };
    (impl $imp:ident, $method:ident for $t:ty, #[$attr:meta]) => {
        #[$attr]
        impl $imp for &$t {
            type Output = <$t as $imp>::Output;

            #[inline]
            fn $method(self) -> <$t as $imp>::Output {
                $imp::$method(*self)
            }
        }
    }
}

macro_rules! forward_ref_binop {
    (impl $imp:ident, $method:ident for $t:ty, $u:ty) => {
        forward_ref_binop!(impl $imp, $method for $t, $u,
                #[stable(feature = "rust1", since = "1.0.0")]);
    };
    (impl $imp:ident, $method:ident for $t:ty, $u:ty, #[$attr:meta]) => {
        #[$attr]
        impl<'a> $imp<$u> for &'a $t {
            type Output = <$t as $imp<$u>>::Output;

            #[inline]
            fn $method(self, other: $u) -> <$t as $imp<$u>>::Output {
                $imp::$method(*self, other)
            }
        }

        #[$attr]
        impl $imp<&$u> for $t {
            type Output = <$t as $imp<$u>>::Output;

            #[inline]
            fn $method(self, other: &$u) -> <$t as $imp<$u>>::Output {
                $imp::$method(self, *other)
            }
        }

        #[$attr]
        impl $imp<&$u> for &$t {
            type Output = <$t as $imp<$u>>::Output;

            #[inline]
            fn $method(self, other: &$u) -> <$t as $imp<$u>>::Output {
                $imp::$method(*self, *other)
            }
        }
    }
}

macro_rules! forward_ref_op_assign {
    (impl $imp:ident, $method:ident for $t:ty, $u:ty) => {
        forward_ref_op_assign!(impl $imp, $method for $t, $u,
                #[stable(feature = "op_assign_builtins_by_ref", since = "1.22.0")]);
    };
    (impl $imp:ident, $method:ident for $t:ty, $u:ty, #[$attr:meta]) => {
        #[$attr]
        impl $imp<&$u> for $t {
            #[inline]
            fn $method(&mut self, other: &$u) {
                $imp::$method(self, *other);
            }
        }
    }
}

macro_rules! add_impl {
    ($($t:ty)*) => ($(
        #[stable(feature = "rust1", since = "1.0.0")]
        impl Add for $t {
            type Output = $t;

            #[inline]
            #[rustc_inherit_overflow_checks]
            fn add(self, other: $t) -> $t { self + other }
        }

        forward_ref_binop! { impl Add, add for $t, $t }
    )*)
}

add_impl! { usize u8 u16 u32 u64 u128 isize i8 i16 i32 i64 i128 f32 f64 }

macro_rules! sub_impl {
    ($($t:ty)*) => ($(
        #[stable(feature = "rust1", since = "1.0.0")]
        impl Sub for $t {
            type Output = $t;

            #[inline]
            #[rustc_inherit_overflow_checks]
            fn sub(self, other: $t) -> $t { self - other }
        }

        forward_ref_binop! { impl Sub, sub for $t, $t }
    )*)
}

sub_impl! { usize u8 u16 u32 u64 u128 isize i8 i16 i32 i64 i128 f32 f64 }

macro_rules! mul_impl {
    ($($t:ty)*) => ($(
        #[stable(feature = "rust1", since = "1.0.0")]
        impl Mul for $t {
            type Output = $t;

            #[inline]
            #[rustc_inherit_overflow_checks]
            fn mul(self, other: $t) -> $t { self * other }
        }

        forward_ref_binop! { impl Mul, mul for $t, $t }
    )*)
}

mul_impl! { usize u8 u16 u32 u64 u128 isize i8 i16 i32 i64 i128 f32 f64 }

macro_rules! div_impl_integer {
    ($($t:ty)*) => ($(
        /// This operation rounds towards zero, truncating any
        /// fractional part of the exact result.
        impl Div for $t {
            type Output = $t;

            #[inline]
            fn div(self, other: $t) -> $t { self / other }
        }

        forward_ref_binop! { impl Div, div for $t, $t }
    )*)
}

div_impl_integer! { usize u8 u16 u32 u64 u128 isize i8 i16 i32 i64 i128 }

macro_rules! div_impl_float {
    ($($t:ty)*) => ($(
        impl Div for $t {
            type Output = $t;

            #[inline]
            fn div(self, other: $t) -> $t { self / other }
        }

        forward_ref_binop! { impl Div, div for $t, $t }
    )*)
}

div_impl_float! { f32 f64 }

#[lang = "rem"]
pub trait Rem<Rhs = Self> {
    /// The resulting type after applying the `%` operator.
    type Output;

    /// Performs the `%` operation.
    ///
    /// # Example
    ///
    /// ```
    /// assert_eq!(12 % 10, 2);
    /// ```
    #[must_use]
    fn rem(self, rhs: Rhs) -> Self::Output;
}

macro_rules! rem_impl_integer {
    ($($t:ty)*) => ($(
        /// This operation satisfies `n % d == n - (n / d) * d`. The
        /// result has the same sign as the left operand.
        impl Rem for $t {
            type Output = $t;

            #[inline]
            fn rem(self, other: $t) -> $t { self % other }
        }

        forward_ref_binop! { impl Rem, rem for $t, $t }
    )*)
}

rem_impl_integer! { usize u8 u16 u32 u64 u128 isize i8 i16 i32 i64 i128 }

macro_rules! rem_impl_float {
    ($($t:ty)*) => ($(

        /// The remainder from the division of two floats.
        ///
        /// The remainder has the same sign as the dividend and is computed as:
        /// `x - (x / y).trunc() * y`.
        ///
        /// # Examples
        /// ```
        /// let x: f32 = 50.50;
        /// let y: f32 = 8.125;
        /// let remainder = x - (x / y).trunc() * y;
        ///
        /// // The answer to both operations is 1.75
        /// assert_eq!(x % y, remainder);
        /// ```
        impl Rem for $t {
            type Output = $t;

            #[inline]
            fn rem(self, other: $t) -> $t { self % other }
        }

        forward_ref_binop! { impl Rem, rem for $t, $t }
    )*)
}

rem_impl_float! { f32 f64 }

#[lang = "neg"]
pub trait Neg {
    /// The resulting type after applying the `-` operator.
    type Output;

    /// Performs the unary `-` operation.
    ///
    /// # Example
    ///
    /// ```
    /// let x: i32 = 12;
    /// assert_eq!(-x, -12);
    /// ```
    #[must_use]
    fn neg(self) -> Self::Output;
}

#[lang = "add_assign"]
pub trait AddAssign<Rhs = Self> {
    /// Performs the `+=` operation.
    ///
    /// # Example
    ///
    /// ```
    /// let mut x: u32 = 12;
    /// x += 1;
    /// assert_eq!(x, 13);
    /// ```
    #[stable(feature = "op_assign_traits", since = "1.8.0")]
    fn add_assign(&mut self, rhs: Rhs);
}

macro_rules! add_assign_impl {
    ($($t:ty)+) => ($(
        #[stable(feature = "op_assign_traits", since = "1.8.0")]
        impl AddAssign for $t {
            #[inline]
            #[rustc_inherit_overflow_checks]
            fn add_assign(&mut self, other: $t) { *self += other }
        }

        forward_ref_op_assign! { impl AddAssign, add_assign for $t, $t }
    )+)
}

add_assign_impl! { usize u8 u16 u32 u64 u128 isize i8 i16 i32 i64 i128 f32 f64 }

#[lang = "sub_assign"]
pub trait SubAssign<Rhs = Self> {
    /// Performs the `-=` operation.
    ///
    /// # Example
    ///
    /// ```
    /// let mut x: u32 = 12;
    /// x -= 1;
    /// assert_eq!(x, 11);
    /// ```
    fn sub_assign(&mut self, rhs: Rhs);
}

macro_rules! sub_assign_impl {
    ($($t:ty)+) => ($(
        #[stable(feature = "op_assign_traits", since = "1.8.0")]
        impl SubAssign for $t {
            #[inline]
            #[rustc_inherit_overflow_checks]
            fn sub_assign(&mut self, other: $t) { *self -= other }
        }

        forward_ref_op_assign! { impl SubAssign, sub_assign for $t, $t }
    )+)
}

sub_assign_impl! { usize u8 u16 u32 u64 u128 isize i8 i16 i32 i64 i128 f32 f64 }

#[lang = "mul_assign"]
pub trait MulAssign<Rhs = Self> {
    /// Performs the `*=` operation.
    ///
    /// # Example
    ///
    /// ```
    /// let mut x: u32 = 12;
    /// x *= 2;
    /// assert_eq!(x, 24);
    /// ```
    fn mul_assign(&mut self, rhs: Rhs);
}

macro_rules! mul_assign_impl {
    ($($t:ty)+) => ($(
        #[stable(feature = "op_assign_traits", since = "1.8.0")]
        impl MulAssign for $t {
            #[inline]
            #[rustc_inherit_overflow_checks]
            fn mul_assign(&mut self, other: $t) { *self *= other }
        }

        forward_ref_op_assign! { impl MulAssign, mul_assign for $t, $t }
    )+)
}

mul_assign_impl! { usize u8 u16 u32 u64 u128 isize i8 i16 i32 i64 i128 f32 f64 }

#[lang = "div_assign"]
pub trait DivAssign<Rhs = Self> {
    /// Performs the `/=` operation.
    ///
    /// # Example
    ///
    /// ```
    /// let mut x: u32 = 12;
    /// x /= 2;
    /// assert_eq!(x, 6);
    /// ```
    fn div_assign(&mut self, rhs: Rhs);
}

macro_rules! div_assign_impl {
    ($($t:ty)+) => ($(
        #[stable(feature = "op_assign_traits", since = "1.8.0")]
        impl DivAssign for $t {
            #[inline]
            fn div_assign(&mut self, other: $t) { *self /= other }
        }

        forward_ref_op_assign! { impl DivAssign, div_assign for $t, $t }
    )+)
}

div_assign_impl! { usize u8 u16 u32 u64 u128 isize i8 i16 i32 i64 i128 f32 f64 }

#[lang = "rem_assign"]
pub trait RemAssign<Rhs = Self> {
    /// Performs the `%=` operation.
    ///
    /// # Example
    ///
    /// ```
    /// let mut x: u32 = 12;
    /// x %= 10;
    /// assert_eq!(x, 2);
    /// ```
    fn rem_assign(&mut self, rhs: Rhs);
}

macro_rules! rem_assign_impl {
    ($($t:ty)+) => ($(
        #[stable(feature = "op_assign_traits", since = "1.8.0")]
        impl RemAssign for $t {
            #[inline]
            fn rem_assign(&mut self, other: $t) { *self %= other }
        }

        forward_ref_op_assign! { impl RemAssign, rem_assign for $t, $t }
    )+)
}

rem_assign_impl! { usize u8 u16 u32 u64 u128 isize i8 i16 i32 i64 i128 f32 f64 }

#[lang = "not"]
pub trait Not {
    /// The resulting type after applying the `!` operator.
    type Output;

    /// Performs the unary `!` operation.
    ///
    /// # Examples
    ///
    /// ```
    /// assert_eq!(!true, false);
    /// assert_eq!(!false, true);
    /// assert_eq!(!1u8, 254);
    /// assert_eq!(!0u8, 255);
    /// ```
    #[must_use]
    fn not(self) -> Self::Output;
}

#[lang = "deref"]
pub trait Deref {
    /// The resulting type after dereferencing.
    type Target: ?Sized;

    /// Dereferences the value.
    #[must_use]
    fn deref(&self) -> &Self::Target;
}

#[lang = "deref_mut"]
pub trait DerefMut: Deref {
    /// Mutably dereferences the value.
    fn deref_mut(&mut self) -> &mut Self::Target;
}

// #[lang = "receiver"]
pub trait Receiver {
    // Empty.
}

impl<T: ?Sized> Receiver for &T {}

impl<T: ?Sized> Receiver for &mut T {}

// #[lang = "drop"]
pub trait Drop {
    /// Executes the destructor for this type.
    ///
    /// This method is called implicitly when the value goes out of scope,
    /// and cannot be called explicitly (this is compiler error [E0040]).
    /// However, the [`mem::drop`] function in the prelude can be
    /// used to call the argument's `Drop` implementation.
    ///
    /// When this method has been called, `self` has not yet been deallocated.
    /// That only happens after the method is over.
    /// If this wasn't the case, `self` would be a dangling reference.
    ///
    /// # Panics
    ///
    /// Given that a [`panic!`] will call `drop` as it unwinds, any [`panic!`]
    /// in a `drop` implementation will likely abort.
    ///
    /// Note that even if this panics, the value is considered to be dropped;
    /// you must not cause `drop` to be called again. This is normally automatically
    /// handled by the compiler, but when using unsafe code, can sometimes occur
    /// unintentionally, particularly when using [`ptr::drop_in_place`].
    ///
    /// [E0040]: ../../error-index.html#E0040
    /// [`panic!`]: crate::panic!
    /// [`mem::drop`]: drop
    /// [`ptr::drop_in_place`]: crate::ptr::drop_in_place
    fn drop(&mut self);
}

#[lang = "index"]
pub trait Index<Idx: ?Sized> {
    /// The returned type after indexing.
    type Output: ?Sized;

    /// Performs the indexing (`container[index]`) operation.
    // #[track_caller]
    fn index(&self, index: Idx) -> &Self::Output;
}

#[lang = "index_mut"]
pub trait IndexMut<Idx: ?Sized>: Index<Idx> {
    /// Performs the mutable indexing (`container[index]`) operation.
    // #[track_caller]
    fn index_mut(&mut self, index: Idx) -> &mut Self::Output;
}

#[lang = "RangeFull"]
pub struct RangeFull;

#[lang = "Range"]
pub struct Range<Idx> {
    /// The lower bound of the range (inclusive).
    pub start: Idx,
    /// The upper bound of the range (exclusive).
    pub end: Idx,
}

#[lang = "RangeInclusive"]
pub struct RangeInclusive<Idx> {
    // Note that the fields here are not public to allow changing the
    // representation in the future; in particular, while we could plausibly
    // expose start/end, modifying them without changing (future/current)
    // private fields may lead to incorrect behavior, so we don't want to
    // support that mode.
    pub(crate) start: Idx,
    pub(crate) end: Idx,

    // This field is:
    //  - `false` upon construction
    //  - `false` when iteration has yielded an element and the iterator is not exhausted
    //  - `true` when iteration has been used to exhaust the iterator
    //
    // This is required to support PartialEq and Hash without a PartialOrd bound or specialization.
    pub(crate) exhausted: bool,
}

#[lang = "RangeToInclusive"]
pub struct RangeToInclusive<Idx> {
    /// The upper bound of the range (inclusive)
    pub end: Idx,
}

pub enum Bound<T> {
    /// An inclusive bound.
    Included(T),
    /// An exclusive bound.
    Excluded(T),
    /// An infinite endpoint. Indicates that there is no bound in this direction.
    Unbounded,
}

impl<T: Clone> Bound<&T> {}

#[lang = "coerce_unsized"]
pub trait CoerceUnsized<T: ?Sized> {
    // Empty.
}
