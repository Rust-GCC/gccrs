// Test that parameter cardinality or missing method error gets span exactly.

pub struct Foo;
impl Foo {
    fn zero(self) -> Foo { self }
    fn one(self, _: isize) -> Foo { self }
    fn two(self, _: isize, _: isize) -> Foo { self }
    fn three<T>(self, _: T, _: T, _: T) -> Foo { self }
}

fn main() {
    let x = Foo;
    x.zero(0)   // { dg-error ".E0061." "" { target *-*-* } }
     .one()     // { dg-error ".E0061." "" { target *-*-* } }
     .two(0);   // { dg-error ".E0061." "" { target *-*-* } }

    let y = Foo;
    y.zero()
     .take()    // { dg-error ".E0599." "" { target *-*-* } }
     .one(0);
    y.three::<usize>(); // { dg-error ".E0061." "" { target *-*-* } }
}

