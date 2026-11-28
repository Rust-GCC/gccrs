struct Foo<T, U = [u8; std::mem::size_of::<T>()]>(T, U);
// { dg-error "" "" { target *-*-* } .-1 }

fn main() {}

