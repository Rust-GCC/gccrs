// Check that the user gets an error if they omit a binding from an
// object type.

pub trait Foo {
    type A;
    type B;
    fn boo(&self) -> <Self as Foo>::A;
}

struct Bar;

impl Foo for isize {
    type A = usize;
    type B = char;
    fn boo(&self) -> usize {
        42
    }
}

pub fn main() {
    let a = &42isize as &dyn Foo<A=usize, B=char>;

    let b = &42isize as &dyn Foo<A=usize>;
// { dg-error ".E0191." "" { target *-*-* } .-1 }

    let c = &42isize as &dyn Foo<B=char>;
// { dg-error ".E0191." "" { target *-*-* } .-1 }

    let d = &42isize as &dyn Foo;
// { dg-error ".E0191." "" { target *-*-* } .-1 }
}

