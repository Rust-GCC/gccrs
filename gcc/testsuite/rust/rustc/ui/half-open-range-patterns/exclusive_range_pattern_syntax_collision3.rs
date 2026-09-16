#![feature(half_open_range_patterns)]
#![feature(exclusive_range_pattern)]

fn main() {
    match [5..4, 99..105, 43..44] {
        [..9, 99..100, _] => {},
// { dg-error ".E0308." "" { target *-*-* } .-1 }
// { dg-error ".E0308." "" { target *-*-* } .-2 }
// { dg-error ".E0308." "" { target *-*-* } .-3 }
        _ => {},
    }
}

