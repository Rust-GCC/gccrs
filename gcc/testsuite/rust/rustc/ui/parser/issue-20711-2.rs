struct Foo;

impl Foo {
    fn foo() {}

    #[stable(feature = "rust1", since = "1.0.0")]
// { dg-error "" "" { target *-*-* } .-1 }
}

fn main() {}

