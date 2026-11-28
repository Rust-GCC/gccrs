#![feature(half_open_range_patterns)]
#![feature(exclusive_range_pattern)]

fn main() {
    let "a".. = "a"; // { dg-error ".E0029." "" { target *-*-* } }
    let .."a" = "a"; // { dg-error ".E0029." "" { target *-*-* } }
    let ..="a" = "a"; // { dg-error ".E0029." "" { target *-*-* } }
}

