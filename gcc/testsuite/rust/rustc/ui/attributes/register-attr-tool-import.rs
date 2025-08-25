// edition:2018
// compile-flags: -Zsave-analysis
// ~^ Also regression test for #69588

#![feature(register_attr)]
#![feature(register_tool)]

#![register_attr(attr)]
#![register_tool(tool)]

use attr as renamed_attr; // OK
use tool as renamed_tool; // OK

#[renamed_attr] // { dg-error "" "" { target *-*-* } }
#[renamed_tool::attr] // { dg-error "" "" { target *-*-* } }
// { dg-error "" "" { target *-*-* } .-2 }
fn main() {}

