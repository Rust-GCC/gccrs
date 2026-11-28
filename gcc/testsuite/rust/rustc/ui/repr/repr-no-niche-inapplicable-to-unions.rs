#![feature(no_niche)]

use std::num::NonZeroU8 as N8;
use std::num::NonZeroU16 as N16;

#[repr(no_niche)]
pub union Cloaked1 { _A: N16 }
// { dg-error ".E0517." "" { target *-*-* } .-2 }

#[repr(no_niche)]
pub union Cloaked2 { _A: N16, _B: (u8, N8) }
// { dg-error ".E0517." "" { target *-*-* } .-2 }

fn main() { }

