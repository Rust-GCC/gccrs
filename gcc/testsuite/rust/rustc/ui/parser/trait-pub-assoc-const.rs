trait Foo {
    pub const Foo: u32;
// { dg-error ".E0449." "" { target *-*-* } .-1 }
}

fn main() {}

