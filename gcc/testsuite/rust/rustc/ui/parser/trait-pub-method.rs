trait Foo {
    pub fn foo();
// { dg-error ".E0449." "" { target *-*-* } .-1 }
}

fn main() {}

