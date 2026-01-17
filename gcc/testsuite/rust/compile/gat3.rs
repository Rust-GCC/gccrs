#![feature(lang_items)]

#[lang = "sized"]
trait Sized {}

trait Foo {
    type Bar<T>;

    fn foo<T>(self, value: T) -> Self::Bar<T>;
}

impl Foo for i32 {
    type Bar<T> = T;

    fn foo<T>(self, value: T) -> Self::Bar<T> {
        value
    }
}

fn main() {
    let a = 15;

    let _b = a.foo::<i8>(14i8);
}
