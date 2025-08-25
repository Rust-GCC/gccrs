mod inner {
    pub struct Foo;

    impl Foo {
        fn method(&self) {}
    }
}

fn main() {
    let foo = inner::Foo;
    foo.method(); // { dg-error ".E0624." "" { target *-*-* } }
}

