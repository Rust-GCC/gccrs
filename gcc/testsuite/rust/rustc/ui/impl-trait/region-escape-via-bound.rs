// Test that we do not allow the region `'x` to escape in the impl
// trait **even though** `'y` escapes, which outlives `'x`.
//
// See https://github.com/rust-lang/rust/issues/46541 for more details.

#![allow(dead_code)]
#![feature(in_band_lifetimes)]

use std::cell::Cell;

trait Trait<'a> { }

impl Trait<'b> for Cell<&'a u32> { }

fn foo(x: Cell<&'x u32>) -> impl Trait<'y>
// { dg-error ".E0700." "" { target *-*-* } .-1 }
where 'x: 'y
{
    x
}

fn main() { }

