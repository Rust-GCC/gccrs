struct Foo {
    x: isize
}

impl Fo { // { dg-error ".E0412." "" { target *-*-* } }
    fn foo() {}
}

fn main() {}

