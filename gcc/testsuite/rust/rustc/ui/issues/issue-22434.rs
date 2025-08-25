pub trait Foo {
    type A;
}

type I<'a> = &'a (dyn Foo + 'a);
// { dg-error ".E0191." "" { target *-*-* } .-1 }

fn main() {}

