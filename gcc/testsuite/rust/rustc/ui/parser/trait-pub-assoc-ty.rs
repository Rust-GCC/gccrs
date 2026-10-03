trait Foo {
    pub type Foo;
// { dg-error ".E0449." "" { target *-*-* } .-1 }
}

fn main() {}

