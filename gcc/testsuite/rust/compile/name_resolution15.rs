#![feature(decl_macro)]

pub mod foo {
    pub struct Foo {
        pub a: i32,
    }
    pub fn Foo() {}
    pub macro Foo() {{}}
}

pub use foo::Foo;

use self::Foo as Fo;

fn main() {
    let a = Fo();
    let b = Fo { a: 15 };
    let c = Fo!();
}
