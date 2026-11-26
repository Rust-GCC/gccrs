#![feature(no_core)]
#![no_core]
#![feature(lang_items)]

#[lang = "sized"]
pub trait Sized {}

pub trait Foo {
    type bar;
    fn bar(&self) {}
}

impl Foo for char {
    type bar = u8;
}

fn main() {
    'a'.bar()
}