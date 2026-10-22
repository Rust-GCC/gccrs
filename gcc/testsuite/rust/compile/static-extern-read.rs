#![feature(no_core, lang_items)]
#![no_core]

#[lang = "sized"]
pub trait Sized {}

extern "C" {
    static E: i32;
}

pub static S: i32 = unsafe { E }; // { dg-error "evaluation of constant value failed .E0080." }
