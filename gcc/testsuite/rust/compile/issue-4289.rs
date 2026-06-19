// { dg-options "-frust-incomplete-and-experimental-compiler-do-not-use" }
#![feature(no_core)]
#![feature(lang_items)]
#![no_core]

#[lang = "sized"]
pub trait Sized {}

pub fn a() {
    let v = 10;
    let ref mut r = v; // { dg-error "cannot borrow immutable local variable as mutable" }
}

pub fn b() {
    let mut v2 = 10;
    let ref mut r2 = v2; // Should compile fine
}

fn main() {}