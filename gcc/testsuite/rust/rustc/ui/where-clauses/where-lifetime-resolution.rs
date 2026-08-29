trait Trait1<'a> {}
trait Trait2<'a, 'b> {}

fn f() where
    for<'a> dyn Trait1<'a>: Trait1<'a>, // OK
    (dyn for<'a> Trait1<'a>): Trait1<'a>,
// { dg-error ".E0261." "" { target *-*-* } .-1 }
    for<'a> dyn for<'b> Trait2<'a, 'b>: Trait2<'a, 'b>,
// { dg-error ".E0261." "" { target *-*-* } .-1 }
{}

fn main() {}

