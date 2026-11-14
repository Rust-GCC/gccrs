#![feature(no_core)]
#![no_core]

fn test() -> isize {
    1
}

enum Foo {
    Bar = test() // { dg-error "calls in constants are limited to constant functions, tuple structs and tuple variants" }
}

fn main() {}
