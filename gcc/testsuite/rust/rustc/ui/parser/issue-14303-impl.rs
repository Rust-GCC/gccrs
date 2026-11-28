struct X<T>(T);

impl<'a, T, 'b> X<T> {}
// { dg-error "" "" { target *-*-* } .-1 }

fn main() {}

