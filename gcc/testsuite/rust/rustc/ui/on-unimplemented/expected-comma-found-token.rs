// Tests that two closures cannot simultaneously have mutable
// access to the variable, whether that mutable access be used
// for direct assignment or for taking mutable ref. Issue #6801.

#![feature(rustc_attrs)]

#[rustc_on_unimplemented(
    message="the message"
    label="the label" // { dg-error "" "" { target *-*-* } }
)]
trait T {}

fn main() {  }

