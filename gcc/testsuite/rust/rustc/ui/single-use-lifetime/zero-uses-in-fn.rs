// run-rustfix

// Test that we DO warn when lifetime name is not used at all.

#![deny(unused_lifetimes)]
#![allow(dead_code, unused_variables)]

fn september<'a>() {}
// { dg-error "" "" { target *-*-* } .-1 }
// { help "" "" { target *-*-* } .-2 }

fn october<'a, 'b, T>(s: &'b T) -> &'b T {
// { dg-error "" "" { target *-*-* } .-1 }
// { help "" "" { target *-*-* } .-2 }
    s
}

fn november<'a, 'b>(s: &'a str) -> &'a str {
// { dg-error "" "" { target *-*-* } .-1 }
// { help "" "" { target *-*-* } .-2 }
    s
}

fn main() {}

