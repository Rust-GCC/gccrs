// Regression test for issue #48276 - ICE when self type does not match what is
// required by a trait and regions are involved.

trait MyFrom<A> {
    fn from(a: A) -> Self;
}

struct A;

impl<'a, 'b> MyFrom<A> for &'a str {
    fn from(self: &'a Self) -> &'b str {
// { dg-error ".E0185." "" { target *-*-* } .-1 }
        "asdf"
    }
}

struct B;

impl From<A> for B {
    fn from(&self) -> B {
// { dg-error ".E0185." "" { target *-*-* } .-1 }
        B
    }
}

impl From<A> for &'static str {
    fn from(&self) -> &'static str {
// { dg-error ".E0185." "" { target *-*-* } .-1 }
        ""
    }
}

fn main(){}

