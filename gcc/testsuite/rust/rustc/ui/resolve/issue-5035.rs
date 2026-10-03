trait I {}
type K = dyn I;
impl K for isize {} // { dg-error ".E0404." "" { target *-*-* } }

use ImportError; // { dg-error ".E0432." "" { target *-*-* } }
// { dg-error ".E0432." "" { target *-*-* } .-1 }
impl ImportError for () {} // check that this is not an additional error (cf. issue #35142)

fn main() {}

