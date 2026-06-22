#![feature(no_core)]
#![feature(lang_items)]
#![no_core]

#[lang = "sized"]
trait Sized {}

trait Foo {
    type Bar<T>;
    fn put<T>(self, x: Self::Bar<T>) -> Self::Bar<T>;
}

impl Foo for i32 {
    type Bar<T> = T;
    fn put<T>(self, x: Self::Bar<T>) -> Self::Bar<T> {
        x
    }
}

fn main() {
    let x: i32 = 1;
    let _r = x.put::<i64>(2i64);
}
