struct Heap;

struct Vec<A = Heap, T>(A, T);
// { dg-error "" "" { target *-*-* } .-1 }

struct Foo<A, B = Vec<C>, C>(A, B, C);
// { dg-error ".E0128." "" { target *-*-* } .-1 }
// { dg-error ".E0128." "" { target *-*-* } .-2 }

fn main() {}

