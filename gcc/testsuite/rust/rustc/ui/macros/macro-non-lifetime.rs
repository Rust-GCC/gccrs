// Test for issue #50381: non-lifetime passed to :lifetime.

#![feature(macro_lifetime_matcher)]

macro_rules! m { ($x:lifetime) => { } }

fn main() {
    m!(a);
// { dg-error "" "" { target *-*-* } .-1 }
}

