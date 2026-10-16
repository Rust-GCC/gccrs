// Each path node in a `use` declaration must be treated as an item. If not, the following code
// will trigger an ICE.
//
// Related issue: #25763

use std::{mem, ptr};
use std::mem; // { dg-error ".E0252." "" { target *-*-* } }

fn main() {}

