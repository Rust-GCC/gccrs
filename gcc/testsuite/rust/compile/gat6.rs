#![feature(no_core)]
#![feature(lang_items)]
#![no_core]

#[lang = "sized"]
trait Sized {}

trait Foo {
    type Bar<T>;
    fn nest<T>(self, x: T) -> Self::Bar<Self::Bar<T>>;
}

impl Foo for i32 {
    type Bar<T> = T;
    fn nest<T>(self, x: T) -> Self::Bar<Self::Bar<T>> {
        x
    }
}

fn main() {
    let x: i32 = 1;
    let _r = x.nest::<i64>(5i64);
}
