#![feature(lang_items)]

#[lang = "sized"]
trait Sized {}

pub enum Option<T> {
    Some(T),
    None,
}

trait Foo {
    type Bar<T>;

    fn foo<T>(self) -> Self::Bar<T>;
}

impl Foo for i32 {
    type Bar<T> = Option<T>;

    fn foo<T>(self) -> Self::Bar<T> {
        Option::None
    }
}

pub fn main() -> i32 {
    let a = 15;
    let res: Option<i8> = a.foo::<i8>();

    match res {
        Option::None => 0,
        Option::Some(_) => 1,
    }
}
