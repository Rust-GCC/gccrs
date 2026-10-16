// Make sure that inclusive ranges with `...` syntax don't parse.

use std::ops::RangeToInclusive;

fn return_range_to() -> RangeToInclusive<i32> {
    return ...1; // { dg-error "" "" { target *-*-* } }
// { help "" "" { target *-*-* } .-1 }
// { help "" "" { target *-*-* } .-2 }
}

pub fn main() {
    let x = ...0;    // { dg-error "" "" { target *-*-* } }
// { help "" "" { target *-*-* } .-1 }
// { help "" "" { target *-*-* } .-2 }

    let x = 5...5;   // { dg-error "" "" { target *-*-* } }
// { help "" "" { target *-*-* } .-1 }
// { help "" "" { target *-*-* } .-2 }

    for _ in 0...1 {} // { dg-error "" "" { target *-*-* } }
// { help "" "" { target *-*-* } .-1 }
// { help "" "" { target *-*-* } .-2 }
}

