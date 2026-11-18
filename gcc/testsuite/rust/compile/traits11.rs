#![feature(no_core)]
#![no_core]

#![feature(lang_items)]
#[lang = "sized"]
pub trait Sized {}

struct Foo(i32);

trait A {
    const A: i32 = 123;
    fn B();
    fn C(&self);
}

trait B: A {
    fn test(&self);
}

pub fn main() {
    let a;
    a = Foo(123);

    let _: &dyn B = &a;
    // { dg-warning "trait bound is not object safe" "" { target *-*-* } .-1 }
}
