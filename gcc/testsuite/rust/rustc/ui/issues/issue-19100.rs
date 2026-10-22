// run-pass
// run-rustfix

#![allow(non_snake_case)]
#![allow(dead_code)]
#![allow(unused_variables)]

#[derive(Copy, Clone)]
enum Foo {
    Bar,
    Baz
}

impl Foo {
    fn foo(&self) {
        match self {
            &
Bar if true
// { dg-warning "" "" { target *-*-* } .-1 }
=> println!("bar"),
            &
Baz if false
// { dg-warning "" "" { target *-*-* } .-1 }
=> println!("baz"),
_ => ()
        }
    }
}

fn main() {}

