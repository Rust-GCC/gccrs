struct Foo<T: ?Hash> { }
// { dg-error ".E0392." "" { target *-*-* } .-1 }
// { dg-error ".E0392." "" { target *-*-* } .-2 }
// { dg-warning ".E0392." "" { target *-*-* } .-3 }

fn main() { }

