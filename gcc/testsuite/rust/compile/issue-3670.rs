#![feature(no_core)]
#![no_core]
pub enum E {
    A = {
        pub enum Foo {
            Bar(isize),
            Baz,
        }
        0
    },
}
