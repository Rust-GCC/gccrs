#![feature(nll)]

// Test that we are NOT able to establish that `<T as
// MyTrait<'a>>::Output: 'a` outlives `'a` here -- we have only one
// recourse, which is to prove that `T: 'a` and `'a: 'a`, but we don't
// know that `T: 'a`.

trait MyTrait<'a> {
    type Output;
}

fn foo<'a, T>() -> &'a ()
where
    T: MyTrait<'a>,
{
    bar::<T::Output>() // { dg-error ".E0309." "" { target *-*-* } }
}

fn bar<'a, T>() -> &'a ()
where
    T: 'a,
{
    &()
}

fn main() {}

