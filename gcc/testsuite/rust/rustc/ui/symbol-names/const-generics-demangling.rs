// build-fail
// compile-flags: -Z symbol-mangling-version=v0

#![feature(min_const_generics, rustc_attrs)]

pub struct Unsigned<const F: u8>;

#[rustc_symbol_name]
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-error "" "" { target *-*-* } .-2 }
// { dg-error "" "" { target *-*-* } .-3 }
impl Unsigned<11> {}

pub struct Signed<const F: i16>;

#[rustc_symbol_name]
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-error "" "" { target *-*-* } .-2 }
// { dg-error "" "" { target *-*-* } .-3 }
impl Signed<-152> {}

pub struct Bool<const F: bool>;

#[rustc_symbol_name]
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-error "" "" { target *-*-* } .-2 }
// { dg-error "" "" { target *-*-* } .-3 }
impl Bool<true> {}

pub struct Char<const F: char>;

#[rustc_symbol_name]
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-error "" "" { target *-*-* } .-2 }
// { dg-error "" "" { target *-*-* } .-3 }
impl Char<'∂'> {}

fn main() {}

