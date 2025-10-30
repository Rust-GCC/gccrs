// ignore-cloudabi no std::env support

use std::env;

pub struct Foo {
    text: String
}

pub fn foo() -> Foo {
    let args: Vec<String> = env::args().collect();
    let text = args[1].clone();

    pub Foo { text }
}
// { dg-error "" "" { target *-*-* } .-2 }

pub fn bar() -> Foo {
    fn
    Foo { text: "".to_string() }
}
// { dg-error "" "" { target *-*-* } .-2 }

fn main() {}

