#![feature(no_core)]
#![feature(lang_items)]
#![no_core]
#[lang = "sized"]
pub trait Sized {}
#[lang = "copy"]
pub trait Copy {}
#[lang = "drop"]
pub trait Drop {
    fn drop(&mut self);
}

struct Foo {
    x: i32,
}

impl Drop for Foo {
    fn drop(&mut self) {}
}

fn main() {
    let mut x = Foo { x: -7 };
    x.drop(); // { dg-error "explicit use of destructor method .E0040." }
}
