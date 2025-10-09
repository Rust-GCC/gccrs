use std::ops::{Add, Sub, Mul, Div};
trait X<Rhs>: Mul<Rhs> + Div<Rhs> {}
trait Y<Rhs>: Div<Rhs, Output = Rhs> {
    type A;
}
trait Z<Rhs>: Div<Rhs> {
    type A;
    type B;
}
trait Fine<Rhs>: Div<Rhs, Output = Rhs> {}

type Foo<Rhs> = dyn Add<Rhs> + Sub<Rhs> + X<Rhs> + Y<Rhs>;
// { dg-error ".E0225." "" { target *-*-* } .-1 }
// { dg-error ".E0191." "" { target *-*-* } .-2 }
type Bar<Rhs> = dyn Add<Rhs> + Sub<Rhs> + X<Rhs> + Z<Rhs>;
// { dg-error ".E0225." "" { target *-*-* } .-1 }
// { dg-error ".E0191." "" { target *-*-* } .-2 }
type Baz<Rhs> = dyn Add<Rhs> + Sub<Rhs> + Y<Rhs>;
// { dg-error ".E0225." "" { target *-*-* } .-1 }
// { dg-error ".E0191." "" { target *-*-* } .-2 }
type Bat<Rhs> = dyn Add<Rhs> + Sub<Rhs> + Fine<Rhs>;
// { dg-error ".E0191." "" { target *-*-* } .-1 }
// { dg-error ".E0191." "" { target *-*-* } .-2 }
type Bal<Rhs> = dyn X<Rhs>;
// { dg-error ".E0191." "" { target *-*-* } .-1 }

fn main() {}

