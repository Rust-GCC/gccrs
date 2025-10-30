// Test that Cell is considered invariant with respect to its
// type.

use std::cell::Cell;

struct Foo<'a> {
    x: Cell<Option<&'a isize>>,
}

fn use_<'short,'long>(c: Foo<'short>,
                      s: &'short isize,
                      l: &'long isize,
                      _where:Option<&'short &'long ()>) {
    let _: Foo<'long> = c; // { dg-error ".E0623." "" { target *-*-* } }
}

fn main() {
}

