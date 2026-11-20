struct Foo<T>(T, *const ());

unsafe impl Send for Foo<T> {}
// { dg-error ".E0412." "" { target *-*-* } .-1 }

fn main() {}

