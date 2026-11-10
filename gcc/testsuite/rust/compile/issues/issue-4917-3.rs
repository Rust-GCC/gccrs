#![feature(no_core, lang_items)]
#![no_core]

// Rust-GCC/gccrs#4917: a bound on one of the item's own generics is not a
// bound on Self, so it has to stay on that generic.

#[lang = "sized"]
pub trait Sized {}

pub trait Marker {}

pub trait Picker {
    fn pick<T>(self, t: T) -> T
    where
        Self: Sized,
        T: Marker,
    {
        t
    }
}

pub struct Forward;

impl Picker for Forward {}

pub struct Tagged;

impl Marker for Tagged {}

fn main() {
    let _ = Forward.pick(Tagged);
    let _ = Forward.pick(Forward);
    // { dg-error "not satisfied" "" { target *-*-* } .-1 }
    // { dg-error "failed to lookup type" "" { target *-*-* } .-2 }
}
