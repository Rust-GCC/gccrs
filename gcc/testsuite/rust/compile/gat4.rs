#![feature(no_core)]
#![feature(lang_items)]
#![no_core]

#[lang = "sized"]
trait Sized {}

pub struct Pair<A, B>(A, B);

trait Foo {
    type Out<A, B>;
    fn build<A, B>(self, a: A, b: B) -> Self::Out<A, B>;
}

impl Foo for i32 {
    type Out<A, B> = Pair<A, B>;
    fn build<A, B>(self, a: A, b: B) -> Self::Out<A, B> {
        Pair(a, b)
    }
}

fn main() {
    let x: i32 = 1;
    let _p = x.build::<i32, i64>(2, 3i64);
}
