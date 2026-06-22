#![feature(no_core)]
#![feature(lang_items)]
#![no_core]

#[lang = "sized"]
trait Sized {}

trait LendingIter {
    type Item<'a>;
}

struct Foo; // { dg-warning "struct is never constructed" }

impl LendingIter for Foo {
    type Item<'a> = &'a i32;
}

fn main() {}
