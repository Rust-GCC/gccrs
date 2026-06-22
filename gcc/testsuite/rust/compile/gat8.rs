#![feature(no_core)]
#![feature(lang_items)]
#![no_core]

#[lang = "sized"]
trait Sized {}

trait Foo {
    type Bar<T>;
    fn make<T>(self) -> Self::Bar<T>;
}

impl Foo for i32 {
    type Bar<T> = T;
    fn make<T>(self) -> Self::Bar<T> { loop {} }
}

fn main() {
    let x: i32 = 1;
    let _r = x.make::<i32, i64>(); // { dg-error "generic item takes at most 1 type arguments but 2 were supplied" }
}
