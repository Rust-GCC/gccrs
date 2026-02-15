#![feature(exclusive_range_pattern)]

fn main() {
    match 5 {
        6 ..= 1 => { }
        _ => { }
    };
// { dg-error ".E0030." "" { target *-*-* } .-3 }
// { dg-error ".E0030." "" { target *-*-* } .-2 }

    match 5 {
        0 .. 0 => { }
        _ => { }
    };
// { dg-error ".E0579." "" { target *-*-* } .-3 }
// { dg-error ".E0579." "" { target *-*-* } .-2 }

    match 5u64 {
        0xFFFF_FFFF_FFFF_FFFF ..= 1 => { }
        _ => { }
    };
// { dg-error ".E0030." "" { target *-*-* } .-3 }
// { dg-error ".E0030." "" { target *-*-* } .-2 }
}

