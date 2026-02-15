struct Foo;

impl Foo {
    const bar: bool = true;
    fn bar() {} // { dg-error ".E0201." "" { target *-*-* } }
}

fn main() {}

