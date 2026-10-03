mod a {
    struct Foo;
    impl Foo { pub fn new() {} }
    enum Bar {}
    impl Bar { pub fn new() {} }
}

fn main() {
    a::Foo::new();
// { dg-error ".E0603." "" { target *-*-* } .-1 }
    a::Bar::new();
// { dg-error ".E0603." "" { target *-*-* } .-1 }
}

