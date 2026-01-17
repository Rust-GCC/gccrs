#![feature(lang_items)]
#![feature(no_core)]
#![no_core]

#[lang = "sized"]
pub trait Sized {}

trait Foo {
    type Bar<T>;
    type Baz<'a>;
}
