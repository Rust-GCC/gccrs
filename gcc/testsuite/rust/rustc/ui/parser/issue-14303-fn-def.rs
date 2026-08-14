fn foo<'a, T, 'b>(x: &'a T) {}
// { dg-error "" "" { target *-*-* } .-1 }

fn main() {}

