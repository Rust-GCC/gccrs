// The raw_pointer_derived lint was removed, but is now reported by
// the renamed_and_removed_lints lint, which means it's a warning by
// default, and allowed in cargo dependency builds.
// cc #30346

#[deny(raw_pointer_derive)] // { dg-warning "" "" { target *-*-* } }
#[deny(unused_variables)]
fn main() { let unused = (); } // { dg-error "" "" { target *-*-* } }

