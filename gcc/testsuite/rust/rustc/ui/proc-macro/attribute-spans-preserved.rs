// aux-build:attribute-spans-preserved.rs

extern crate attribute_spans_preserved as foo;

use foo::foo;

#[ foo ( let y: u32 = "z"; ) ] // { dg-error ".E0308." "" { target *-*-* } }
#[ bar { let x: u32 = "y"; } ] // { dg-error ".E0308." "" { target *-*-* } }
fn main() {
}

