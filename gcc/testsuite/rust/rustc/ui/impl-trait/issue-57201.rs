// Regression test for #57201
// FIXME: The error is temporary hack, we'll revisit here at some point.

#![feature(impl_trait_in_bindings)]
#![allow(incomplete_features)]

fn bug<'a, 'b, T>()
where
    'a: 'b,
{
    let f: &impl Fn(&'a T) -> &'b T = &|x| x;
// { dg-error "" "" { target *-*-* } .-1 }
}

fn main() {}

