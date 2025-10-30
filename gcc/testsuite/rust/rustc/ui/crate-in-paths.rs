// edition:2018

#![feature(crate_visibility_modifier)]

mod bar {
    crate struct Foo;
}

fn main() {
    Foo;
// { dg-error ".E0425." "" { target *-*-* } .-1 }
}

