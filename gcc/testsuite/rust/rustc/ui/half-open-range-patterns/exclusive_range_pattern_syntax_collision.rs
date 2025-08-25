#![feature(half_open_range_patterns)]
#![feature(exclusive_range_pattern)]

fn main() {
    match [5..4, 99..105, 43..44] {
        [_, 99.., _] => {},
// { dg-error ".E0308." "" { target *-*-* } .-1 }
        _ => {},
    }
}

