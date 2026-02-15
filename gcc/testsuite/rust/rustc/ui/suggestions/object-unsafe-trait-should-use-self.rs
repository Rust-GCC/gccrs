#![allow(bare_trait_objects)]
trait A: Sized {
    fn f(a: A) -> A;
// { dg-error ".E0038." "" { target *-*-* } .-1 }
// { dg-error ".E0038." "" { target *-*-* } .-2 }
}
trait B {
    fn f(a: B) -> B;
// { dg-error ".E0038." "" { target *-*-* } .-1 }
// { dg-error ".E0038." "" { target *-*-* } .-2 }
}
trait C {
    fn f(&self, a: C) -> C;
}

fn main() {}

