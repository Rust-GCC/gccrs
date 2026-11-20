// We are going to disallow `&a..=b` and `box a..=b` in a pattern. However, the
// older ... syntax is still allowed as a stability guarantee.

#![feature(box_patterns)]
#![warn(ellipsis_inclusive_range_patterns)]

fn main() {
    match Box::new(12) {
        // FIXME: can we add suggestions like `&(0..=9)`?
        box 0...9 => {}
// { dg-warning "" "" { target *-*-* } .-1 }
// { help "" "" { target *-*-* } .-2 }
        box 10..=15 => {}
// { dg-error "" "" { target *-*-* } .-1 }
// { help "" "" { target *-*-* } .-2 }
        box (16..=20) => {}
        _ => {}
    }
}

