const FOO: Option<&[[u8; 3]]> = Some(&[*b"foo"]); // { dg-error ".E0716." "" { target *-*-* } }

use std::borrow::Cow;

pub const X: [u8; 3] = *b"ABC";
pub const Y: Cow<'static, [ [u8; 3] ]> = Cow::Borrowed(&[X]);


pub const Z: Cow<'static, [ [u8; 3] ]> = Cow::Borrowed(&[*b"ABC"]);
// { dg-error ".E0716." "" { target *-*-* } .-1 }

fn main() {}

