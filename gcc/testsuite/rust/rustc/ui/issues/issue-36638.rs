struct Foo<Self>(Self);
// { dg-error ".E0392." "" { target *-*-* } .-1 }
// { dg-error ".E0392." "" { target *-*-* } .-2 }

trait Bar<Self> {}
// { dg-error "" "" { target *-*-* } .-1 }

fn main() {}

