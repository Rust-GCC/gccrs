#![feature(no_core, lang_items)]
#![no_core]

#[lang = "sized"]
pub trait Sized {}

static mut S: i32 = 4;
// { dg-message "not initialized with a constant expression" "" { target *-*-* } .-1 }

const X: i32 = unsafe { S }; // { dg-error "evaluation of constant value failed .E0080." }

pub fn f() -> i32 {
    X
}
