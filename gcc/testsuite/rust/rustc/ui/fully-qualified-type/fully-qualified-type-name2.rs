// Test that we use fully-qualified type names in error messages.

mod x {
    pub enum Foo { }
}

mod y {
    pub enum Foo { }
}

fn bar(x: x::Foo) -> y::Foo {
    return x;
// { dg-error ".E0308." "" { target *-*-* } .-1 }
// { dg-error ".E0308." "" { target *-*-* } .-2 }
}

fn main() {
}

