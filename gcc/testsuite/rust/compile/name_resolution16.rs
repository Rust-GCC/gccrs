#![feature(decl_macro)]

pub mod foo {
    pub struct Foo {
        pub a: i32,
    }
    pub fn Foo() {}
    pub macro Foo() {{}}
}

pub use foo::Foo;

fn main() {
    let a = Foo();
    let b = Foo { a: 15 };
    let c = Foo!();
}
