// This file tests the exhaustiveness algorithm on opaque constants. Most of the examples give
// unnecessary warnings because const_to_pat.rs converts a constant pattern to a wildcard when the
// constant is not allowed as a pattern. This is an edge case so we may not care to fix it.
// See also https://github.com/rust-lang/rust/issues/78057

#![deny(unreachable_patterns)]

#[derive(PartialEq)]
struct Foo(i32);
impl Eq for Foo {}
const FOO: Foo = Foo(42);
const FOO_REF: &Foo = &Foo(42);
const FOO_REF_REF: &&Foo = &&Foo(42);

#[derive(PartialEq)]
struct Bar;
impl Eq for Bar {}
const BAR: Bar = Bar;

#[derive(PartialEq)]
enum Baz {
    Baz1,
    Baz2
}
impl Eq for Baz {}
const BAZ: Baz = Baz::Baz1;

type Quux = fn(usize, usize) -> usize;
fn quux(a: usize, b: usize) -> usize { a + b }
const QUUX: Quux = quux;

fn main() {
    match FOO {
        FOO => {}
// { dg-error "" "" { target *-*-* } .-1 }
        _ => {} // should not be emitting unreachable warning
// { dg-error "" "" { target *-*-* } .-1 }
    }

    match FOO_REF {
        FOO_REF => {}
// { dg-error "" "" { target *-*-* } .-1 }
        Foo(_) => {} // should not be emitting unreachable warning
// { dg-error "" "" { target *-*-* } .-1 }
    }

    // This used to cause an ICE (https://github.com/rust-lang/rust/issues/78071)
    match FOO_REF_REF {
        FOO_REF_REF => {}
// { dg-warning "" "" { target *-*-* } .-1 }
// { dg-warning "" "" { target *-*-* } .-2 }
        Foo(_) => {}
    }

    match BAR {
        Bar => {}
        BAR => {} // should not be emitting unreachable warning
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-error "" "" { target *-*-* } .-2 }
        _ => {}
// { dg-error "" "" { target *-*-* } .-1 }
    }

    match BAR {
        BAR => {}
// { dg-error "" "" { target *-*-* } .-1 }
        Bar => {} // should not be emitting unreachable warning
// { dg-error "" "" { target *-*-* } .-1 }
        _ => {}
// { dg-error "" "" { target *-*-* } .-1 }
    }

    match BAR {
        BAR => {}
// { dg-error "" "" { target *-*-* } .-1 }
        BAR => {} // should not be emitting unreachable warning
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-error "" "" { target *-*-* } .-2 }
        _ => {} // should not be emitting unreachable warning
// { dg-error "" "" { target *-*-* } .-1 }
    }

    match BAZ {
        BAZ => {}
// { dg-error "" "" { target *-*-* } .-1 }
        Baz::Baz1 => {} // should not be emitting unreachable warning
// { dg-error "" "" { target *-*-* } .-1 }
        _ => {}
// { dg-error "" "" { target *-*-* } .-1 }
    }

    match BAZ {
        Baz::Baz1 => {}
        BAZ => {}
// { dg-error "" "" { target *-*-* } .-1 }
        _ => {}
// { dg-error "" "" { target *-*-* } .-1 }
    }

    match BAZ {
        BAZ => {}
// { dg-error "" "" { target *-*-* } .-1 }
        Baz::Baz2 => {} // should not be emitting unreachable warning
// { dg-error "" "" { target *-*-* } .-1 }
        _ => {} // should not be emitting unreachable warning
// { dg-error "" "" { target *-*-* } .-1 }
    }

    match QUUX {
        QUUX => {}
        QUUX => {}
        _ => {}
    }
}

