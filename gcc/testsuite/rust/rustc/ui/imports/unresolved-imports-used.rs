// There should be *one* unused import error.
#![deny(unused_imports)]

mod qux {
   fn quz() {}
   pub fn quy() {}
}

use qux::quz;  // { dg-error ".E0603." "" { target *-*-* } }
use qux::bar;  // { dg-error ".E0432." "" { target *-*-* } }
use foo::bar;  // { dg-error ".E0432." "" { target *-*-* } }
use baz::*;    // { dg-error ".E0432." "" { target *-*-* } }
use qux::bar2; // { dg-error ".E0432." "" { target *-*-* } }
use foo2::bar2;// { dg-error ".E0432." "" { target *-*-* } }
use baz2::*;   // { dg-error ".E0432." "" { target *-*-* } }
use qux::quy;  // { dg-error "" "" { target *-*-* } }

fn main() {}

