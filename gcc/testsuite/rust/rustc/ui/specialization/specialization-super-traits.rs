// run-pass

#![feature(specialization)] // { dg-warning "" "" { target *-*-* } }

// Test that you can specialize via an explicit trait hierarchy

// FIXME: this doesn't work yet...

trait Parent {}
trait Child: Parent {}

trait Foo {}

impl<T: Parent> Foo for T {}
impl<T: Child> Foo for T {}

fn main() {}

