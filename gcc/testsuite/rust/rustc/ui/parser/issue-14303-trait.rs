trait Foo<'a, T, 'b> {}
// { dg-error "" "" { target *-*-* } .-1 }

fn main() {}

