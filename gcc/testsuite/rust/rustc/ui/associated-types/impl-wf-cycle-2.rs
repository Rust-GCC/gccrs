// Regression test for #79714

trait Grault {
    type A;
}

impl<T: Grault> Grault for (T,)
where
    Self::A: Copy,
{
    type A = ();
// { dg-error ".E0275." "" { target *-*-* } .-1 }
}
// { dg-error ".E0275." "" { target *-*-* } .-7 }

fn main() {}

