// aux-build:orphan-check-diagnostics.rs

// See issue #22388.

extern crate orphan_check_diagnostics;

use orphan_check_diagnostics::RemoteTrait;

trait LocalTrait { fn dummy(&self) { } }

impl<T> RemoteTrait for T where T: LocalTrait {}
// { dg-error ".E0210." "" { target *-*-* } .-1 }

fn main() {}

