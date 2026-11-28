// aux-build:coherence_orphan_lib.rs
#![feature(negative_impls)]

extern crate coherence_orphan_lib as lib;

use lib::TheTrait;

struct TheType;

impl TheTrait<usize> for isize { }
// { dg-error ".E0117." "" { target *-*-* } .-1 }

impl TheTrait<TheType> for isize { }

impl TheTrait<isize> for TheType { }

impl !Send for Vec<isize> { }
// { dg-error ".E0117." "" { target *-*-* } .-1 }

fn main() { }

