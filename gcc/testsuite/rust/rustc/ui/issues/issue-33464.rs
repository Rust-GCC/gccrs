// Make sure that the spans of import errors are correct.

use abc::one_el;
// { dg-error ".E0432." "" { target *-*-* } .-1 }
use abc::{a, bbb, cccccc};
// { dg-error ".E0432." "" { target *-*-* } .-1 }
use a_very_long_name::{el, el2};
// { dg-error ".E0432." "" { target *-*-* } .-1 }

fn main() {}

