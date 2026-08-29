// Test a cycle where a type parameter on a trait has a default that
// again references the trait.

trait Foo<X = Box<dyn Foo>> {
// { dg-error ".E0391." "" { target *-*-* } .-1 }
// { dg-error ".E0391." "" { target *-*-* } .-2 }
}

fn main() { }

