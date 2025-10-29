macro_rules! int_impl {
    ($SelfT:ty, $ActualT:ident, $UnsignedT:ty, $BITS:expr, $Min:expr, $Max:expr, $Feature:expr,
     $EndFeature:expr, $rot:expr, $rot_op:expr, $rot_result:expr, $swap_op:expr, $swapped:expr,
     $reversed:expr, $le_bytes:expr, $be_bytes:expr,
     $to_xe_bytes_doc:expr, $from_xe_bytes_doc:expr) => {
        doc_comment! {
            concat!("The smallest value that can be represented by this integer type.

# Examples

Basic usage:

```
", $Feature, "assert_eq!(", stringify!($SelfT), "::MIN, ", stringify!($Min), ");",
$EndFeature, "
```"),
            #[stable(feature = "assoc_int_consts", since = "1.43.0")]
            pub const MIN: Self = !0 ^ ((!0 as $UnsignedT) >> 1) as Self;
        }

        doc_comment! {
            concat!("The largest value that can be represented by this integer type.

# Examples

Basic usage:

```
", $Feature, "assert_eq!(", stringify!($SelfT), "::MAX, ", stringify!($Max), ");",
$EndFeature, "
```"),
            #[stable(feature = "assoc_int_consts", since = "1.43.0")]
            pub const MAX: Self = !Self::MIN;
        }

        doc_comment! {
            concat!("The size of this integer type in bits.

# Examples

```
", $Feature, "#![feature(int_bits_const)]
assert_eq!(", stringify!($SelfT), "::BITS, ", stringify!($BITS), ");",
$EndFeature, "
```"),
            #[unstable(feature = "int_bits_const", issue = "76904")]
            pub const BITS: u32 = $BITS;
        }

        doc_comment! {
            concat!("Converts a string slice in a given base to an integer.

The string is expected to be an optional `+` or `-` sign followed by digits.
Leading and trailing whitespace represent an error. Digits are a subset of these characters,
depending on `radix`:

 * `0-9`
 * `a-z`
 * `A-Z`

# Panics

This function panics if `radix` is not in the range from 2 to 36.

# Examples

Basic usage:

```
", $Feature, "assert_eq!(", stringify!($SelfT), "::from_str_radix(\"A\", 16), Ok(10));",
$EndFeature, "
```"),
            #[stable(feature = "rust1", since = "1.0.0")]
            pub fn from_str_radix(src: &str, radix: u32) -> Result<Self, ParseIntError> {
                from_str_radix(src, radix)
            }
        }

        doc_comment! {
            concat!("Returns the number of ones in the binary representation of `self`.

# Examples

Basic usage:

```
", $Feature, "let n = 0b100_0000", stringify!($SelfT), ";

assert_eq!(n.count_ones(), 1);",
$EndFeature, "
```
"),
            #[stable(feature = "rust1", since = "1.0.0")]
            #[rustc_const_stable(feature = "const_int_methods", since = "1.32.0")]
            #[inline]
            pub const fn count_ones(self) -> u32 { (self as $UnsignedT).count_ones() }
        }

        doc_comment! {
            concat!("Returns the number of zeros in the binary representation of `self`.

# Examples

Basic usage:

```
", $Feature, "assert_eq!(", stringify!($SelfT), "::MAX.count_zeros(), 1);", $EndFeature, "
```"),
            #[stable(feature = "rust1", since = "1.0.0")]
            #[rustc_const_stable(feature = "const_int_methods", since = "1.32.0")]
            #[inline]
            pub const fn count_zeros(self) -> u32 {
                (!self).count_ones()
            }
        }

        doc_comment! {
            concat!("Returns the number of leading zeros in the binary representation of `self`.

# Examples

Basic usage:

```
", $Feature, "let n = -1", stringify!($SelfT), ";

assert_eq!(n.leading_zeros(), 0);",
$EndFeature, "
```"),
            #[stable(feature = "rust1", since = "1.0.0")]
            #[rustc_const_stable(feature = "const_int_methods", since = "1.32.0")]
            #[inline]
            pub const fn leading_zeros(self) -> u32 {
                (self as $UnsignedT).leading_zeros()
            }
        }

        doc_comment! {
            concat!("Returns the number of trailing zeros in the binary representation of `self`.

# Examples

Basic usage:

```
", $Feature, "let n = -4", stringify!($SelfT), ";

assert_eq!(n.trailing_zeros(), 2);",
$EndFeature, "
```"),
            #[stable(feature = "rust1", since = "1.0.0")]
            #[rustc_const_stable(feature = "const_int_methods", since = "1.32.0")]
            #[inline]
            pub const fn trailing_zeros(self) -> u32 {
                (self as $UnsignedT).trailing_zeros()
            }
        }

        doc_comment! {
            concat!("Returns the number of leading ones in the binary representation of `self`.

# Examples

Basic usage:

```
", $Feature, "let n = -1", stringify!($SelfT), ";

assert_eq!(n.leading_ones(), ", stringify!($BITS), ");",
$EndFeature, "
```"),
            #[stable(feature = "leading_trailing_ones", since = "1.46.0")]
            #[rustc_const_stable(feature = "leading_trailing_ones", since = "1.46.0")]
            #[inline]
            pub const fn leading_ones(self) -> u32 {
                (self as $UnsignedT).leading_ones()
            }
        }

        doc_comment! {
            concat!("Returns the number of trailing ones in the binary representation of `self`.

# Examples

Basic usage:

```
", $Feature, "let n = 3", stringify!($SelfT), ";

assert_eq!(n.trailing_ones(), 2);",
$EndFeature, "
```"),
            #[stable(feature = "leading_trailing_ones", since = "1.46.0")]
            #[rustc_const_stable(feature = "leading_trailing_ones", since = "1.46.0")]
            #[inline]
            pub const fn trailing_ones(self) -> u32 {
                (self as $UnsignedT).trailing_ones()
            }
        }

        doc_comment! {
            concat!("Shifts the bits to the left by a specified amount, `n`,
wrapping the truncated bits to the end of the resulting integer.

Please note this isn't the same operation as the `<<` shifting operator!

# Examples

Basic usage:

```
let n = ", $rot_op, stringify!($SelfT), ";
let m = ", $rot_result, ";

assert_eq!(n.rotate_left(", $rot, "), m);
```"),
            #[stable(feature = "rust1", since = "1.0.0")]
            #[rustc_const_stable(feature = "const_int_methods", since = "1.32.0")]
            #[must_use = "this returns the result of the operation, \
                          without modifying the original"]
            #[inline]
            pub const fn rotate_left(self, n: u32) -> Self {
                (self as $UnsignedT).rotate_left(n) as Self
            }
        }

        doc_comment! {
            concat!("Shifts the bits to the right by a specified amount, `n`,
wrapping the truncated bits to the beginning of the resulting
integer.

Please note this isn't the same operation as the `>>` shifting operator!

# Examples

Basic usage:

```
let n = ", $rot_result, stringify!($SelfT), ";
let m = ", $rot_op, ";

assert_eq!(n.rotate_right(", $rot, "), m);
```"),
            #[stable(feature = "rust1", since = "1.0.0")]
            #[rustc_const_stable(feature = "const_int_methods", since = "1.32.0")]
            #[must_use = "this returns the result of the operation, \
                          without modifying the original"]
            #[inline]
            pub const fn rotate_right(self, n: u32) -> Self {
                (self as $UnsignedT).rotate_right(n) as Self
            }
        }

        doc_comment! {
            concat!("Reverses the byte order of the integer.

# Examples

Basic usage:

```
let n = ", $swap_op, stringify!($SelfT), ";

let m = n.swap_bytes();

assert_eq!(m, ", $swapped, ");
```"),
            #[stable(feature = "rust1", since = "1.0.0")]
            #[rustc_const_stable(feature = "const_int_methods", since = "1.32.0")]
            #[inline]
            pub const fn swap_bytes(self) -> Self {
                (self as $UnsignedT).swap_bytes() as Self
            }
        }

        doc_comment! {
            concat!("Reverses the order of bits in the integer. The least significant bit becomes the most significant bit,
                second least-significant bit becomes second most-significant bit, etc.

# Examples

Basic usage:

```
let n = ", $swap_op, stringify!($SelfT), ";
let m = n.reverse_bits();

assert_eq!(m, ", $reversed, ");
assert_eq!(0, 0", stringify!($SelfT), ".reverse_bits());
```"),
            #[stable(feature = "reverse_bits", since = "1.37.0")]
            #[rustc_const_stable(feature = "const_int_methods", since = "1.32.0")]
            #[inline]
            #[must_use]
            pub const fn reverse_bits(self) -> Self {
                (self as $UnsignedT).reverse_bits() as Self
            }
        }

        doc_comment! {
            concat!("Converts an integer from big endian to the target's endianness.

On big endian this is a no-op. On little endian the bytes are swapped.

# Examples

Basic usage:

```
", $Feature, "let n = 0x1A", stringify!($SelfT), ";

if cfg!(target_endian = \"big\") {
    assert_eq!(", stringify!($SelfT), "::from_be(n), n)
} else {
    assert_eq!(", stringify!($SelfT), "::from_be(n), n.swap_bytes())
}",
$EndFeature, "
```"),
            #[stable(feature = "rust1", since = "1.0.0")]
            #[rustc_const_stable(feature = "const_int_conversions", since = "1.32.0")]
            #[inline]
            pub const fn from_be(x: Self) -> Self {
                #[cfg(target_endian = "big")]
                {
                    x
                }
                #[cfg(not(target_endian = "big"))]
                {
                    x.swap_bytes()
                }
            }
        }

        doc_comment! {
            concat!("Converts an integer from little endian to the target's endianness.

On little endian this is a no-op. On big endian the bytes are swapped.

# Examples

Basic usage:

```
", $Feature, "let n = 0x1A", stringify!($SelfT), ";

if cfg!(target_endian = \"little\") {
    assert_eq!(", stringify!($SelfT), "::from_le(n), n)
} else {
    assert_eq!(", stringify!($SelfT), "::from_le(n), n.swap_bytes())
}",
$EndFeature, "
```"),
            #[stable(feature = "rust1", since = "1.0.0")]
            #[rustc_const_stable(feature = "const_int_conversions", since = "1.32.0")]
            #[inline]
            pub const fn from_le(x: Self) -> Self {
                #[cfg(target_endian = "little")]
                {
                    x
                }
                #[cfg(not(target_endian = "little"))]
                {
                    x.swap_bytes()
                }
            }
        }

        doc_comment! {
            concat!("Converts `self` to big endian from the target's endianness.

On big endian this is a no-op. On little endian the bytes are swapped.

# Examples

Basic usage:

```
", $Feature, "let n = 0x1A", stringify!($SelfT), ";

if cfg!(target_endian = \"big\") {
    assert_eq!(n.to_be(), n)
} else {
    assert_eq!(n.to_be(), n.swap_bytes())
}",
$EndFeature, "
```"),
            #[stable(feature = "rust1", since = "1.0.0")]
            #[rustc_const_stable(feature = "const_int_conversions", since = "1.32.0")]
            #[inline]
            pub const fn to_be(self) -> Self { // or not to be?
                #[cfg(target_endian = "big")]
                {
                    self
                }
                #[cfg(not(target_endian = "big"))]
                {
                    self.swap_bytes()
                }
            }
        }

        doc_comment! {
            concat!("Converts `self` to little endian from the target's endianness.

On little endian this is a no-op. On big endian the bytes are swapped.

# Examples

Basic usage:

```
", $Feature, "let n = 0x1A", stringify!($SelfT), ";

if cfg!(target_endian = \"little\") {
    assert_eq!(n.to_le(), n)
} else {
    assert_eq!(n.to_le(), n.swap_bytes())
}",
$EndFeature, "
```"),
            #[stable(feature = "rust1", since = "1.0.0")]
            #[rustc_const_stable(feature = "const_int_conversions", since = "1.32.0")]
            #[inline]
            pub const fn to_le(self) -> Self {
                #[cfg(target_endian = "little")]
                {
                    self
                }
                #[cfg(not(target_endian = "little"))]
                {
                    self.swap_bytes()
                }
            }
        }

        doc_comment! {
            concat!("Checked integer addition. Computes `self + rhs`, returning `None`
if overflow occurred.

# Examples

Basic usage:

```
", $Feature, "assert_eq!((", stringify!($SelfT),
"::MAX - 2).checked_add(1), Some(", stringify!($SelfT), "::MAX - 1));
assert_eq!((", stringify!($SelfT), "::MAX - 2).checked_add(3), None);",
$EndFeature, "
```"),
            #[stable(feature = "rust1", since = "1.0.0")]
            #[rustc_const_stable(feature = "const_checked_int_methods", since = "1.47.0")]
            #[must_use = "this returns the result of the operation, \
                          without modifying the original"]
            #[inline]
            pub const fn checked_add(self, rhs: Self) -> Option<Self> {
                let (a, b) = self.overflowing_add(rhs);
                if unlikely!(b) {None} else {Some(a)}
            }
        }

        doc_comment! {
            concat!("Unchecked integer addition. Computes `self + rhs`, assuming overflow
cannot occur. This results in undefined behavior when `self + rhs > ", stringify!($SelfT),
"::MAX` or `self + rhs < ", stringify!($SelfT), "::MIN`."),
            #[unstable(
                feature = "unchecked_math",
                reason = "niche optimization path",
                issue = "none",
            )]
            #[must_use = "this returns the result of the operation, \
                          without modifying the original"]
            #[inline]
            pub unsafe fn unchecked_add(self, rhs: Self) -> Self {
                // SAFETY: the caller must uphold the safety contract for
                // `unchecked_add`.
                unsafe { intrinsics::unchecked_add(self, rhs) }
            }
        }

        doc_comment! {
            concat!("Checked integer subtraction. Computes `self - rhs`, returning `None` if
overflow occurred.

# Examples

Basic usage:

```
", $Feature, "assert_eq!((", stringify!($SelfT),
"::MIN + 2).checked_sub(1), Some(", stringify!($SelfT), "::MIN + 1));
assert_eq!((", stringify!($SelfT), "::MIN + 2).checked_sub(3), None);",
$EndFeature, "
```"),
            #[stable(feature = "rust1", since = "1.0.0")]
            #[rustc_const_stable(feature = "const_checked_int_methods", since = "1.47.0")]
            #[must_use = "this returns the result of the operation, \
                          without modifying the original"]
            #[inline]
            pub const fn checked_sub(self, rhs: Self) -> Option<Self> {
                let (a, b) = self.overflowing_sub(rhs);
                if unlikely!(b) {None} else {Some(a)}
            }
        }

        doc_comment! {
            concat!("Unchecked integer subtraction. Computes `self - rhs`, assuming overflow
cannot occur. This results in undefined behavior when `self - rhs > ", stringify!($SelfT),
"::MAX` or `self - rhs < ", stringify!($SelfT), "::MIN`."),
            #[unstable(
                feature = "unchecked_math",
                reason = "niche optimization path",
                issue = "none",
            )]
            #[must_use = "this returns the result of the operation, \
                          without modifying the original"]
            #[inline]
            pub unsafe fn unchecked_sub(self, rhs: Self) -> Self {
                // SAFETY: the caller must uphold the safety contract for
                // `unchecked_sub`.
                unsafe { intrinsics::unchecked_sub(self, rhs) }
            }
        }

        doc_comment! {
            concat!("Checked integer multiplication. Computes `self * rhs`, returning `None` if
overflow occurred.

# Examples

Basic usage:

```
", $Feature, "assert_eq!(", stringify!($SelfT),
"::MAX.checked_mul(1), Some(", stringify!($SelfT), "::MAX));
assert_eq!(", stringify!($SelfT), "::MAX.checked_mul(2), None);",
$EndFeature, "
```"),
            #[stable(feature = "rust1", since = "1.0.0")]
            #[rustc_const_stable(feature = "const_checked_int_methods", since = "1.47.0")]
            #[must_use = "this returns the result of the operation, \
                          without modifying the original"]
            #[inline]
            pub const fn checked_mul(self, rhs: Self) -> Option<Self> {
                let (a, b) = self.overflowing_mul(rhs);
                if unlikely!(b) {None} else {Some(a)}
            }
        }

        doc_comment! {
            concat!("Unchecked integer multiplication. Computes `self * rhs`, assuming overflow
cannot occur. This results in undefined behavior when `self * rhs > ", stringify!($SelfT),
"::MAX` or `self * rhs < ", stringify!($SelfT), "::MIN`."),
            #[unstable(
                feature = "unchecked_math",
                reason = "niche optimization path",
                issue = "none",
            )]
            #[must_use = "this returns the result of the operation, \
                          without modifying the original"]
            #[inline]
            pub unsafe fn unchecked_mul(self, rhs: Self) -> Self {
                // SAFETY: the caller must uphold the safety contract for
                // `unchecked_mul`.
                unsafe { intrinsics::unchecked_mul(self, rhs) }
            }
        }

        doc_comment! {
            concat!("Checked integer division. Computes `self / rhs`, returning `None` if `rhs == 0`
or the division results in overflow.

# Examples

Basic usage:

```
", $Feature, "assert_eq!((", stringify!($SelfT),
"::MIN + 1).checked_div(-1), Some(", stringify!($Max), "));
assert_eq!(", stringify!($SelfT), "::MIN.checked_div(-1), None);
assert_eq!((1", stringify!($SelfT), ").checked_div(0), None);",
$EndFeature, "
```"),
            #[stable(feature = "rust1", since = "1.0.0")]
            #[rustc_const_unstable(feature = "const_checked_int_methods", issue = "53718")]
            #[must_use = "this returns the result of the operation, \
                          without modifying the original"]
            #[inline]
            pub const fn checked_div(self, rhs: Self) -> Option<Self> {
                if unlikely!(rhs == 0 || (self == Self::MIN && rhs == -1)) {
                    None
                } else {
                    // SAFETY: div by zero and by INT_MIN have been checked above
                    Some(unsafe { intrinsics::unchecked_div(self, rhs) })
                }
            }
        }

        doc_comment! {
            concat!("Checked Euclidean division. Computes `self.div_euclid(rhs)`,
returning `None` if `rhs == 0` or the division results in overflow.

# Examples

Basic usage:

```
assert_eq!((", stringify!($SelfT),
"::MIN + 1).checked_div_euclid(-1), Some(", stringify!($Max), "));
assert_eq!(", stringify!($SelfT), "::MIN.checked_div_euclid(-1), None);
assert_eq!((1", stringify!($SelfT), ").checked_div_euclid(0), None);
```"),
            #[stable(feature = "euclidean_division", since = "1.38.0")]
            #[rustc_const_unstable(feature = "const_euclidean_int_methods", issue = "53718")]
            #[must_use = "this returns the result of the operation, \
                          without modifying the original"]
            #[inline]
            pub const fn checked_div_euclid(self, rhs: Self) -> Option<Self> {
                if unlikely!(rhs == 0 || (self == Self::MIN && rhs == -1)) {
                    None
                } else {
                    Some(self.div_euclid(rhs))
                }
            }
        }

        doc_comment! {
            concat!("Checked integer remainder. Computes `self % rhs`, returning `None` if
`rhs == 0` or the division results in overflow.

# Examples

Basic usage:

```
", $Feature, "
assert_eq!(5", stringify!($SelfT), ".checked_rem(2), Some(1));
assert_eq!(5", stringify!($SelfT), ".checked_rem(0), None);
assert_eq!(", stringify!($SelfT), "::MIN.checked_rem(-1), None);",
$EndFeature, "
```"),
            #[stable(feature = "wrapping", since = "1.7.0")]
            #[rustc_const_unstable(feature = "const_checked_int_methods", issue = "53718")]
            #[must_use = "this returns the result of the operation, \
                          without modifying the original"]
            #[inline]
            pub const fn checked_rem(self, rhs: Self) -> Option<Self> {
                if unlikely!(rhs == 0 || (self == Self::MIN && rhs == -1)) {
                    None
                } else {
                    // SAFETY: div by zero and by INT_MIN have been checked above
                    Some(unsafe { intrinsics::unchecked_rem(self, rhs) })
                }
            }
        }

        doc_comment! {
            concat!("Checked Euclidean remainder. Computes `self.rem_euclid(rhs)`, returning `None`
if `rhs == 0` or the division results in overflow.

# Examples

Basic usage:

```
assert_eq!(5", stringify!($SelfT), ".checked_rem_euclid(2), Some(1));
assert_eq!(5", stringify!($SelfT), ".checked_rem_euclid(0), None);
assert_eq!(", stringify!($SelfT), "::MIN.checked_rem_euclid(-1), None);
```"),
            #[stable(feature = "euclidean_division", since = "1.38.0")]
            #[rustc_const_unstable(feature = "const_euclidean_int_methods", issue = "53718")]
            #[must_use = "this returns the result of the operation, \
                          without modifying the original"]
            #[inline]
            pub const fn checked_rem_euclid(self, rhs: Self) -> Option<Self> {
                if unlikely!(rhs == 0 || (self == Self::MIN && rhs == -1)) {
                    None
                } else {
                    Some(self.rem_euclid(rhs))
                }
            }
        }

        doc_comment! {
            concat!("Checked negation. Computes `-self`, returning `None` if `self == MIN`.

# Examples

Basic usage:

```
", $Feature, "
assert_eq!(5", stringify!($SelfT), ".checked_neg(), Some(-5));
assert_eq!(", stringify!($SelfT), "::MIN.checked_neg(), None);",
$EndFeature, "
```"),
            #[stable(feature = "wrapping", since = "1.7.0")]
            #[rustc_const_stable(feature = "const_checked_int_methods", since = "1.47.0")]
            #[inline]
            pub const fn checked_neg(self) -> Option<Self> {
                let (a, b) = self.overflowing_neg();
                if unlikely!(b) {None} else {Some(a)}
            }
        }

        doc_comment! {
            concat!("Checked shift left. Computes `self << rhs`, returning `None` if `rhs` is larger
than or equal to the number of bits in `self`.

# Examples

Basic usage:

```
", $Feature, "assert_eq!(0x1", stringify!($SelfT), ".checked_shl(4), Some(0x10));
assert_eq!(0x1", stringify!($SelfT), ".checked_shl(129), None);",
$EndFeature, "
```"),
            #[stable(feature = "wrapping", since = "1.7.0")]
            #[rustc_const_stable(feature = "const_checked_int_methods", since = "1.47.0")]
            #[must_use = "this returns the result of the operation, \
                          without modifying the original"]
            #[inline]
            pub const fn checked_shl(self, rhs: u32) -> Option<Self> {
                let (a, b) = self.overflowing_shl(rhs);
                if unlikely!(b) {None} else {Some(a)}
            }
        }

        doc_comment! {
            concat!("Checked shift right. Computes `self >> rhs`, returning `None` if `rhs` is
larger than or equal to the number of bits in `self`.

# Examples

Basic usage:

```
", $Feature, "assert_eq!(0x10", stringify!($SelfT), ".checked_shr(4), Some(0x1));
assert_eq!(0x10", stringify!($SelfT), ".checked_shr(128), None);",
$EndFeature, "
```"),
            #[stable(feature = "wrapping", since = "1.7.0")]
            #[rustc_const_stable(feature = "const_checked_int_methods", since = "1.47.0")]
            #[must_use = "this returns the result of the operation, \
                          without modifying the original"]
            #[inline]
            pub const fn checked_shr(self, rhs: u32) -> Option<Self> {
                let (a, b) = self.overflowing_shr(rhs);
                if unlikely!(b) {None} else {Some(a)}
            }
        }

        doc_comment! {
            concat!("Checked absolute value. Computes `self.abs()`, returning `None` if
`self == MIN`.

# Examples

Basic usage:

```
", $Feature, "
assert_eq!((-5", stringify!($SelfT), ").checked_abs(), Some(5));
assert_eq!(", stringify!($SelfT), "::MIN.checked_abs(), None);",
$EndFeature, "
```"),
            #[stable(feature = "no_panic_abs", since = "1.13.0")]
            #[rustc_const_stable(feature = "const_checked_int_methods", since = "1.47.0")]
            #[inline]
            pub const fn checked_abs(self) -> Option<Self> {
                if self.is_negative() {
                    self.checked_neg()
                } else {
                    Some(self)
                }
            }
        }

        doc_comment! {
            concat!("Checked exponentiation. Computes `self.pow(exp)`, returning `None` if
overflow occurred.

# Examples

Basic usage:

```
", $Feature, "assert_eq!(8", stringify!($SelfT), ".checked_pow(2), Some(64));
assert_eq!(", stringify!($SelfT), "::MAX.checked_pow(2), None);",
$EndFeature, "
```"),

            #[stable(feature = "no_panic_pow", since = "1.34.0")]
            #[rustc_const_unstable(feature = "const_int_pow", issue = "53718")]
            #[must_use = "this returns the result of the operation, \
                          without modifying the original"]
            #[inline]
            pub const fn checked_pow(self, mut exp: u32) -> Option<Self> {
                if exp == 0 {
                    return Some(1);
                }
                let mut base = self;
                let mut acc: Self = 1;

                while exp > 1 {
                    if (exp & 1) == 1 {
                        acc = try_opt!(acc.checked_mul(base));
                    }
                    exp /= 2;
                    base = try_opt!(base.checked_mul(base));
                }
                // since exp!=0, finally the exp must be 1.
                // Deal with the final bit of the exponent separately, since
                // squaring the base afterwards is not necessary and may cause a
                // needless overflow.
                Some(try_opt!(acc.checked_mul(base)))
            }
        }

        doc_comment! {
            concat!("Saturating integer addition. Computes `self + rhs`, saturating at the numeric
bounds instead of overflowing.

# Examples

Basic usage:

```
", $Feature, "assert_eq!(100", stringify!($SelfT), ".saturating_add(1), 101);
assert_eq!(", stringify!($SelfT), "::MAX.saturating_add(100), ", stringify!($SelfT),
"::MAX);
assert_eq!(", stringify!($SelfT), "::MIN.saturating_add(-1), ", stringify!($SelfT),
"::MIN);",
$EndFeature, "
```"),

            #[stable(feature = "rust1", since = "1.0.0")]
            #[rustc_const_stable(feature = "const_saturating_int_methods", since = "1.47.0")]
            #[must_use = "this returns the result of the operation, \
                          without modifying the original"]
            #[inline]
            pub const fn saturating_add(self, rhs: Self) -> Self {
                intrinsics::saturating_add(self, rhs)
            }
        }

        doc_comment! {
            concat!("Saturating integer subtraction. Computes `self - rhs`, saturating at the
numeric bounds instead of overflowing.

# Examples

Basic usage:

```
", $Feature, "assert_eq!(100", stringify!($SelfT), ".saturating_sub(127), -27);
assert_eq!(", stringify!($SelfT), "::MIN.saturating_sub(100), ", stringify!($SelfT),
"::MIN);
assert_eq!(", stringify!($SelfT), "::MAX.saturating_sub(-1), ", stringify!($SelfT),
"::MAX);",
$EndFeature, "
```"),
            #[stable(feature = "rust1", since = "1.0.0")]
            #[rustc_const_stable(feature = "const_saturating_int_methods", since = "1.47.0")]
            #[must_use = "this returns the result of the operation, \
                          without modifying the original"]
            #[inline]
            pub const fn saturating_sub(self, rhs: Self) -> Self {
                intrinsics::saturating_sub(self, rhs)
            }
        }

        doc_comment! {
            concat!("Saturating integer negation. Computes `-self`, returning `MAX` if `self == MIN`
instead of overflowing.

# Examples

Basic usage:

```
", $Feature, "assert_eq!(100", stringify!($SelfT), ".saturating_neg(), -100);
assert_eq!((-100", stringify!($SelfT), ").saturating_neg(), 100);
assert_eq!(", stringify!($SelfT), "::MIN.saturating_neg(), ", stringify!($SelfT),
"::MAX);
assert_eq!(", stringify!($SelfT), "::MAX.saturating_neg(), ", stringify!($SelfT),
"::MIN + 1);",
$EndFeature, "
```"),

            #[stable(feature = "saturating_neg", since = "1.45.0")]
            #[rustc_const_stable(feature = "const_saturating_int_methods", since = "1.47.0")]
            #[inline]
            pub const fn saturating_neg(self) -> Self {
                intrinsics::saturating_sub(0, self)
            }
        }

        doc_comment! {
            concat!("Saturating absolute value. Computes `self.abs()`, returning `MAX` if `self ==
MIN` instead of overflowing.

# Examples

Basic usage:

```
", $Feature, "assert_eq!(100", stringify!($SelfT), ".saturating_abs(), 100);
assert_eq!((-100", stringify!($SelfT), ").saturating_abs(), 100);
assert_eq!(", stringify!($SelfT), "::MIN.saturating_abs(), ", stringify!($SelfT),
"::MAX);
assert_eq!((", stringify!($SelfT), "::MIN + 1).saturating_abs(), ", stringify!($SelfT),
"::MAX);",
$EndFeature, "
```"),

            #[stable(feature = "saturating_neg", since = "1.45.0")]
            #[rustc_const_stable(feature = "const_saturating_int_methods", since = "1.47.0")]
            #[inline]
            pub const fn saturating_abs(self) -> Self {
                if self.is_negative() {
                    self.saturating_neg()
                } else {
                    self
                }
            }
        }

        doc_comment! {
            concat!("Saturating integer multiplication. Computes `self * rhs`, saturating at the
numeric bounds instead of overflowing.

# Examples

Basic usage:

```
", $Feature, "
assert_eq!(10", stringify!($SelfT), ".saturating_mul(12), 120);
assert_eq!(", stringify!($SelfT), "::MAX.saturating_mul(10), ", stringify!($SelfT), "::MAX);
assert_eq!(", stringify!($SelfT), "::MIN.saturating_mul(10), ", stringify!($SelfT), "::MIN);",
$EndFeature, "
```"),
            #[stable(feature = "wrapping", since = "1.7.0")]
            #[rustc_const_stable(feature = "const_saturating_int_methods", since = "1.47.0")]
            #[must_use = "this returns the result of the operation, \
                          without modifying the original"]
            #[inline]
            pub const fn saturating_mul(self, rhs: Self) -> Self {
                match self.checked_mul(rhs) {
                    Some(x) => x,
                    None => if (self < 0) == (rhs < 0) {
                        Self::MAX
                    } else {
                        Self::MIN
                    }
                }
            }
        }

        doc_comment! {
            concat!("Saturating integer exponentiation. Computes `self.pow(exp)`,
saturating at the numeric bounds instead of overflowing.

# Examples

Basic usage:

```
", $Feature, "
assert_eq!((-4", stringify!($SelfT), ").saturating_pow(3), -64);
assert_eq!(", stringify!($SelfT), "::MIN.saturating_pow(2), ", stringify!($SelfT), "::MAX);
assert_eq!(", stringify!($SelfT), "::MIN.saturating_pow(3), ", stringify!($SelfT), "::MIN);",
$EndFeature, "
```"),
            #[stable(feature = "no_panic_pow", since = "1.34.0")]
            #[rustc_const_unstable(feature = "const_int_pow", issue = "53718")]
            #[must_use = "this returns the result of the operation, \
                          without modifying the original"]
            #[inline]
            pub const fn saturating_pow(self, exp: u32) -> Self {
                match self.checked_pow(exp) {
                    Some(x) => x,
                    None if self < 0 && exp % 2 == 1 => Self::MIN,
                    None => Self::MAX,
                }
            }
        }

        doc_comment! {
            concat!("Wrapping (modular) addition. Computes `self + rhs`, wrapping around at the
boundary of the type.

# Examples

Basic usage:

```
", $Feature, "assert_eq!(100", stringify!($SelfT), ".wrapping_add(27), 127);
assert_eq!(", stringify!($SelfT), "::MAX.wrapping_add(2), ", stringify!($SelfT),
"::MIN + 1);",
$EndFeature, "
```"),
            #[stable(feature = "rust1", since = "1.0.0")]
            #[rustc_const_stable(feature = "const_int_methods", since = "1.32.0")]
            #[must_use = "this returns the result of the operation, \
                          without modifying the original"]
            #[inline]
            pub const fn wrapping_add(self, rhs: Self) -> Self {
                intrinsics::wrapping_add(self, rhs)
            }
        }

        doc_comment! {
            concat!("Wrapping (modular) subtraction. Computes `self - rhs`, wrapping around at the
boundary of the type.

# Examples

Basic usage:

```
", $Feature, "assert_eq!(0", stringify!($SelfT), ".wrapping_sub(127), -127);
assert_eq!((-2", stringify!($SelfT), ").wrapping_sub(", stringify!($SelfT), "::MAX), ",
stringify!($SelfT), "::MAX);",
$EndFeature, "
```"),
            #[stable(feature = "rust1", since = "1.0.0")]
            #[rustc_const_stable(feature = "const_int_methods", since = "1.32.0")]
            #[must_use = "this returns the result of the operation, \
                          without modifying the original"]
            #[inline]
            pub const fn wrapping_sub(self, rhs: Self) -> Self {
                intrinsics::wrapping_sub(self, rhs)
            }
        }

        doc_comment! {
            concat!("Wrapping (modular) multiplication. Computes `self * rhs`, wrapping around at
the boundary of the type.

# Examples

Basic usage:

```
", $Feature, "assert_eq!(10", stringify!($SelfT), ".wrapping_mul(12), 120);
assert_eq!(11i8.wrapping_mul(12), -124);",
$EndFeature, "
```"),
            #[stable(feature = "rust1", since = "1.0.0")]
            #[rustc_const_stable(feature = "const_int_methods", since = "1.32.0")]
            #[must_use = "this returns the result of the operation, \
                          without modifying the original"]
            #[inline]
            pub const fn wrapping_mul(self, rhs: Self) -> Self {
                intrinsics::wrapping_mul(self, rhs)
            }
        }

        doc_comment! {
            concat!("Wrapping (modular) division. Computes `self / rhs`, wrapping around at the
boundary of the type.

The only case where such wrapping can occur is when one divides `MIN / -1` on a signed type (where
`MIN` is the negative minimal value for the type); this is equivalent to `-MIN`, a positive value
that is too large to represent in the type. In such a case, this function returns `MIN` itself.

# Panics

This function will panic if `rhs` is 0.

# Examples

Basic usage:

```
", $Feature, "assert_eq!(100", stringify!($SelfT), ".wrapping_div(10), 10);
assert_eq!((-128i8).wrapping_div(-1), -128);",
$EndFeature, "
```"),
            #[stable(feature = "num_wrapping", since = "1.2.0")]
            #[rustc_const_unstable(feature = "const_wrapping_int_methods", issue = "53718")]
            #[must_use = "this returns the result of the operation, \
                          without modifying the original"]
            #[inline]
            pub const fn wrapping_div(self, rhs: Self) -> Self {
                self.overflowing_div(rhs).0
            }
        }

        doc_comment! {
            concat!("Wrapping Euclidean division. Computes `self.div_euclid(rhs)`,
wrapping around at the boundary of the type.

Wrapping will only occur in `MIN / -1` on a signed type (where `MIN` is the negative minimal value
for the type). This is equivalent to `-MIN`, a positive value that is too large to represent in the
type. In this case, this method returns `MIN` itself.

# Panics

This function will panic if `rhs` is 0.

# Examples

Basic usage:

```
assert_eq!(100", stringify!($SelfT), ".wrapping_div_euclid(10), 10);
assert_eq!((-128i8).wrapping_div_euclid(-1), -128);
```"),
            #[stable(feature = "euclidean_division", since = "1.38.0")]
            #[rustc_const_unstable(feature = "const_euclidean_int_methods", issue = "53718")]
            #[must_use = "this returns the result of the operation, \
                          without modifying the original"]
            #[inline]
            pub const fn wrapping_div_euclid(self, rhs: Self) -> Self {
                self.overflowing_div_euclid(rhs).0
            }
        }

        doc_comment! {
            concat!("Wrapping (modular) remainder. Computes `self % rhs`, wrapping around at the
boundary of the type.

Such wrap-around never actually occurs mathematically; implementation artifacts make `x % y`
invalid for `MIN / -1` on a signed type (where `MIN` is the negative minimal value). In such a case,
this function returns `0`.

# Panics

This function will panic if `rhs` is 0.

# Examples

Basic usage:

```
", $Feature, "assert_eq!(100", stringify!($SelfT), ".wrapping_rem(10), 0);
assert_eq!((-128i8).wrapping_rem(-1), 0);",
$EndFeature, "
```"),
            #[stable(feature = "num_wrapping", since = "1.2.0")]
            #[rustc_const_unstable(feature = "const_wrapping_int_methods", issue = "53718")]
            #[must_use = "this returns the result of the operation, \
                          without modifying the original"]
            #[inline]
            pub const fn wrapping_rem(self, rhs: Self) -> Self {
                self.overflowing_rem(rhs).0
            }
        }

        doc_comment! {
            concat!("Wrapping Euclidean remainder. Computes `self.rem_euclid(rhs)`, wrapping around
at the boundary of the type.

Wrapping will only occur in `MIN % -1` on a signed type (where `MIN` is the negative minimal value
for the type). In this case, this method returns 0.

# Panics

This function will panic if `rhs` is 0.

# Examples

Basic usage:

```
assert_eq!(100", stringify!($SelfT), ".wrapping_rem_euclid(10), 0);
assert_eq!((-128i8).wrapping_rem_euclid(-1), 0);
```"),
            #[stable(feature = "euclidean_division", since = "1.38.0")]
            #[rustc_const_unstable(feature = "const_euclidean_int_methods", issue = "53718")]
            #[must_use = "this returns the result of the operation, \
                          without modifying the original"]
            #[inline]
            pub const fn wrapping_rem_euclid(self, rhs: Self) -> Self {
                self.overflowing_rem_euclid(rhs).0
            }
        }

        doc_comment! {
            concat!("Wrapping (modular) negation. Computes `-self`, wrapping around at the boundary
of the type.

The only case where such wrapping can occur is when one negates `MIN` on a signed type (where `MIN`
is the negative minimal value for the type); this is a positive value that is too large to represent
in the type. In such a case, this function returns `MIN` itself.

# Examples

Basic usage:

```
", $Feature, "assert_eq!(100", stringify!($SelfT), ".wrapping_neg(), -100);
assert_eq!(", stringify!($SelfT), "::MIN.wrapping_neg(), ", stringify!($SelfT),
"::MIN);",
$EndFeature, "
```"),
            #[stable(feature = "num_wrapping", since = "1.2.0")]
            #[rustc_const_stable(feature = "const_int_methods", since = "1.32.0")]
            #[inline]
            pub const fn wrapping_neg(self) -> Self {
                self.overflowing_neg().0
            }
        }

        doc_comment! {
            concat!("Panic-free bitwise shift-left; yields `self << mask(rhs)`, where `mask` removes
any high-order bits of `rhs` that would cause the shift to exceed the bitwidth of the type.

Note that this is *not* the same as a rotate-left; the RHS of a wrapping shift-left is restricted to
the range of the type, rather than the bits shifted out of the LHS being returned to the other end.
The primitive integer types all implement a `[`rotate_left`](#method.rotate_left) function,
which may be what you want instead.

# Examples

Basic usage:

```
", $Feature, "assert_eq!((-1", stringify!($SelfT), ").wrapping_shl(7), -128);
assert_eq!((-1", stringify!($SelfT), ").wrapping_shl(128), -1);",
$EndFeature, "
```"),
            #[stable(feature = "num_wrapping", since = "1.2.0")]
            #[rustc_const_stable(feature = "const_int_methods", since = "1.32.0")]
            #[must_use = "this returns the result of the operation, \
                          without modifying the original"]
            #[inline]
            pub const fn wrapping_shl(self, rhs: u32) -> Self {
                // SAFETY: the masking by the bitsize of the type ensures that we do not shift
                // out of bounds
                unsafe {
                    intrinsics::unchecked_shl(self, (rhs & ($BITS - 1)) as $SelfT)
                }
            }
        }

        doc_comment! {
            concat!("Panic-free bitwise shift-right; yields `self >> mask(rhs)`, where `mask`
removes any high-order bits of `rhs` that would cause the shift to exceed the bitwidth of the type.

Note that this is *not* the same as a rotate-right; the RHS of a wrapping shift-right is restricted
to the range of the type, rather than the bits shifted out of the LHS being returned to the other
end. The primitive integer types all implement a [`rotate_right`](#method.rotate_right) function,
which may be what you want instead.

# Examples

Basic usage:

```
", $Feature, "assert_eq!((-128", stringify!($SelfT), ").wrapping_shr(7), -1);
assert_eq!((-128i16).wrapping_shr(64), -128);",
$EndFeature, "
```"),
            #[stable(feature = "num_wrapping", since = "1.2.0")]
            #[rustc_const_stable(feature = "const_int_methods", since = "1.32.0")]
            #[must_use = "this returns the result of the operation, \
                          without modifying the original"]
            #[inline]
            pub const fn wrapping_shr(self, rhs: u32) -> Self {
                // SAFETY: the masking by the bitsize of the type ensures that we do not shift
                // out of bounds
                unsafe {
                    intrinsics::unchecked_shr(self, (rhs & ($BITS - 1)) as $SelfT)
                }
            }
        }

        doc_comment! {
            concat!("Wrapping (modular) absolute value. Computes `self.abs()`, wrapping around at
the boundary of the type.

The only case where such wrapping can occur is when one takes the absolute value of the negative
minimal value for the type; this is a positive value that is too large to represent in the type. In
such a case, this function returns `MIN` itself.

# Examples

Basic usage:

```
", $Feature, "assert_eq!(100", stringify!($SelfT), ".wrapping_abs(), 100);
assert_eq!((-100", stringify!($SelfT), ").wrapping_abs(), 100);
assert_eq!(", stringify!($SelfT), "::MIN.wrapping_abs(), ", stringify!($SelfT),
"::MIN);
assert_eq!((-128i8).wrapping_abs() as u8, 128);",
$EndFeature, "
```"),
            #[stable(feature = "no_panic_abs", since = "1.13.0")]
            #[rustc_const_stable(feature = "const_int_methods", since = "1.32.0")]
            #[allow(unused_attributes)]
            #[inline]
            pub const fn wrapping_abs(self) -> Self {
                 if self.is_negative() {
                     self.wrapping_neg()
                 } else {
                     self
                 }
            }
        }

        doc_comment! {
            concat!("Computes the absolute value of `self` without any wrapping
or panicking.


# Examples

Basic usage:

```
", $Feature, "#![feature(unsigned_abs)]
assert_eq!(100", stringify!($SelfT), ".unsigned_abs(), 100", stringify!($UnsignedT), ");
assert_eq!((-100", stringify!($SelfT), ").unsigned_abs(), 100", stringify!($UnsignedT), ");
assert_eq!((-128i8).unsigned_abs(), 128u8);",
$EndFeature, "
```"),
            #[unstable(feature = "unsigned_abs", issue = "74913")]
            #[inline]
            pub const fn unsigned_abs(self) -> $UnsignedT {
                 self.wrapping_abs() as $UnsignedT
            }
        }

        doc_comment! {
            concat!("Wrapping (modular) exponentiation. Computes `self.pow(exp)`,
wrapping around at the boundary of the type.

# Examples

Basic usage:

```
", $Feature, "assert_eq!(3", stringify!($SelfT), ".wrapping_pow(4), 81);
assert_eq!(3i8.wrapping_pow(5), -13);
assert_eq!(3i8.wrapping_pow(6), -39);",
$EndFeature, "
```"),
            #[stable(feature = "no_panic_pow", since = "1.34.0")]
            #[rustc_const_unstable(feature = "const_int_pow", issue = "53718")]
            #[must_use = "this returns the result of the operation, \
                          without modifying the original"]
            #[inline]
            pub const fn wrapping_pow(self, mut exp: u32) -> Self {
                if exp == 0 {
                    return 1;
                }
                let mut base = self;
                let mut acc: Self = 1;

                while exp > 1 {
                    if (exp & 1) == 1 {
                        acc = acc.wrapping_mul(base);
                    }
                    exp /= 2;
                    base = base.wrapping_mul(base);
                }

                // since exp!=0, finally the exp must be 1.
                // Deal with the final bit of the exponent separately, since
                // squaring the base afterwards is not necessary and may cause a
                // needless overflow.
                acc.wrapping_mul(base)
            }
        }

        doc_comment! {
            concat!("Calculates `self` + `rhs`

Returns a tuple of the addition along with a boolean indicating whether an arithmetic overflow would
occur. If an overflow would have occurred then the wrapped value is returned.

# Examples

Basic usage:

```
", $Feature, "
assert_eq!(5", stringify!($SelfT), ".overflowing_add(2), (7, false));
assert_eq!(", stringify!($SelfT), "::MAX.overflowing_add(1), (", stringify!($SelfT),
"::MIN, true));", $EndFeature, "
```"),
            #[stable(feature = "wrapping", since = "1.7.0")]
            #[rustc_const_stable(feature = "const_int_methods", since = "1.32.0")]
            #[must_use = "this returns the result of the operation, \
                          without modifying the original"]
            #[inline]
            pub const fn overflowing_add(self, rhs: Self) -> (Self, bool) {
                let (a, b) = intrinsics::add_with_overflow(self as $ActualT, rhs as $ActualT);
                (a as Self, b)
            }
        }

        doc_comment! {
            concat!("Calculates `self` - `rhs`

Returns a tuple of the subtraction along with a boolean indicating whether an arithmetic overflow
would occur. If an overflow would have occurred then the wrapped value is returned.

# Examples

Basic usage:

```
", $Feature, "
assert_eq!(5", stringify!($SelfT), ".overflowing_sub(2), (3, false));
assert_eq!(", stringify!($SelfT), "::MIN.overflowing_sub(1), (", stringify!($SelfT),
"::MAX, true));", $EndFeature, "
```"),
            #[stable(feature = "wrapping", since = "1.7.0")]
            #[rustc_const_stable(feature = "const_int_methods", since = "1.32.0")]
            #[must_use = "this returns the result of the operation, \
                          without modifying the original"]
            #[inline]
            pub const fn overflowing_sub(self, rhs: Self) -> (Self, bool) {
                let (a, b) = intrinsics::sub_with_overflow(self as $ActualT, rhs as $ActualT);
                (a as Self, b)
            }
        }

        doc_comment! {
            concat!("Calculates the multiplication of `self` and `rhs`.

Returns a tuple of the multiplication along with a boolean indicating whether an arithmetic overflow
would occur. If an overflow would have occurred then the wrapped value is returned.

# Examples

Basic usage:

```
", $Feature, "assert_eq!(5", stringify!($SelfT), ".overflowing_mul(2), (10, false));
assert_eq!(1_000_000_000i32.overflowing_mul(10), (1410065408, true));",
$EndFeature, "
```"),
            #[stable(feature = "wrapping", since = "1.7.0")]
            #[rustc_const_stable(feature = "const_int_methods", since = "1.32.0")]
            #[must_use = "this returns the result of the operation, \
                          without modifying the original"]
            #[inline]
            pub const fn overflowing_mul(self, rhs: Self) -> (Self, bool) {
                let (a, b) = intrinsics::mul_with_overflow(self as $ActualT, rhs as $ActualT);
                (a as Self, b)
            }
        }

        doc_comment! {
            concat!("Calculates the divisor when `self` is divided by `rhs`.

Returns a tuple of the divisor along with a boolean indicating whether an arithmetic overflow would
occur. If an overflow would occur then self is returned.

# Panics

This function will panic if `rhs` is 0.

# Examples

Basic usage:

```
", $Feature, "
assert_eq!(5", stringify!($SelfT), ".overflowing_div(2), (2, false));
assert_eq!(", stringify!($SelfT), "::MIN.overflowing_div(-1), (", stringify!($SelfT),
"::MIN, true));",
$EndFeature, "
```"),
            #[inline]
            #[stable(feature = "wrapping", since = "1.7.0")]
            #[rustc_const_unstable(feature = "const_overflowing_int_methods", issue = "53718")]
            #[must_use = "this returns the result of the operation, \
                          without modifying the original"]
            pub const fn overflowing_div(self, rhs: Self) -> (Self, bool) {
                if unlikely!(self == Self::MIN && rhs == -1) {
                    (self, true)
                } else {
                    (self / rhs, false)
                }
            }
        }

        doc_comment! {
            concat!("Calculates the quotient of Euclidean division `self.div_euclid(rhs)`.

Returns a tuple of the divisor along with a boolean indicating whether an arithmetic overflow would
occur. If an overflow would occur then `self` is returned.

# Panics

This function will panic if `rhs` is 0.

# Examples

Basic usage:

```
assert_eq!(5", stringify!($SelfT), ".overflowing_div_euclid(2), (2, false));
assert_eq!(", stringify!($SelfT), "::MIN.overflowing_div_euclid(-1), (", stringify!($SelfT),
"::MIN, true));
```"),
            #[inline]
            #[stable(feature = "euclidean_division", since = "1.38.0")]
            #[rustc_const_unstable(feature = "const_euclidean_int_methods", issue = "53718")]
            #[must_use = "this returns the result of the operation, \
                          without modifying the original"]
            pub const fn overflowing_div_euclid(self, rhs: Self) -> (Self, bool) {
                if unlikely!(self == Self::MIN && rhs == -1) {
                    (self, true)
                } else {
                    (self.div_euclid(rhs), false)
                }
            }
        }

        doc_comment! {
            concat!("Calculates the remainder when `self` is divided by `rhs`.

Returns a tuple of the remainder after dividing along with a boolean indicating whether an
arithmetic overflow would occur. If an overflow would occur then 0 is returned.

# Panics

This function will panic if `rhs` is 0.

# Examples

Basic usage:

```
", $Feature, "
assert_eq!(5", stringify!($SelfT), ".overflowing_rem(2), (1, false));
assert_eq!(", stringify!($SelfT), "::MIN.overflowing_rem(-1), (0, true));",
$EndFeature, "
```"),
            #[inline]
            #[stable(feature = "wrapping", since = "1.7.0")]
            #[rustc_const_unstable(feature = "const_overflowing_int_methods", issue = "53718")]
            #[must_use = "this returns the result of the operation, \
                          without modifying the original"]
            pub const fn overflowing_rem(self, rhs: Self) -> (Self, bool) {
                if unlikely!(self == Self::MIN && rhs == -1) {
                    (0, true)
                } else {
                    (self % rhs, false)
                }
            }
        }


        doc_comment! {
            concat!("Overflowing Euclidean remainder. Calculates `self.rem_euclid(rhs)`.

Returns a tuple of the remainder after dividing along with a boolean indicating whether an
arithmetic overflow would occur. If an overflow would occur then 0 is returned.

# Panics

This function will panic if `rhs` is 0.

# Examples

Basic usage:

```
assert_eq!(5", stringify!($SelfT), ".overflowing_rem_euclid(2), (1, false));
assert_eq!(", stringify!($SelfT), "::MIN.overflowing_rem_euclid(-1), (0, true));
```"),
            #[stable(feature = "euclidean_division", since = "1.38.0")]
            #[rustc_const_unstable(feature = "const_euclidean_int_methods", issue = "53718")]
            #[must_use = "this returns the result of the operation, \
                          without modifying the original"]
            #[inline]
            pub const fn overflowing_rem_euclid(self, rhs: Self) -> (Self, bool) {
                if unlikely!(self == Self::MIN && rhs == -1) {
                    (0, true)
                } else {
                    (self.rem_euclid(rhs), false)
                }
            }
        }


        doc_comment! {
            concat!("Negates self, overflowing if this is equal to the minimum value.

Returns a tuple of the negated version of self along with a boolean indicating whether an overflow
happened. If `self` is the minimum value (e.g., `i32::MIN` for values of type `i32`), then the
minimum value will be returned again and `true` will be returned for an overflow happening.

# Examples

Basic usage:

```
assert_eq!(2", stringify!($SelfT), ".overflowing_neg(), (-2, false));
assert_eq!(", stringify!($SelfT), "::MIN.overflowing_neg(), (", stringify!($SelfT),
"::MIN, true));", $EndFeature, "
```"),
            #[inline]
            #[stable(feature = "wrapping", since = "1.7.0")]
            #[rustc_const_stable(feature = "const_int_methods", since = "1.32.0")]
            #[allow(unused_attributes)]
            pub const fn overflowing_neg(self) -> (Self, bool) {
                if unlikely!(self == Self::MIN) {
                    (Self::MIN, true)
                } else {
                    (-self, false)
                }
            }
        }

        doc_comment! {
            concat!("Shifts self left by `rhs` bits.

Returns a tuple of the shifted version of self along with a boolean indicating whether the shift
value was larger than or equal to the number of bits. If the shift value is too large, then value is
masked (N-1) where N is the number of bits, and this value is then used to perform the shift.

# Examples

Basic usage:

```
", $Feature, "assert_eq!(0x1", stringify!($SelfT),".overflowing_shl(4), (0x10, false));
assert_eq!(0x1i32.overflowing_shl(36), (0x10, true));",
$EndFeature, "
```"),
            #[stable(feature = "wrapping", since = "1.7.0")]
            #[rustc_const_stable(feature = "const_int_methods", since = "1.32.0")]
            #[must_use = "this returns the result of the operation, \
                          without modifying the original"]
            #[inline]
            pub const fn overflowing_shl(self, rhs: u32) -> (Self, bool) {
                (self.wrapping_shl(rhs), (rhs > ($BITS - 1)))
            }
        }

        doc_comment! {
            concat!("Shifts self right by `rhs` bits.

Returns a tuple of the shifted version of self along with a boolean indicating whether the shift
value was larger than or equal to the number of bits. If the shift value is too large, then value is
masked (N-1) where N is the number of bits, and this value is then used to perform the shift.

# Examples

Basic usage:

```
", $Feature, "assert_eq!(0x10", stringify!($SelfT), ".overflowing_shr(4), (0x1, false));
assert_eq!(0x10i32.overflowing_shr(36), (0x1, true));",
$EndFeature, "
```"),
            #[stable(feature = "wrapping", since = "1.7.0")]
            #[rustc_const_stable(feature = "const_int_methods", since = "1.32.0")]
            #[must_use = "this returns the result of the operation, \
                          without modifying the original"]
            #[inline]
            pub const fn overflowing_shr(self, rhs: u32) -> (Self, bool) {
                (self.wrapping_shr(rhs), (rhs > ($BITS - 1)))
            }
        }

        doc_comment! {
            concat!("Computes the absolute value of `self`.

Returns a tuple of the absolute version of self along with a boolean indicating whether an overflow
happened. If self is the minimum value (e.g., ", stringify!($SelfT), "::MIN for values of type
 ", stringify!($SelfT), "), then the minimum value will be returned again and true will be returned
for an overflow happening.

# Examples

Basic usage:

```
", $Feature, "assert_eq!(10", stringify!($SelfT), ".overflowing_abs(), (10, false));
assert_eq!((-10", stringify!($SelfT), ").overflowing_abs(), (10, false));
assert_eq!((", stringify!($SelfT), "::MIN).overflowing_abs(), (", stringify!($SelfT),
"::MIN, true));",
$EndFeature, "
```"),
            #[stable(feature = "no_panic_abs", since = "1.13.0")]
            #[rustc_const_stable(feature = "const_int_methods", since = "1.32.0")]
            #[inline]
            pub const fn overflowing_abs(self) -> (Self, bool) {
                (self.wrapping_abs(), self == Self::MIN)
            }
        }

        doc_comment! {
            concat!("Raises self to the power of `exp`, using exponentiation by squaring.

Returns a tuple of the exponentiation along with a bool indicating
whether an overflow happened.

# Examples

Basic usage:

```
", $Feature, "assert_eq!(3", stringify!($SelfT), ".overflowing_pow(4), (81, false));
assert_eq!(3i8.overflowing_pow(5), (-13, true));",
$EndFeature, "
```"),
            #[stable(feature = "no_panic_pow", since = "1.34.0")]
            #[rustc_const_unstable(feature = "const_int_pow", issue = "53718")]
            #[must_use = "this returns the result of the operation, \
                          without modifying the original"]
            #[inline]
            pub const fn overflowing_pow(self, mut exp: u32) -> (Self, bool) {
                if exp == 0 {
                    return (1,false);
                }
                let mut base = self;
                let mut acc: Self = 1;
                let mut overflown = false;
                // Scratch space for storing results of overflowing_mul.
                let mut r;

                while exp > 1 {
                    if (exp & 1) == 1 {
                        r = acc.overflowing_mul(base);
                        acc = r.0;
                        overflown |= r.1;
                    }
                    exp /= 2;
                    r = base.overflowing_mul(base);
                    base = r.0;
                    overflown |= r.1;
                }

                // since exp!=0, finally the exp must be 1.
                // Deal with the final bit of the exponent separately, since
                // squaring the base afterwards is not necessary and may cause a
                // needless overflow.
                r = acc.overflowing_mul(base);
                r.1 |= overflown;
                r
            }
        }

        doc_comment! {
            concat!("Raises self to the power of `exp`, using exponentiation by squaring.

# Examples

Basic usage:

```
", $Feature, "let x: ", stringify!($SelfT), " = 2; // or any other integer type

assert_eq!(x.pow(5), 32);",
$EndFeature, "
```"),
            #[stable(feature = "rust1", since = "1.0.0")]
            #[rustc_const_unstable(feature = "const_int_pow", issue = "53718")]
            #[must_use = "this returns the result of the operation, \
                          without modifying the original"]
            #[inline]
            #[rustc_inherit_overflow_checks]
            pub const fn pow(self, mut exp: u32) -> Self {
                if exp == 0 {
                    return 1;
                }
                let mut base = self;
                let mut acc = 1;

                while exp > 1 {
                    if (exp & 1) == 1 {
                        acc = acc * base;
                    }
                    exp /= 2;
                    base = base * base;
                }

                // since exp!=0, finally the exp must be 1.
                // Deal with the final bit of the exponent separately, since
                // squaring the base afterwards is not necessary and may cause a
                // needless overflow.
                acc * base
            }
        }

        doc_comment! {
            concat!("Calculates the quotient of Euclidean division of `self` by `rhs`.

This computes the integer `n` such that `self = n * rhs + self.rem_euclid(rhs)`,
with `0 <= self.rem_euclid(rhs) < rhs`.

In other words, the result is `self / rhs` rounded to the integer `n`
such that `self >= n * rhs`.
If `self > 0`, this is equal to round towards zero (the default in Rust);
if `self < 0`, this is equal to round towards +/- infinity.

# Panics

This function will panic if `rhs` is 0 or the division results in overflow.

# Examples

Basic usage:

```
let a: ", stringify!($SelfT), " = 7; // or any other integer type
let b = 4;

assert_eq!(a.div_euclid(b), 1); // 7 >= 4 * 1
assert_eq!(a.div_euclid(-b), -1); // 7 >= -4 * -1
assert_eq!((-a).div_euclid(b), -2); // -7 >= 4 * -2
assert_eq!((-a).div_euclid(-b), 2); // -7 >= -4 * 2
```"),
            #[stable(feature = "euclidean_division", since = "1.38.0")]
            #[rustc_const_unstable(feature = "const_euclidean_int_methods", issue = "53718")]
            #[must_use = "this returns the result of the operation, \
                          without modifying the original"]
            #[inline]
            #[rustc_inherit_overflow_checks]
            pub const fn div_euclid(self, rhs: Self) -> Self {
                let q = self / rhs;
                if self % rhs < 0 {
                    return if rhs > 0 { q - 1 } else { q + 1 }
                }
                q
            }
        }


        doc_comment! {
            concat!("Calculates the least nonnegative remainder of `self (mod rhs)`.

This is done as if by the Euclidean division algorithm -- given
`r = self.rem_euclid(rhs)`, `self = rhs * self.div_euclid(rhs) + r`, and
`0 <= r < abs(rhs)`.

# Panics

This function will panic if `rhs` is 0 or the division results in overflow.

# Examples

Basic usage:

```
let a: ", stringify!($SelfT), " = 7; // or any other integer type
let b = 4;

assert_eq!(a.rem_euclid(b), 3);
assert_eq!((-a).rem_euclid(b), 1);
assert_eq!(a.rem_euclid(-b), 3);
assert_eq!((-a).rem_euclid(-b), 1);
```"),
            #[stable(feature = "euclidean_division", since = "1.38.0")]
            #[rustc_const_unstable(feature = "const_euclidean_int_methods", issue = "53718")]
            #[must_use = "this returns the result of the operation, \
                          without modifying the original"]
            #[inline]
            #[rustc_inherit_overflow_checks]
            pub const fn rem_euclid(self, rhs: Self) -> Self {
                let r = self % rhs;
                if r < 0 {
                    if rhs < 0 {
                        r - rhs
                    } else {
                        r + rhs
                    }
                } else {
                    r
                }
            }
        }

        doc_comment! {
            concat!("Computes the absolute value of `self`.

# Overflow behavior

The absolute value of `", stringify!($SelfT), "::MIN` cannot be represented as an
`", stringify!($SelfT), "`, and attempting to calculate it will cause an overflow. This means that
code in debug mode will trigger a panic on this case and optimized code will return `",
stringify!($SelfT), "::MIN` without a panic.

# Examples

Basic usage:

```
", $Feature, "assert_eq!(10", stringify!($SelfT), ".abs(), 10);
assert_eq!((-10", stringify!($SelfT), ").abs(), 10);",
$EndFeature, "
```"),
            #[stable(feature = "rust1", since = "1.0.0")]
            #[rustc_const_stable(feature = "const_int_methods", since = "1.32.0")]
            #[allow(unused_attributes)]
            #[inline]
            #[rustc_inherit_overflow_checks]
            pub const fn abs(self) -> Self {
                // Note that the #[inline] above means that the overflow
                // semantics of the subtraction depend on the crate we're being
                // inlined into.
                if self.is_negative() {
                    -self
                } else {
                    self
                }
            }
        }

        doc_comment! {
            concat!("Returns a number representing sign of `self`.

 - `0` if the number is zero
 - `1` if the number is positive
 - `-1` if the number is negative

# Examples

Basic usage:

```
", $Feature, "assert_eq!(10", stringify!($SelfT), ".signum(), 1);
assert_eq!(0", stringify!($SelfT), ".signum(), 0);
assert_eq!((-10", stringify!($SelfT), ").signum(), -1);",
$EndFeature, "
```"),
            #[stable(feature = "rust1", since = "1.0.0")]
            #[rustc_const_stable(feature = "const_int_sign", since = "1.47.0")]
            #[inline]
            pub const fn signum(self) -> Self {
                match self {
                    n if n > 0 =>  1,
                    0          =>  0,
                    _          => -1,
                }
            }
        }

        doc_comment! {
            concat!("Returns `true` if `self` is positive and `false` if the number is zero or
negative.

# Examples

Basic usage:

```
", $Feature, "assert!(10", stringify!($SelfT), ".is_positive());
assert!(!(-10", stringify!($SelfT), ").is_positive());",
$EndFeature, "
```"),
            #[stable(feature = "rust1", since = "1.0.0")]
            #[rustc_const_stable(feature = "const_int_methods", since = "1.32.0")]
            #[inline]
            pub const fn is_positive(self) -> bool { self > 0 }
        }

        doc_comment! {
            concat!("Returns `true` if `self` is negative and `false` if the number is zero or
positive.

# Examples

Basic usage:

```
", $Feature, "assert!((-10", stringify!($SelfT), ").is_negative());
assert!(!10", stringify!($SelfT), ".is_negative());",
$EndFeature, "
```"),
            #[stable(feature = "rust1", since = "1.0.0")]
            #[rustc_const_stable(feature = "const_int_methods", since = "1.32.0")]
            #[inline]
            pub const fn is_negative(self) -> bool { self < 0 }
        }

        doc_comment! {
            concat!("Return the memory representation of this integer as a byte array in
big-endian (network) byte order.
",
$to_xe_bytes_doc,
"
# Examples

```
let bytes = ", $swap_op, stringify!($SelfT), ".to_be_bytes();
assert_eq!(bytes, ", $be_bytes, ");
```"),
            #[stable(feature = "int_to_from_bytes", since = "1.32.0")]
            #[rustc_const_stable(feature = "const_int_conversion", since = "1.44.0")]
            #[inline]
            pub const fn to_be_bytes(self) -> [u8; mem::size_of::<Self>()] {
                self.to_be().to_ne_bytes()
            }
        }

doc_comment! {
            concat!("Return the memory representation of this integer as a byte array in
little-endian byte order.
",
$to_xe_bytes_doc,
"
# Examples

```
let bytes = ", $swap_op, stringify!($SelfT), ".to_le_bytes();
assert_eq!(bytes, ", $le_bytes, ");
```"),
            #[stable(feature = "int_to_from_bytes", since = "1.32.0")]
            #[rustc_const_stable(feature = "const_int_conversion", since = "1.44.0")]
            #[inline]
            pub const fn to_le_bytes(self) -> [u8; mem::size_of::<Self>()] {
                self.to_le().to_ne_bytes()
            }
        }

        doc_comment! {
            concat!("
Return the memory representation of this integer as a byte array in
native byte order.

As the target platform's native endianness is used, portable code
should use [`to_be_bytes`] or [`to_le_bytes`], as appropriate,
instead.
",
$to_xe_bytes_doc,
"
[`to_be_bytes`]: #method.to_be_bytes
[`to_le_bytes`]: #method.to_le_bytes

# Examples

```
let bytes = ", $swap_op, stringify!($SelfT), ".to_ne_bytes();
assert_eq!(
    bytes,
    if cfg!(target_endian = \"big\") {
        ", $be_bytes, "
    } else {
        ", $le_bytes, "
    }
);
```"),
            #[stable(feature = "int_to_from_bytes", since = "1.32.0")]
            #[rustc_const_stable(feature = "const_int_conversion", since = "1.44.0")]
            // SAFETY: const sound because integers are plain old datatypes so we can always
            // transmute them to arrays of bytes
            #[cfg_attr(not(bootstrap), rustc_allow_const_fn_unstable(const_fn_transmute))]
            #[cfg_attr(bootstrap, allow_internal_unstable(const_fn_transmute))]
            #[inline]
            pub const fn to_ne_bytes(self) -> [u8; mem::size_of::<Self>()] {
                // SAFETY: integers are plain old datatypes so we can always transmute them to
                // arrays of bytes
                unsafe { mem::transmute(self) }
            }
        }

        doc_comment! {
            concat!("
Return the memory representation of this integer as a byte array in
native byte order.

[`to_ne_bytes`] should be preferred over this whenever possible.

[`to_ne_bytes`]: #method.to_ne_bytes
",

"
# Examples

```
#![feature(num_as_ne_bytes)]
let num = ", $swap_op, stringify!($SelfT), ";
let bytes = num.as_ne_bytes();
assert_eq!(
    bytes,
    if cfg!(target_endian = \"big\") {
        &", $be_bytes, "
    } else {
        &", $le_bytes, "
    }
);
```"),
            #[unstable(feature = "num_as_ne_bytes", issue = "76976")]
            #[inline]
            pub fn as_ne_bytes(&self) -> &[u8; mem::size_of::<Self>()] {
                // SAFETY: integers are plain old datatypes so we can always transmute them to
                // arrays of bytes
                unsafe { &*(self as *const Self as *const _) }
            }
        }

doc_comment! {
            concat!("Create an integer value from its representation as a byte array in
big endian.
",
$from_xe_bytes_doc,
"
# Examples

```
let value = ", stringify!($SelfT), "::from_be_bytes(", $be_bytes, ");
assert_eq!(value, ", $swap_op, ");
```

When starting from a slice rather than an array, fallible conversion APIs can be used:

```
use std::convert::TryInto;

fn read_be_", stringify!($SelfT), "(input: &mut &[u8]) -> ", stringify!($SelfT), " {
    let (int_bytes, rest) = input.split_at(std::mem::size_of::<", stringify!($SelfT), ">());
    *input = rest;
    ", stringify!($SelfT), "::from_be_bytes(int_bytes.try_into().unwrap())
}
```"),
            #[stable(feature = "int_to_from_bytes", since = "1.32.0")]
            #[rustc_const_stable(feature = "const_int_conversion", since = "1.44.0")]
            #[inline]
            pub const fn from_be_bytes(bytes: [u8; mem::size_of::<Self>()]) -> Self {
                Self::from_be(Self::from_ne_bytes(bytes))
            }
        }

doc_comment! {
            concat!("
Create an integer value from its representation as a byte array in
little endian.
",
$from_xe_bytes_doc,
"
# Examples

```
let value = ", stringify!($SelfT), "::from_le_bytes(", $le_bytes, ");
assert_eq!(value, ", $swap_op, ");
```

When starting from a slice rather than an array, fallible conversion APIs can be used:

```
use std::convert::TryInto;

fn read_le_", stringify!($SelfT), "(input: &mut &[u8]) -> ", stringify!($SelfT), " {
    let (int_bytes, rest) = input.split_at(std::mem::size_of::<", stringify!($SelfT), ">());
    *input = rest;
    ", stringify!($SelfT), "::from_le_bytes(int_bytes.try_into().unwrap())
}
```"),
            #[stable(feature = "int_to_from_bytes", since = "1.32.0")]
            #[rustc_const_stable(feature = "const_int_conversion", since = "1.44.0")]
            #[inline]
            pub const fn from_le_bytes(bytes: [u8; mem::size_of::<Self>()]) -> Self {
                Self::from_le(Self::from_ne_bytes(bytes))
            }
        }

        doc_comment! {
            concat!("Create an integer value from its memory representation as a byte
array in native endianness.

As the target platform's native endianness is used, portable code
likely wants to use [`from_be_bytes`] or [`from_le_bytes`], as
appropriate instead.

[`from_be_bytes`]: #method.from_be_bytes
[`from_le_bytes`]: #method.from_le_bytes
",
$from_xe_bytes_doc,
"
# Examples

```
let value = ", stringify!($SelfT), "::from_ne_bytes(if cfg!(target_endian = \"big\") {
    ", $be_bytes, "
} else {
    ", $le_bytes, "
});
assert_eq!(value, ", $swap_op, ");
```

When starting from a slice rather than an array, fallible conversion APIs can be used:

```
use std::convert::TryInto;

fn read_ne_", stringify!($SelfT), "(input: &mut &[u8]) -> ", stringify!($SelfT), " {
    let (int_bytes, rest) = input.split_at(std::mem::size_of::<", stringify!($SelfT), ">());
    *input = rest;
    ", stringify!($SelfT), "::from_ne_bytes(int_bytes.try_into().unwrap())
}
```"),
            #[stable(feature = "int_to_from_bytes", since = "1.32.0")]
            #[rustc_const_stable(feature = "const_int_conversion", since = "1.44.0")]
            // SAFETY: const sound because integers are plain old datatypes so we can always
            // transmute to them
            #[cfg_attr(not(bootstrap), rustc_allow_const_fn_unstable(const_fn_transmute))]
            #[cfg_attr(bootstrap, allow_internal_unstable(const_fn_transmute))]
            #[inline]
            pub const fn from_ne_bytes(bytes: [u8; mem::size_of::<Self>()]) -> Self {
                // SAFETY: integers are plain old datatypes so we can always transmute to them
                unsafe { mem::transmute(bytes) }
            }
        }

        doc_comment! {
            concat!("**This method is soft-deprecated.**

Although using it won’t cause a compilation warning,
new code should use [`", stringify!($SelfT), "::MIN", "`](#associatedconstant.MIN) instead.

Returns the smallest value that can be represented by this integer type."),
            #[stable(feature = "rust1", since = "1.0.0")]
            #[inline(always)]
            #[rustc_promotable]
            #[rustc_const_stable(feature = "const_min_value", since = "1.32.0")]
            pub const fn min_value() -> Self {
                Self::MIN
            }
        }

        doc_comment! {
            concat!("**This method is soft-deprecated.**

Although using it won’t cause a compilation warning,
new code should use [`", stringify!($SelfT), "::MAX", "`](#associatedconstant.MAX) instead.

Returns the largest value that can be represented by this integer type."),
            #[stable(feature = "rust1", since = "1.0.0")]
            #[inline(always)]
            #[rustc_promotable]
            #[rustc_const_stable(feature = "const_max_value", since = "1.32.0")]
            pub const fn max_value() -> Self {
                Self::MAX
            }
        }
    }
}
