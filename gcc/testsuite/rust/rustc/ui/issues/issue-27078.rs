#![feature(associated_consts)]

trait Foo {
    const BAR: i32;
    fn foo(self) -> &'static i32 {
// { dg-error ".E0277." "" { target *-*-* } .-1 }
        &<Self>::BAR
    }
}

fn main() {}

