struct Foo;
#[derive(Copy, Clone)]
// { dg-error ".E0204." "" { target *-*-* } .-1 }
struct Bar(Foo);

fn main() {}

