trait A<T=Self> {}

fn f(a: &dyn A) {}
// { dg-error ".E0393." "" { target *-*-* } .-1 }

fn main() {}

