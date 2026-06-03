#![feature(no_core, lang_items, box_syntax)]
#![no_core]

#[lang = "sized"]
pub trait Sized {}

#[lang = "owned_box"]
pub struct BadBox;

fn _foo() {
    let _ = box 5; // { dg-error "lang item must be applied to a struct with at least 1 generic argument" }
}
