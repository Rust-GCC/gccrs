// Regression test for #79714

trait Baz {}
impl Baz for () {}
impl<T> Baz for (T,) {}

trait Fiz {}
impl Fiz for bool {}

trait Grault {
    type A;
    type B;
}

impl<T: Grault> Grault for (T,)
where
    Self::A: Baz,
    Self::B: Fiz,
{
    type A = ();
// { dg-error ".E0275." "" { target *-*-* } .-1 }
    type B = bool;
// { dg-error ".E0275." "" { target *-*-* } .-1 }
}
// { dg-error ".E0275." "" { target *-*-* } .-10 }

fn main() {
    let x: <(_,) as Grault>::A = ();
}

