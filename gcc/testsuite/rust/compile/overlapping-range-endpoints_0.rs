// { dg-additional-options "-frust-unused-check-2.0" }
#![feature(no_core, lang_items)]
#![no_core]

#[lang = "sized"]
pub trait Sized {}

pub fn f(x: i32) {
    match x {
        0..=5 => {}
// { dg-warning "multiple patterns overlap on their endpoints" "" { target *-*-* } .-1 }
        5..=10 => {}
        _ => {}
    }
}
