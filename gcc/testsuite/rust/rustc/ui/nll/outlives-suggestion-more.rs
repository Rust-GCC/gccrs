// Test the more elaborate outlives suggestions.

#![feature(nll)]

// Should suggest: 'a: 'c, 'b: 'd
fn foo1<'a, 'b, 'c, 'd>(x: &'a usize, y: &'b usize) -> (&'c usize, &'d usize) {
    (x, y) // { dg-error "" "" { target *-*-* } }
// { dg-error "" "" { target *-*-* } .-1 }
}

// Should suggest: 'a: 'c and use 'static instead of 'b
fn foo2<'a, 'b, 'c>(x: &'a usize, y: &'b usize) -> (&'c usize, &'static usize) {
    (x, y) // { dg-error "" "" { target *-*-* } }
// { dg-error "" "" { target *-*-* } .-1 }
}

// Should suggest: 'a and 'b are the same and use 'static instead of 'c
fn foo3<'a, 'b, 'c, 'd, 'e>(
    x: &'a usize,
    y: &'b usize,
    z: &'c usize,
) -> (&'b usize, &'a usize, &'static usize) {
    (x, y, z) // { dg-error "" "" { target *-*-* } }
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-error "" "" { target *-*-* } .-2 }
}

fn main() {}

