// compile-flags: -Zunleash-the-miri-inside-of-you
// aux-build:static_cross_crate.rs
#![allow(const_err)]

#![feature(exclusive_range_pattern, half_open_range_patterns)]

extern crate static_cross_crate;

// Sneaky: reference to a mutable static.
// Allowing this would be a disaster for pattern matching, we could violate exhaustiveness checking!
const SLICE_MUT: &[u8; 1] = { // { dg-error ".E0080." "" { target *-*-* } }
// { dg-note "" "" { target *-*-* } .-2 }
// { dg-note "" "" { target *-*-* } .-3 }
    unsafe { &static_cross_crate::ZERO }
};

const U8_MUT: &u8 = { // { dg-error ".E0080." "" { target *-*-* } }
// { dg-note "" "" { target *-*-* } .-2 }
// { dg-note "" "" { target *-*-* } .-3 }
    unsafe { &static_cross_crate::ZERO[0] }
};

// Also test indirection that reads from other static. This causes a const_err.
#[warn(const_err)] // { dg-note "" "" { target *-*-* } }
const U8_MUT2: &u8 = { // { dg-note "" "" { target *-*-* } }
    unsafe { &(*static_cross_crate::ZERO_REF)[0] }
// { dg-warning "" "" { target *-*-* } .-1 }
// { dg-note "" "" { target *-*-* } .-2 }
};
#[warn(const_err)] // { dg-note "" "" { target *-*-* } }
const U8_MUT3: &u8 = { // { dg-note "" "" { target *-*-* } }
    unsafe { match static_cross_crate::OPT_ZERO { Some(ref u) => u, None => panic!() } }
// { dg-warning "" "" { target *-*-* } .-1 }
// { dg-note "" "" { target *-*-* } .-2 }
};

pub fn test(x: &[u8; 1]) -> bool {
    match x {
        SLICE_MUT => true,
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-error "" "" { target *-*-* } .-2 }
        &[1..] => false,
    }
}

pub fn test2(x: &u8) -> bool {
    match x {
        U8_MUT => true,
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-error "" "" { target *-*-* } .-2 }
        &(1..) => false,
    }
}

// We need to use these *in a pattern* to trigger the failure... likely because
// the errors above otherwise stop compilation too early?
pub fn test3(x: &u8) -> bool {
    match x {
        U8_MUT2 => true,
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-error "" "" { target *-*-* } .-2 }
        &(1..) => false,
    }
}
pub fn test4(x: &u8) -> bool {
    match x {
        U8_MUT3 => true,
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-error "" "" { target *-*-* } .-2 }
        &(1..) => false,
    }
}

fn main() {
    unsafe {
        static_cross_crate::ZERO[0] = 1;
    }
    // Now the pattern is not exhaustive any more!
    test(&[0]);
    test2(&0);
}

