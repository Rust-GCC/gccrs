// Check that projections don't count as constraining type parameters.

struct S<T>(T);

trait Tr { type Assoc; fn test(); }

impl<T: Tr> S<T::Assoc> {
// { dg-error ".E0207." "" { target *-*-* } .-1 }
    fn foo(self, _: T) {
        T::test();
    }
}

trait Trait1<T> { type Bar; }
trait Trait2<'x> { type Foo; }

impl<'a,T: Trait2<'a>> Trait1<<T as Trait2<'a>>::Foo> for T {
// { dg-error ".E0207." "" { target *-*-* } .-1 }
    type Bar = &'a ();
}

fn main() {}

