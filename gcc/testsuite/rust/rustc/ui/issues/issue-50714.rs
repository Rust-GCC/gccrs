// Regression test for issue 50714, make sure that this isn't a linker error.

fn main() where fn(&()): Eq {} // { dg-error ".E0646." "" { target *-*-* } }

