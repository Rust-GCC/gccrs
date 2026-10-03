fn main() {
    for<'a> |x: &'a u8| *x + 1;
// { dg-error "" "" { target *-*-* } .-1 }
}

enum Foo { Bar }
fn foo(x: impl Iterator<Item = Foo>) {
    for <Foo>::Bar in x {}
// { dg-error "" "" { target *-*-* } .-1 }
}

