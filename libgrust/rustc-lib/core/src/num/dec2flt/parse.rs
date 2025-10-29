//! Validating and decomposing a decimal string of the form:
//!
//! `(digits | digits? '.'? digits?) (('e' | 'E') ('+' | '-')? digits)?`
//!
//! In other words, standard floating-point syntax, with two exceptions: No sign, and no
//! handling of "inf" and "NaN". These are handled by the driver function (super::dec2flt).
//!
//! Although recognizing valid inputs is relatively easy, this module also has to reject the
//! countless invalid variations, never panic, and perform numerous checks that the other
//! modules rely on to not panic (or overflow) in turn.
//! To make matters worse, all that happens in a single pass over the input.
//! So, be careful when modifying anything, and double-check with the other modules.
use self::ParseResult::{Invalid, ShortcutToInf, ShortcutToZero, Valid};
use super::num;

#[derive(Debug)]
pub enum Sign {
    Positive,
    Negative,
}

#[derive(Debug, PartialEq, Eq)]
/// The interesting parts of a decimal string.
pub struct Decimal<'a> {
    pub integral: &'a [u8],
    pub fractional: &'a [u8],
    /// The decimal exponent, guaranteed to have fewer than 18 decimal digits.
    pub exp: i64,
}

impl<'a> Decimal<'a> {
    pub fn new(integral: &'a [u8], fractional: &'a [u8], exp: i64) -> Decimal<'a> {
        Decimal { integral, fractional, exp }
    }
}

#[derive(Debug, PartialEq, Eq)]
pub enum ParseResult<'a> {
    Valid(Decimal<'a>),
    ShortcutToInf,
    ShortcutToZero,
    Invalid,
}

/// Checks if the input string is a valid floating point number and if so, locate the integral
/// part, the fractional part, and the exponent in it. Does not handle signs.
pub fn parse_decimal(s: &str) -> ParseResult<'_> {
    if s.is_empty() {
        return Invalid;
    }

    let s = s.as_bytes();
    let (integral, s) = eat_digits(s);

    match s.first() {
        None => Valid(Decimal::new(integral, b"", 0)),
        Some(&b'e' | &b'E') => {
            if integral.is_empty() {
                return Invalid; // No digits before 'e'
            }

            parse_exp(integral, b"", &s[1..])
        }
        Some(&b'.') => {
            let (fractional, s) = eat_digits(&s[1..]);
            if integral.is_empty() && fractional.is_empty() {
                // We require at least a single digit before or after the point.
                return Invalid;
            }

            match s.first() {
                None => Valid(Decimal::new(integral, fractional, 0)),
                Some(&b'e' | &b'E') => parse_exp(integral, fractional, &s[1..]),
                _ => Invalid, // Trailing junk after fractional part
            }
        }
        _ => Invalid, // Trailing junk after first digit string
    }
}

/// Carves off decimal digits up to the first non-digit character.
fn eat_digits(s: &[u8]) -> (&[u8], &[u8]) {
    let mut i = 0;
    while i < s.len() && b'0' <= s[i] && s[i] <= b'9' {
        i += 1;
    }
    (&s[..i], &s[i..])
}

/// Exponent extraction and error checking.
fn parse_exp<'a>(integral: &'a [u8], fractional: &'a [u8], rest: &'a [u8]) -> ParseResult<'a> {
    let (sign, rest) = match rest.first() {
        Some(&b'-') => (Sign::Negative, &rest[1..]),
        Some(&b'+') => (Sign::Positive, &rest[1..]),
        _ => (Sign::Positive, rest),
    };
    let (mut number, trailing) = eat_digits(rest);
    if !trailing.is_empty() {
        return Invalid; // Trailing junk after exponent
    }
    if number.is_empty() {
        return Invalid; // Empty exponent
    }
    // At this point, we certainly have a valid string of digits. It may be too long to put into
    // an `i64`, but if it's that huge, the input is certainly zero or infinity. Since each zero
    // in the decimal digits only adjusts the exponent by +/- 1, at exp = 10^18 the input would
    // have to be 17 exabyte (!) of zeros to get even remotely close to being finite.
    // This is not exactly a use case we need to cater to.
    while number.first() == Some(&b'0') {
        number = &number[1..];
    }
    if number.len() >= 18 {
        return match sign {
            Sign::Positive => ShortcutToInf,
            Sign::Negative => ShortcutToZero,
        };
    }
    let abs_exp = num::from_str_unchecked(number);
    let e = match sign {
        Sign::Positive => abs_exp as i64,
        Sign::Negative => -(abs_exp as i64),
    };
    Valid(Decimal::new(integral, fractional, e))
}
