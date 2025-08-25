// Lifetime annotation needed because we have no arguments.
fn f() -> &isize {    // { dg-error ".E0106." "" { target *-*-* } }
    panic!()
}

// Lifetime annotation needed because we have two by-reference parameters.
fn g(_x: &isize, _y: &isize) -> &isize {    // { dg-error ".E0106." "" { target *-*-* } }
    panic!()
}

struct Foo<'a> {
    x: &'a isize,
}

// Lifetime annotation needed because we have two lifetimes: one as a parameter
// and one on the reference.
fn h(_x: &Foo) -> &isize { // { dg-error ".E0106." "" { target *-*-* } }
    panic!()
}

fn i(_x: isize) -> &isize { // { dg-error ".E0106." "" { target *-*-* } }
    panic!()
}

// Cases which used to work but now don't.

type StaticStr = &'static str; // hides 'static
trait WithLifetime<'a> {
    type Output; // can hide 'a
}

// This worked because the type of the first argument contains
// 'static, although StaticStr doesn't even have parameters.
fn j(_x: StaticStr) -> &isize { // { dg-error ".E0106." "" { target *-*-* } }
    panic!()
}

// This worked because the compiler resolved the argument type
// to <T as WithLifetime<'a>>::Output which has the hidden 'a.
fn k<'a, T: WithLifetime<'a>>(_x: T::Output) -> &isize {
// { dg-error ".E0106." "" { target *-*-* } .-1 }
    panic!()
}

fn main() {}

