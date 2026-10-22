// Checks that immutable static items can't have mutable slices

static TEST: &'static mut [isize] = &mut [];
// { dg-error ".E0764." "" { target *-*-* } .-1 }

pub fn main() { }

