// This test checks that a suggestion to add a `self: ` parameter name is provided
// to functions where this is applicable.

pub fn foo(Box<Self>) { }
// { dg-error "" "" { target *-*-* } .-1 }

struct Bar;

impl Bar {
    fn bar(Box<Self>) { }
// { dg-error "" "" { target *-*-* } .-1 }
}

fn main() { }

