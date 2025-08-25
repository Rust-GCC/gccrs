// edition:2018
// compile-flags:--extern foo --extern bar

use foo::bar; // { dg-error ".E0463." "" { target *-*-* } }
use bar::foo;

fn main() {}

