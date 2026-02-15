use std::num::NonZeroU8 as N8;
use std::num::NonZeroU16 as N16;

#[repr(no_niche)]
pub struct Cloaked(N16);
// { dg-error ".E0658." "" { target *-*-* } .-2 }

#[repr(transparent, no_niche)]
pub struct Shadowy(N16);
// { dg-error ".E0658." "" { target *-*-* } .-2 }

#[repr(no_niche)]
pub enum Cloaked1 { _A(N16), }
// { dg-error ".E0658." "" { target *-*-* } .-2 }

#[repr(no_niche)]
pub enum Cloaked2 { _A(N16), _B(u8, N8) }
// { dg-error ".E0658." "" { target *-*-* } .-2 }

fn main() { }

