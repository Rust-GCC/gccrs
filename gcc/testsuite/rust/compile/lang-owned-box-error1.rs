#![feature(no_core, lang_items, box_syntax)]
#![no_core]

#[lang = "sized"]
pub trait Sized {}

#[lang = "owned_box"]
pub enum EnumBox<T> { // { dg-error "language item must be applied to a struct" }
    Some(T),
    None,
}

fn _foo() {
    let _ = box 5;
}
