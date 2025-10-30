// In expression, `&a..=b` is treated as `(&a)..=(b)` and `box a..=b` is
// `(box a)..=(b)`. In a pattern, however, `&a..=b` means `&(a..=b)`. This may
// lead to confusion.

// run-rustfix

#![warn(ellipsis_inclusive_range_patterns)]

pub fn main() {
    match &12 {
        &0...9 => {}
// { dg-warning "" "" { target *-*-* } .-1 }
// { help "" "" { target *-*-* } .-2 }
        &10..=15 => {}
// { dg-error "" "" { target *-*-* } .-1 }
// { help "" "" { target *-*-* } .-2 }
        &(16..=20) => {}
        _ => {}
    }
}

