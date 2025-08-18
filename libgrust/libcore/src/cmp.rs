//! Functionality for ordering and comparison.
//!
//! This module contains various tools for ordering and comparing values. In
//! summary:
//!
//! * [`Eq`] and [`PartialEq`] are traits that allow you to define total and
//!   partial equality between values, respectively. Implementing them overloads
//!   the `==` and `!=` operators.
//! * [`Ord`] and [`PartialOrd`] are traits that allow you to define total and
//!   partial orderings between values, respectively. Implementing them overloads
//!   the `<`, `<=`, `>`, and `>=` operators.
//! * [`Ordering`] is an enum returned by the main functions of [`Ord`] and
//!   [`PartialOrd`], and describes an ordering.
//! * [`Reverse`] is a struct that allows you to easily reverse an ordering.
//! * [`max`] and [`min`] are functions that build off of [`Ord`] and allow you
//!   to find the maximum or minimum of two values.
//!
//! For more details, see the respective documentation of each item in the list.
//!
//! [`max`]: Ord::max
//! [`min`]: Ord::min

#![stable(feature = "rust1", since = "1.0.0")]

use self::Ordering::*;

/// Trait for equality comparisons which are [partial equivalence
/// relations](https://en.wikipedia.org/wiki/Partial_equivalence_relation).
///
/// This trait allows for partial equality, for types that do not have a full
/// equivalence relation. For example, in floating point numbers `NaN != NaN`,
/// so floating point types implement `PartialEq` but not [`Eq`].
///
/// Formally, the equality must be (for all `a`, `b` and `c`):
///
/// - symmetric: `a == b` implies `b == a`; and
/// - transitive: `a == b` and `b == c` implies `a == c`.
///
/// Note that these requirements mean that the trait itself must be implemented
/// symmetrically and transitively: if `T: PartialEq<U>` and `U: PartialEq<V>`
/// then `U: PartialEq<T>` and `T: PartialEq<V>`.
///
/// ## Derivable
///
/// This trait can be used with `#[derive]`. When `derive`d on structs, two
/// instances are equal if all fields are equal, and not equal if any fields
/// are not equal. When `derive`d on enums, each variant is equal to itself
/// and not equal to the other variants.
///
/// ## How can I implement `PartialEq`?
///
/// `PartialEq` only requires the [`eq`] method to be implemented; [`ne`] is defined
/// in terms of it by default. Any manual implementation of [`ne`] *must* respect
/// the rule that [`eq`] is a strict inverse of [`ne`]; that is, `!(a == b)` if and
/// only if `a != b`.
///
/// Implementations of `PartialEq`, [`PartialOrd`], and [`Ord`] *must* agree with
/// each other. It's easy to accidentally make them disagree by deriving some
/// of the traits and manually implementing others.
///
/// An example implementation for a domain in which two books are considered
/// the same book if their ISBN matches, even if the formats differ:
///
/// ```
/// enum BookFormat {
///     Paperback,
///     Hardback,
///     Ebook,
/// }
///
/// struct Book {
///     isbn: i32,
///     format: BookFormat,
/// }
///
/// impl PartialEq for Book {
///     fn eq(&self, other: &Self) -> bool {
///         self.isbn == other.isbn
///     }
/// }
///
/// let b1 = Book { isbn: 3, format: BookFormat::Paperback };
/// let b2 = Book { isbn: 3, format: BookFormat::Ebook };
/// let b3 = Book { isbn: 10, format: BookFormat::Paperback };
///
/// assert!(b1 == b2);
/// assert!(b1 != b3);
/// ```
///
/// ## How can I compare two different types?
///
/// The type you can compare with is controlled by `PartialEq`'s type parameter.
/// For example, let's tweak our previous code a bit:
///
/// ```
/// // The derive implements <BookFormat> == <BookFormat> comparisons
/// #[derive(PartialEq)]
/// enum BookFormat {
///     Paperback,
///     Hardback,
///     Ebook,
/// }
///
/// struct Book {
///     isbn: i32,
///     format: BookFormat,
/// }
///
/// // Implement <Book> == <BookFormat> comparisons
/// impl PartialEq<BookFormat> for Book {
///     fn eq(&self, other: &BookFormat) -> bool {
///         self.format == *other
///     }
/// }
///
/// // Implement <BookFormat> == <Book> comparisons
/// impl PartialEq<Book> for BookFormat {
///     fn eq(&self, other: &Book) -> bool {
///         *self == other.format
///     }
/// }
///
/// let b1 = Book { isbn: 3, format: BookFormat::Paperback };
///
/// assert!(b1 == BookFormat::Paperback);
/// assert!(BookFormat::Ebook != b1);
/// ```
///
/// By changing `impl PartialEq for Book` to `impl PartialEq<BookFormat> for Book`,
/// we allow `BookFormat`s to be compared with `Book`s.
///
/// A comparison like the one above, which ignores some fields of the struct,
/// can be dangerous. It can easily lead to an unintended violation of the
/// requirements for a partial equivalence relation. For example, if we kept
/// the above implementation of `PartialEq<Book>` for `BookFormat` and added an
/// implementation of `PartialEq<Book>` for `Book` (either via a `#[derive]` or
/// via the manual implementation from the first example) then the result would
/// violate transitivity:
///
/// ```should_panic
/// #[derive(PartialEq)]
/// enum BookFormat {
///     Paperback,
///     Hardback,
///     Ebook,
/// }
///
/// #[derive(PartialEq)]
/// struct Book {
///     isbn: i32,
///     format: BookFormat,
/// }
///
/// impl PartialEq<BookFormat> for Book {
///     fn eq(&self, other: &BookFormat) -> bool {
///         self.format == *other
///     }
/// }
///
/// impl PartialEq<Book> for BookFormat {
///     fn eq(&self, other: &Book) -> bool {
///         *self == other.format
///     }
/// }
///
/// fn main() {
///     let b1 = Book { isbn: 1, format: BookFormat::Paperback };
///     let b2 = Book { isbn: 2, format: BookFormat::Paperback };
///
///     assert!(b1 == BookFormat::Paperback);
///     assert!(BookFormat::Paperback == b2);
///
///     // The following should hold by transitivity but doesn't.
///     assert!(b1 == b2); // <-- PANICS
/// }
/// ```
///
/// # Examples
///
/// ```
/// let x: u32 = 0;
/// let y: u32 = 1;
///
/// assert_eq!(x == y, false);
/// assert_eq!(x.eq(&y), false);
/// ```
///
/// [`eq`]: PartialEq::eq
/// [`ne`]: PartialEq::ne
#[lang = "eq"]
#[stable(feature = "rust1", since = "1.0.0")]
#[doc(alias = "==")]
#[doc(alias = "!=")]
#[rustc_on_unimplemented(
    message = "can't compare `{Self}` with `{Rhs}`",
    label = "no implementation for `{Self} == {Rhs}`"
)]
pub trait PartialEq<Rhs: ?Sized = Self> {
    /// This method tests for `self` and `other` values to be equal, and is used
    /// by `==`.
    #[must_use]
    #[stable(feature = "rust1", since = "1.0.0")]
    fn eq(&self, other: &Rhs) -> bool;

    /// This method tests for `!=`.
    #[inline]
    #[must_use]
    #[stable(feature = "rust1", since = "1.0.0")]
    fn ne(&self, other: &Rhs) -> bool {
        !self.eq(other)
    }
}

/// Derive macro generating an impl of the trait `PartialEq`.
#[rustc_builtin_macro]
#[stable(feature = "builtin_macro_prelude", since = "1.38.0")]
#[allow_internal_unstable(core_intrinsics, structural_match)]
pub macro PartialEq($item:item) {
    /* compiler built-in */
}

/// Trait for equality comparisons which are [equivalence relations](
/// https://en.wikipedia.org/wiki/Equivalence_relation).
///
/// This means, that in addition to `a == b` and `a != b` being strict inverses, the equality must
/// be (for all `a`, `b` and `c`):
///
/// - reflexive: `a == a`;
/// - symmetric: `a == b` implies `b == a`; and
/// - transitive: `a == b` and `b == c` implies `a == c`.
///
/// This property cannot be checked by the compiler, and therefore `Eq` implies
/// [`PartialEq`], and has no extra methods.
///
/// ## Derivable
///
/// This trait can be used with `#[derive]`. When `derive`d, because `Eq` has
/// no extra methods, it is only informing the compiler that this is an
/// equivalence relation rather than a partial equivalence relation. Note that
/// the `derive` strategy requires all fields are `Eq`, which isn't
/// always desired.
///
/// ## How can I implement `Eq`?
///
/// If you cannot use the `derive` strategy, specify that your type implements
/// `Eq`, which has no methods:
///
/// ```
/// enum BookFormat { Paperback, Hardback, Ebook }
/// struct Book {
///     isbn: i32,
///     format: BookFormat,
/// }
/// impl PartialEq for Book {
///     fn eq(&self, other: &Self) -> bool {
///         self.isbn == other.isbn
///     }
/// }
/// impl Eq for Book {}
/// ```
#[doc(alias = "==")]
#[doc(alias = "!=")]
#[stable(feature = "rust1", since = "1.0.0")]
pub trait Eq: PartialEq<Self> {
    // this method is used solely by #[deriving] to assert
    // that every component of a type implements #[deriving]
    // itself, the current deriving infrastructure means doing this
    // assertion without using a method on this trait is nearly
    // impossible.
    //
    // This should never be implemented by hand.
    #[doc(hidden)]
    #[inline]
    #[stable(feature = "rust1", since = "1.0.0")]
    fn assert_receiver_is_total_eq(&self) {}
}

/// Derive macro generating an impl of the trait `Eq`.
#[rustc_builtin_macro]
#[stable(feature = "builtin_macro_prelude", since = "1.38.0")]
#[allow_internal_unstable(core_intrinsics, derive_eq, structural_match)]
pub macro Eq($item:item) {
    /* compiler built-in */
}

// FIXME: this struct is used solely by #[derive] to
// assert that every component of a type implements Eq.
//
// This struct should never appear in user code.
#[doc(hidden)]
#[allow(missing_debug_implementations)]
#[unstable(feature = "derive_eq", reason = "deriving hack, should not be public", issue = "none")]
pub struct AssertParamIsEq<T: Eq + ?Sized> {
    _field: crate::marker::PhantomData<T>,
}

/// An `Ordering` is the result of a comparison between two values.
///
/// # Examples
///
/// ```
/// use std::cmp::Ordering;
///
/// let result = 1.cmp(&2);
/// assert_eq!(Ordering::Less, result);
///
/// let result = 1.cmp(&1);
/// assert_eq!(Ordering::Equal, result);
///
/// let result = 2.cmp(&1);
/// assert_eq!(Ordering::Greater, result);
/// ```
#[derive(Clone, Copy, PartialEq, Debug, Hash)]
#[stable(feature = "rust1", since = "1.0.0")]
pub enum Ordering {
    /// An ordering where a compared value is less than another.
    #[stable(feature = "rust1", since = "1.0.0")]
    Less = -1,
    /// An ordering where a compared value is equal to another.
    #[stable(feature = "rust1", since = "1.0.0")]
    Equal = 0,
    /// An ordering where a compared value is greater than another.
    #[stable(feature = "rust1", since = "1.0.0")]
    Greater = 1,
}

impl Ordering {
    /// Reverses the `Ordering`.
    ///
    /// * `Less` becomes `Greater`.
    /// * `Greater` becomes `Less`.
    /// * `Equal` becomes `Equal`.
    ///
    /// # Examples
    ///
    /// Basic behavior:
    ///
    /// ```
    /// use std::cmp::Ordering;
    ///
    /// assert_eq!(Ordering::Less.reverse(), Ordering::Greater);
    /// assert_eq!(Ordering::Equal.reverse(), Ordering::Equal);
    /// assert_eq!(Ordering::Greater.reverse(), Ordering::Less);
    /// ```
    ///
    /// This method can be used to reverse a comparison:
    ///
    /// ```
    /// let data: &mut [_] = &mut [2, 10, 5, 8];
    ///
    /// // sort the array from largest to smallest.
    /// data.sort_by(|a, b| a.cmp(b).reverse());
    ///
    /// let b: &mut [_] = &mut [10, 8, 5, 2];
    /// assert!(data == b);
    /// ```
    #[inline]
    #[must_use]
    #[rustc_const_stable(feature = "const_ordering", since = "1.48.0")]
    #[stable(feature = "rust1", since = "1.0.0")]
    pub const fn reverse(self) -> Ordering {
        match self {
            Less => Greater,
            Equal => Equal,
            Greater => Less,
        }
    }

    /// Chains two orderings.
    ///
    /// Returns `self` when it's not `Equal`. Otherwise returns `other`.
    ///
    /// # Examples
    ///
    /// ```
    /// use std::cmp::Ordering;
    ///
    /// let result = Ordering::Equal.then(Ordering::Less);
    /// assert_eq!(result, Ordering::Less);
    ///
    /// let result = Ordering::Less.then(Ordering::Equal);
    /// assert_eq!(result, Ordering::Less);
    ///
    /// let result = Ordering::Less.then(Ordering::Greater);
    /// assert_eq!(result, Ordering::Less);
    ///
    /// let result = Ordering::Equal.then(Ordering::Equal);
    /// assert_eq!(result, Ordering::Equal);
    ///
    /// let x: (i64, i64, i64) = (1, 2, 7);
    /// let y: (i64, i64, i64) = (1, 5, 3);
    /// let result = x.0.cmp(&y.0).then(x.1.cmp(&y.1)).then(x.2.cmp(&y.2));
    ///
    /// assert_eq!(result, Ordering::Less);
    /// ```
    #[inline]
    #[must_use]
    #[rustc_const_stable(feature = "const_ordering", since = "1.48.0")]
    #[stable(feature = "ordering_chaining", since = "1.17.0")]
    pub const fn then(self, other: Ordering) -> Ordering {
        match self {
            Equal => other,
            _ => self,
        }
    }

    /// Chains the ordering with the given function.
    ///
    /// Returns `self` when it's not `Equal`. Otherwise calls `f` and returns
    /// the result.
    ///
    /// # Examples
    ///
    /// ```
    /// use std::cmp::Ordering;
    ///
    /// let result = Ordering::Equal.then_with(|| Ordering::Less);
    /// assert_eq!(result, Ordering::Less);
    ///
    /// let result = Ordering::Less.then_with(|| Ordering::Equal);
    /// assert_eq!(result, Ordering::Less);
    ///
    /// let result = Ordering::Less.then_with(|| Ordering::Greater);
    /// assert_eq!(result, Ordering::Less);
    ///
    /// let result = Ordering::Equal.then_with(|| Ordering::Equal);
    /// assert_eq!(result, Ordering::Equal);
    ///
    /// let x: (i64, i64, i64) = (1, 2, 7);
    /// let y: (i64, i64, i64)  = (1, 5, 3);
    /// let result = x.0.cmp(&y.0).then_with(|| x.1.cmp(&y.1)).then_with(|| x.2.cmp(&y.2));
    ///
    /// assert_eq!(result, Ordering::Less);
    /// ```
    #[inline]
    #[must_use]
    #[stable(feature = "ordering_chaining", since = "1.17.0")]
    pub fn then_with<F: FnOnce() -> Ordering>(self, f: F) -> Ordering {
        match self {
            Equal => f(),
            _ => self,
        }
    }
}

/// A helper struct for reverse ordering.
///
/// This struct is a helper to be used with functions like [`Vec::sort_by_key`] and
/// can be used to reverse order a part of a key.
///
/// [`Vec::sort_by_key`]: ../../std/vec/struct.Vec.html#method.sort_by_key
///
/// # Examples
///
/// ```
/// use std::cmp::Reverse;
///
/// let mut v = vec![1, 2, 3, 4, 5, 6];
/// v.sort_by_key(|&num| (num > 3, Reverse(num)));
/// assert_eq!(v, vec![3, 2, 1, 6, 5, 4]);
/// ```
#[derive(PartialEq, Eq, Debug, Copy, Clone, Default, Hash)]
#[stable(feature = "reverse_cmp_key", since = "1.19.0")]
pub struct Reverse<T>(#[stable(feature = "reverse_cmp_key", since = "1.19.0")] pub T);

#[stable(feature = "reverse_cmp_key", since = "1.19.0")]
impl<T: PartialOrd> PartialOrd for Reverse<T> {
    #[inline]
    fn partial_cmp(&self, other: &Reverse<T>) -> Option<Ordering> {
        other.0.partial_cmp(&self.0)
    }

    #[inline]
    fn lt(&self, other: &Self) -> bool {
        other.0 < self.0
    }
    #[inline]
    fn le(&self, other: &Self) -> bool {
        other.0 <= self.0
    }
    #[inline]
    fn gt(&self, other: &Self) -> bool {
        other.0 > self.0
    }
    #[inline]
    fn ge(&self, other: &Self) -> bool {
        other.0 >= self.0
    }
}

#[stable(feature = "reverse_cmp_key", since = "1.19.0")]
impl<T: Ord> Ord for Reverse<T> {
    #[inline]
    fn cmp(&self, other: &Reverse<T>) -> Ordering {
        other.0.cmp(&self.0)
    }
}

/// Trait for types that form a [total order](https://en.wikipedia.org/wiki/Total_order).
///
/// An order is a total order if it is (for all `a`, `b` and `c`):
///
/// - total and asymmetric: exactly one of `a < b`, `a == b` or `a > b` is true; and
/// - transitive, `a < b` and `b < c` implies `a < c`. The same must hold for both `==` and `>`.
///
/// ## Derivable
///
/// This trait can be used with `#[derive]`. When `derive`d on structs, it will produce a
/// [lexicographic](https://en.wikipedia.org/wiki/Lexicographic_order) ordering based on the top-to-bottom declaration order of the struct's members.
/// When `derive`d on enums, variants are ordered by their top-to-bottom discriminant order.
///
/// ## Lexicographical comparison
///
/// Lexicographical comparison is an operation with the following properties:
///  - Two sequences are compared element by element.
///  - The first mismatching element defines which sequence is lexicographically less or greater than the other.
///  - If one sequence is a prefix of another, the shorter sequence is lexicographically less than the other.
///  - If two sequence have equivalent elements and are of the same length, then the sequences are lexicographically equal.
///  - An empty sequence is lexicographically less than any non-empty sequence.
///  - Two empty sequences are lexicographically equal.
///
/// ## How can I implement `Ord`?
///
/// `Ord` requires that the type also be [`PartialOrd`] and [`Eq`] (which requires [`PartialEq`]).
///
/// Then you must define an implementation for [`cmp`]. You may find it useful to use
/// [`cmp`] on your type's fields.
///
/// Implementations of [`PartialEq`], [`PartialOrd`], and `Ord` *must*
/// agree with each other. That is, `a.cmp(b) == Ordering::Equal` if
/// and only if `a == b` and `Some(a.cmp(b)) == a.partial_cmp(b)` for
/// all `a` and `b`. It's easy to accidentally make them disagree by
/// deriving some of the traits and manually implementing others.
///
/// Here's an example where you want to sort people by height only, disregarding `id`
/// and `name`:
///
/// ```
/// use std::cmp::Ordering;
///
/// #[derive(Eq)]
/// struct Person {
///     id: u32,
///     name: String,
///     height: u32,
/// }
///
/// impl Ord for Person {
///     fn cmp(&self, other: &Self) -> Ordering {
///         self.height.cmp(&other.height)
///     }
/// }
///
/// impl PartialOrd for Person {
///     fn partial_cmp(&self, other: &Self) -> Option<Ordering> {
///         Some(self.cmp(other))
///     }
/// }
///
/// impl PartialEq for Person {
///     fn eq(&self, other: &Self) -> bool {
///         self.height == other.height
///     }
/// }
/// ```
///
/// [`cmp`]: Ord::cmp
#[doc(alias = "<")]
#[doc(alias = ">")]
#[doc(alias = "<=")]
#[doc(alias = ">=")]
#[stable(feature = "rust1", since = "1.0.0")]
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
    #[must_use]
    #[stable(feature = "rust1", since = "1.0.0")]
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
    #[stable(feature = "ord_max_min", since = "1.21.0")]
    #[inline]
    #[must_use]
    fn max(self, other: Self) -> Self
    where
        Self: Sized,
    {
        max_by(self, other, Ord::cmp)
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
    #[stable(feature = "ord_max_min", since = "1.21.0")]
    #[inline]
    #[must_use]
    fn min(self, other: Self) -> Self
    where
        Self: Sized,
    {
        min_by(self, other, Ord::cmp)
    }

    /// Restrict a value to a certain interval.
    ///
    /// Returns `max` if `self` is greater than `max`, and `min` if `self` is
    /// less than `min`. Otherwise this returns `self`.
    ///
    /// # Panics
    ///
    /// Panics if `min > max`.
    ///
    /// # Examples
    ///
    /// ```
    /// #![feature(clamp)]
    ///
    /// assert!((-3).clamp(-2, 1) == -2);
    /// assert!(0.clamp(-2, 1) == 0);
    /// assert!(2.clamp(-2, 1) == 1);
    /// ```
    #[must_use]
    #[unstable(feature = "clamp", issue = "44095")]
    fn clamp(self, min: Self, max: Self) -> Self
    where
        Self: Sized,
    {
        assert!(min <= max);
        if self < min {
            min
        } else if self > max {
            max
        } else {
            self
        }
    }
}

/// Derive macro generating an impl of the trait `Ord`.
#[rustc_builtin_macro]
#[stable(feature = "builtin_macro_prelude", since = "1.38.0")]
#[allow_internal_unstable(core_intrinsics)]
pub macro Ord($item:item) {
    /* compiler built-in */
}

#[stable(feature = "rust1", since = "1.0.0")]
impl Eq for Ordering {}

#[stable(feature = "rust1", since = "1.0.0")]
impl Ord for Ordering {
    #[inline]
    fn cmp(&self, other: &Ordering) -> Ordering {
        (*self as i32).cmp(&(*other as i32))
    }
}

#[stable(feature = "rust1", since = "1.0.0")]
impl PartialOrd for Ordering {
    #[inline]
    fn partial_cmp(&self, other: &Ordering) -> Option<Ordering> {
        (*self as i32).partial_cmp(&(*other as i32))
    }
}

/// Trait for values that can be compared for a sort-order.
///
/// The comparison must satisfy, for all `a`, `b` and `c`:
///
/// - asymmetry: if `a < b` then `!(a > b)`, as well as `a > b` implying `!(a < b)`; and
/// - transitivity: `a < b` and `b < c` implies `a < c`. The same must hold for both `==` and `>`.
///
/// Note that these requirements mean that the trait itself must be implemented symmetrically and
/// transitively: if `T: PartialOrd<U>` and `U: PartialOrd<V>` then `U: PartialOrd<T>` and `T:
/// PartialOrd<V>`.
///
/// ## Derivable
///
/// This trait can be used with `#[derive]`. When `derive`d on structs, it will produce a
/// lexicographic ordering based on the top-to-bottom declaration order of the struct's members.
/// When `derive`d on enums, variants are ordered by their top-to-bottom discriminant order.
///
/// ## How can I implement `PartialOrd`?
///
/// `PartialOrd` only requires implementation of the [`partial_cmp`] method, with the others
/// generated from default implementations.
///
/// However it remains possible to implement the others separately for types which do not have a
/// total order. For example, for floating point numbers, `NaN < 0 == false` and `NaN >= 0 ==
/// false` (cf. IEEE 754-2008 section 5.11).
///
/// `PartialOrd` requires your type to be [`PartialEq`].
///
/// Implementations of [`PartialEq`], `PartialOrd`, and [`Ord`] *must* agree with each other. It's
/// easy to accidentally make them disagree by deriving some of the traits and manually
/// implementing others.
///
/// If your type is [`Ord`], you can implement [`partial_cmp`] by using [`cmp`]:
///
/// ```
/// use std::cmp::Ordering;
///
/// #[derive(Eq)]
/// struct Person {
///     id: u32,
///     name: String,
///     height: u32,
/// }
///
/// impl PartialOrd for Person {
///     fn partial_cmp(&self, other: &Self) -> Option<Ordering> {
///         Some(self.cmp(other))
///     }
/// }
///
/// impl Ord for Person {
///     fn cmp(&self, other: &Self) -> Ordering {
///         self.height.cmp(&other.height)
///     }
/// }
///
/// impl PartialEq for Person {
///     fn eq(&self, other: &Self) -> bool {
///         self.height == other.height
///     }
/// }
/// ```
///
/// You may also find it useful to use [`partial_cmp`] on your type's fields. Here
/// is an example of `Person` types who have a floating-point `height` field that
/// is the only field to be used for sorting:
///
/// ```
/// use std::cmp::Ordering;
///
/// struct Person {
///     id: u32,
///     name: String,
///     height: f64,
/// }
///
/// impl PartialOrd for Person {
///     fn partial_cmp(&self, other: &Self) -> Option<Ordering> {
///         self.height.partial_cmp(&other.height)
///     }
/// }
///
/// impl PartialEq for Person {
///     fn eq(&self, other: &Self) -> bool {
///         self.height == other.height
///     }
/// }
/// ```
///
/// # Examples
///
/// ```
/// let x : u32 = 0;
/// let y : u32 = 1;
///
/// assert_eq!(x < y, true);
/// assert_eq!(x.lt(&y), true);
/// ```
///
/// [`partial_cmp`]: PartialOrd::partial_cmp
/// [`cmp`]: Ord::cmp
#[lang = "partial_ord"]
#[stable(feature = "rust1", since = "1.0.0")]
#[doc(alias = ">")]
#[doc(alias = "<")]
#[doc(alias = "<=")]
#[doc(alias = ">=")]
#[rustc_on_unimplemented(
    message = "can't compare `{Self}` with `{Rhs}`",
    label = "no implementation for `{Self} < {Rhs}` and `{Self} > {Rhs}`"
)]
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
    #[must_use]
    #[stable(feature = "rust1", since = "1.0.0")]
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
    #[inline]
    #[must_use]
    #[stable(feature = "rust1", since = "1.0.0")]
    fn lt(&self, other: &Rhs) -> bool {
        matches!(self.partial_cmp(other), Some(Less))
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
    #[inline]
    #[must_use]
    #[stable(feature = "rust1", since = "1.0.0")]
    fn le(&self, other: &Rhs) -> bool {
        matches!(self.partial_cmp(other), Some(Less | Equal))
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
    #[inline]
    #[must_use]
    #[stable(feature = "rust1", since = "1.0.0")]
    fn gt(&self, other: &Rhs) -> bool {
        matches!(self.partial_cmp(other), Some(Greater))
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
    #[inline]
    #[must_use]
    #[stable(feature = "rust1", since = "1.0.0")]
    fn ge(&self, other: &Rhs) -> bool {
        matches!(self.partial_cmp(other), Some(Greater | Equal))
    }
}

/// Derive macro generating an impl of the trait `PartialOrd`.
#[rustc_builtin_macro]
#[stable(feature = "builtin_macro_prelude", since = "1.38.0")]
#[allow_internal_unstable(core_intrinsics)]
pub macro PartialOrd($item:item) {
    /* compiler built-in */
}

/// Compares and returns the minimum of two values.
///
/// Returns the first argument if the comparison determines them to be equal.
///
/// Internally uses an alias to [`Ord::min`].
///
/// # Examples
///
/// ```
/// use std::cmp;
///
/// assert_eq!(1, cmp::min(1, 2));
/// assert_eq!(2, cmp::min(2, 2));
/// ```
#[inline]
#[must_use]
#[stable(feature = "rust1", since = "1.0.0")]
pub fn min<T: Ord>(v1: T, v2: T) -> T {
    v1.min(v2)
}

/// Returns the minimum of two values with respect to the specified comparison function.
///
/// Returns the first argument if the comparison determines them to be equal.
///
/// # Examples
///
/// ```
/// #![feature(cmp_min_max_by)]
///
/// use std::cmp;
///
/// assert_eq!(cmp::min_by(-2, 1, |x: &i32, y: &i32| x.abs().cmp(&y.abs())), 1);
/// assert_eq!(cmp::min_by(-2, 2, |x: &i32, y: &i32| x.abs().cmp(&y.abs())), -2);
/// ```
#[inline]
#[must_use]
#[unstable(feature = "cmp_min_max_by", issue = "64460")]
pub fn min_by<T, F: FnOnce(&T, &T) -> Ordering>(v1: T, v2: T, compare: F) -> T {
    match compare(&v1, &v2) {
        Ordering::Less | Ordering::Equal => v1,
        Ordering::Greater => v2,
    }
}

/// Returns the element that gives the minimum value from the specified function.
///
/// Returns the first argument if the comparison determines them to be equal.
///
/// # Examples
///
/// ```
/// #![feature(cmp_min_max_by)]
///
/// use std::cmp;
///
/// assert_eq!(cmp::min_by_key(-2, 1, |x: &i32| x.abs()), 1);
/// assert_eq!(cmp::min_by_key(-2, 2, |x: &i32| x.abs()), -2);
/// ```
#[inline]
#[must_use]
#[unstable(feature = "cmp_min_max_by", issue = "64460")]
pub fn min_by_key<T, F: FnMut(&T) -> K, K: Ord>(v1: T, v2: T, mut f: F) -> T {
    min_by(v1, v2, |v1, v2| f(v1).cmp(&f(v2)))
}

/// Compares and returns the maximum of two values.
///
/// Returns the second argument if the comparison determines them to be equal.
///
/// Internally uses an alias to [`Ord::max`].
///
/// # Examples
///
/// ```
/// use std::cmp;
///
/// assert_eq!(2, cmp::max(1, 2));
/// assert_eq!(2, cmp::max(2, 2));
/// ```
#[inline]
#[must_use]
#[stable(feature = "rust1", since = "1.0.0")]
pub fn max<T: Ord>(v1: T, v2: T) -> T {
    v1.max(v2)
}

/// Returns the maximum of two values with respect to the specified comparison function.
///
/// Returns the second argument if the comparison determines them to be equal.
///
/// # Examples
///
/// ```
/// #![feature(cmp_min_max_by)]
///
/// use std::cmp;
///
/// assert_eq!(cmp::max_by(-2, 1, |x: &i32, y: &i32| x.abs().cmp(&y.abs())), -2);
/// assert_eq!(cmp::max_by(-2, 2, |x: &i32, y: &i32| x.abs().cmp(&y.abs())), 2);
/// ```
#[inline]
#[must_use]
#[unstable(feature = "cmp_min_max_by", issue = "64460")]
pub fn max_by<T, F: FnOnce(&T, &T) -> Ordering>(v1: T, v2: T, compare: F) -> T {
    match compare(&v1, &v2) {
        Ordering::Less | Ordering::Equal => v2,
        Ordering::Greater => v1,
    }
}

/// Returns the element that gives the maximum value from the specified function.
///
/// Returns the second argument if the comparison determines them to be equal.
///
/// # Examples
///
/// ```
/// #![feature(cmp_min_max_by)]
///
/// use std::cmp;
///
/// assert_eq!(cmp::max_by_key(-2, 1, |x: &i32| x.abs()), -2);
/// assert_eq!(cmp::max_by_key(-2, 2, |x: &i32| x.abs()), 2);
/// ```
#[inline]
#[must_use]
#[unstable(feature = "cmp_min_max_by", issue = "64460")]
pub fn max_by_key<T, F: FnMut(&T) -> K, K: Ord>(v1: T, v2: T, mut f: F) -> T {
    max_by(v1, v2, |v1, v2| f(v1).cmp(&f(v2)))
}

// Implementation of PartialEq, Eq, PartialOrd and Ord for primitive types
mod impls {
    use crate::cmp::Ordering::{self, Equal, Greater, Less};
    use crate::hint::unreachable_unchecked;

    macro_rules! partial_eq_impl {
        ($($t:ty)*) => ($(
            #[stable(feature = "rust1", since = "1.0.0")]
            impl PartialEq for $t {
                #[inline]
                fn eq(&self, other: &$t) -> bool { (*self) == (*other) }
                #[inline]
                fn ne(&self, other: &$t) -> bool { (*self) != (*other) }
            }
        )*)
    }

    #[stable(feature = "rust1", since = "1.0.0")]
    impl PartialEq for () {
        #[inline]
        fn eq(&self, _other: &()) -> bool {
            true
        }
        #[inline]
        fn ne(&self, _other: &()) -> bool {
            false
        }
    }

    partial_eq_impl! {
        bool char usize u8 u16 u32 u64 u128 isize i8 i16 i32 i64 i128 f32 f64
    }

    macro_rules! eq_impl {
        ($($t:ty)*) => ($(
            #[stable(feature = "rust1", since = "1.0.0")]
            impl Eq for $t {}
        )*)
    }

    eq_impl! { () bool char usize u8 u16 u32 u64 u128 isize i8 i16 i32 i64 i128 }

    macro_rules! partial_ord_impl {
        ($($t:ty)*) => ($(
            #[stable(feature = "rust1", since = "1.0.0")]
            impl PartialOrd for $t {
                #[inline]
                fn partial_cmp(&self, other: &$t) -> Option<Ordering> {
                    match (self <= other, self >= other) {
                        (false, false) => None,
                        (false, true) => Some(Greater),
                        (true, false) => Some(Less),
                        (true, true) => Some(Equal),
                    }
                }
                #[inline]
                fn lt(&self, other: &$t) -> bool { (*self) < (*other) }
                #[inline]
                fn le(&self, other: &$t) -> bool { (*self) <= (*other) }
                #[inline]
                fn ge(&self, other: &$t) -> bool { (*self) >= (*other) }
                #[inline]
                fn gt(&self, other: &$t) -> bool { (*self) > (*other) }
            }
        )*)
    }

    #[stable(feature = "rust1", since = "1.0.0")]
    impl PartialOrd for () {
        #[inline]
        fn partial_cmp(&self, _: &()) -> Option<Ordering> {
            Some(Equal)
        }
    }

    #[stable(feature = "rust1", since = "1.0.0")]
    impl PartialOrd for bool {
        #[inline]
        fn partial_cmp(&self, other: &bool) -> Option<Ordering> {
            (*self as u8).partial_cmp(&(*other as u8))
        }
    }

    partial_ord_impl! { f32 f64 }

    macro_rules! ord_impl {
        ($($t:ty)*) => ($(
            #[stable(feature = "rust1", since = "1.0.0")]
            impl PartialOrd for $t {
                #[inline]
                fn partial_cmp(&self, other: &$t) -> Option<Ordering> {
                    Some(self.cmp(other))
                }
                #[inline]
                fn lt(&self, other: &$t) -> bool { (*self) < (*other) }
                #[inline]
                fn le(&self, other: &$t) -> bool { (*self) <= (*other) }
                #[inline]
                fn ge(&self, other: &$t) -> bool { (*self) >= (*other) }
                #[inline]
                fn gt(&self, other: &$t) -> bool { (*self) > (*other) }
            }

            #[stable(feature = "rust1", since = "1.0.0")]
            impl Ord for $t {
                #[inline]
                fn cmp(&self, other: &$t) -> Ordering {
                    // The order here is important to generate more optimal assembly.
                    // See <https://github.com/rust-lang/rust/issues/63758> for more info.
                    if *self < *other { Less }
                    else if *self == *other { Equal }
                    else { Greater }
                }
            }
        )*)
    }

    #[stable(feature = "rust1", since = "1.0.0")]
    impl Ord for () {
        #[inline]
        fn cmp(&self, _other: &()) -> Ordering {
            Equal
        }
    }

    #[stable(feature = "rust1", since = "1.0.0")]
    impl Ord for bool {
        #[inline]
        fn cmp(&self, other: &bool) -> Ordering {
            // Casting to i8's and converting the difference to an Ordering generates
            // more optimal assembly.
            // See <https://github.com/rust-lang/rust/issues/66780> for more info.
            match (*self as i8) - (*other as i8) {
                -1 => Less,
                0 => Equal,
                1 => Greater,
                // SAFETY: bool as i8 returns 0 or 1, so the difference can't be anything else
                _ => unsafe { unreachable_unchecked() },
            }
        }
    }

    ord_impl! { char usize u8 u16 u32 u64 u128 isize i8 i16 i32 i64 i128 }

    #[unstable(feature = "never_type", issue = "35121")]
    impl PartialEq for ! {
        fn eq(&self, _: &!) -> bool {
            *self
        }
    }

    #[unstable(feature = "never_type", issue = "35121")]
    impl Eq for ! {}

    #[unstable(feature = "never_type", issue = "35121")]
    impl PartialOrd for ! {
        fn partial_cmp(&self, _: &!) -> Option<Ordering> {
            *self
        }
    }

    #[unstable(feature = "never_type", issue = "35121")]
    impl Ord for ! {
        fn cmp(&self, _: &!) -> Ordering {
            *self
        }
    }

    // & pointers

    #[stable(feature = "rust1", since = "1.0.0")]
    impl<A: ?Sized, B: ?Sized> PartialEq<&B> for &A
    where
        A: PartialEq<B>,
    {
        #[inline]
        fn eq(&self, other: &&B) -> bool {
            PartialEq::eq(*self, *other)
        }
        #[inline]
        fn ne(&self, other: &&B) -> bool {
            PartialEq::ne(*self, *other)
        }
    }
    #[stable(feature = "rust1", since = "1.0.0")]
    impl<A: ?Sized, B: ?Sized> PartialOrd<&B> for &A
    where
        A: PartialOrd<B>,
    {
        #[inline]
        fn partial_cmp(&self, other: &&B) -> Option<Ordering> {
            PartialOrd::partial_cmp(*self, *other)
        }
        #[inline]
        fn lt(&self, other: &&B) -> bool {
            PartialOrd::lt(*self, *other)
        }
        #[inline]
        fn le(&self, other: &&B) -> bool {
            PartialOrd::le(*self, *other)
        }
        #[inline]
        fn gt(&self, other: &&B) -> bool {
            PartialOrd::gt(*self, *other)
        }
        #[inline]
        fn ge(&self, other: &&B) -> bool {
            PartialOrd::ge(*self, *other)
        }
    }
    #[stable(feature = "rust1", since = "1.0.0")]
    impl<A: ?Sized> Ord for &A
    where
        A: Ord,
    {
        #[inline]
        fn cmp(&self, other: &Self) -> Ordering {
            Ord::cmp(*self, *other)
        }
    }
    #[stable(feature = "rust1", since = "1.0.0")]
    impl<A: ?Sized> Eq for &A where A: Eq {}

    // &mut pointers

    #[stable(feature = "rust1", since = "1.0.0")]
    impl<A: ?Sized, B: ?Sized> PartialEq<&mut B> for &mut A
    where
        A: PartialEq<B>,
    {
        #[inline]
        fn eq(&self, other: &&mut B) -> bool {
            PartialEq::eq(*self, *other)
        }
        #[inline]
        fn ne(&self, other: &&mut B) -> bool {
            PartialEq::ne(*self, *other)
        }
    }
    #[stable(feature = "rust1", since = "1.0.0")]
    impl<A: ?Sized, B: ?Sized> PartialOrd<&mut B> for &mut A
    where
        A: PartialOrd<B>,
    {
        #[inline]
        fn partial_cmp(&self, other: &&mut B) -> Option<Ordering> {
            PartialOrd::partial_cmp(*self, *other)
        }
        #[inline]
        fn lt(&self, other: &&mut B) -> bool {
            PartialOrd::lt(*self, *other)
        }
        #[inline]
        fn le(&self, other: &&mut B) -> bool {
            PartialOrd::le(*self, *other)
        }
        #[inline]
        fn gt(&self, other: &&mut B) -> bool {
            PartialOrd::gt(*self, *other)
        }
        #[inline]
        fn ge(&self, other: &&mut B) -> bool {
            PartialOrd::ge(*self, *other)
        }
    }
    #[stable(feature = "rust1", since = "1.0.0")]
    impl<A: ?Sized> Ord for &mut A
    where
        A: Ord,
    {
        #[inline]
        fn cmp(&self, other: &Self) -> Ordering {
            Ord::cmp(*self, *other)
        }
    }
    #[stable(feature = "rust1", since = "1.0.0")]
    impl<A: ?Sized> Eq for &mut A where A: Eq {}

    #[stable(feature = "rust1", since = "1.0.0")]
    impl<A: ?Sized, B: ?Sized> PartialEq<&mut B> for &A
    where
        A: PartialEq<B>,
    {
        #[inline]
        fn eq(&self, other: &&mut B) -> bool {
            PartialEq::eq(*self, *other)
        }
        #[inline]
        fn ne(&self, other: &&mut B) -> bool {
            PartialEq::ne(*self, *other)
        }
    }

    #[stable(feature = "rust1", since = "1.0.0")]
    impl<A: ?Sized, B: ?Sized> PartialEq<&B> for &mut A
    where
        A: PartialEq<B>,
    {
        #[inline]
        fn eq(&self, other: &&B) -> bool {
            PartialEq::eq(*self, *other)
        }
        #[inline]
        fn ne(&self, other: &&B) -> bool {
            PartialEq::ne(*self, *other)
        }
    }
}
