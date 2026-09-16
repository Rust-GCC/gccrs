// We don't need those errors. Ideally we would silence them, but to do so we need to move the
// lint from being an early-lint during parsing to a late-lint, because it needs to be aware of
// the types involved.
#![allow(bare_trait_objects)]

struct Foo;

fn foo(_x: Box<Foo + Send>) { } // { dg-error ".E0404." "" { target *-*-* } }

type TypeAlias<T> = Box<dyn Vec<T>>; // { dg-error ".E0404." "" { target *-*-* } }

struct A;
fn a() -> A + 'static { // { dg-error ".E0404." "" { target *-*-* } }
    A
}
fn b<'a,T,E>(iter: Iterator<Item=Result<T,E> + 'a>) { // { dg-error ".E0404." "" { target *-*-* } }
    panic!()
}
fn c() -> 'static + A { // { dg-error ".E0404." "" { target *-*-* } }
    A
}
fn d<'a,T,E>(iter: Iterator<Item='a + Result<T,E>>) { // { dg-error ".E0404." "" { target *-*-* } }
    panic!()
}
fn e() -> 'static + A + 'static { // { dg-error ".E0404." "" { target *-*-* } }
// { dg-error ".E0404." "" { target *-*-* } .-1 }
    A
}
fn f<'a,T,E>(iter: Iterator<Item='a + Result<T,E> + 'a>) { // { dg-error ".E0404." "" { target *-*-* } }
// { dg-error ".E0404." "" { target *-*-* } .-1 }
    panic!()
}
struct Traitor;
trait Trait {}
fn g() -> Traitor + 'static { // { dg-error ".E0404." "" { target *-*-* } }
    A
}
fn main() {}

