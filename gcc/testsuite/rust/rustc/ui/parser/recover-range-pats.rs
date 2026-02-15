// Here we test all kinds of range patterns in terms of parsing / recovery.
// We want to ensure that:
// 1. Things parse as they should.
// 2. Or at least we have parser recovery if they don't.

#![feature(exclusive_range_pattern)]
#![feature(half_open_range_patterns)]
#![deny(ellipsis_inclusive_range_patterns)]

fn main() {}

const X: u8 = 0;
const Y: u8 = 3;

fn exclusive_from_to() {
    if let 0..3 = 0 {} // OK.
    if let 0..Y = 0 {} // OK.
    if let X..3 = 0 {} // OK.
    if let X..Y = 0 {} // OK.
    if let true..Y = 0 {} // { dg-error ".E0029." "" { target *-*-* } }
    if let X..true = 0 {} // { dg-error ".E0029." "" { target *-*-* } }
    if let .0..Y = 0 {} // { dg-error ".E0308." "" { target *-*-* } }
// { dg-error ".E0308." "" { target *-*-* } .-1 }
    if let X.. .0 = 0 {} // { dg-error ".E0308." "" { target *-*-* } }
// { dg-error ".E0308." "" { target *-*-* } .-1 }
}

fn inclusive_from_to() {
    if let 0..=3 = 0 {} // OK.
    if let 0..=Y = 0 {} // OK.
    if let X..=3 = 0 {} // OK.
    if let X..=Y = 0 {} // OK.
    if let true..=Y = 0 {} // { dg-error ".E0029." "" { target *-*-* } }
    if let X..=true = 0 {} // { dg-error ".E0029." "" { target *-*-* } }
    if let .0..=Y = 0 {} // { dg-error ".E0308." "" { target *-*-* } }
// { dg-error ".E0308." "" { target *-*-* } .-1 }
    if let X..=.0 = 0 {} // { dg-error ".E0308." "" { target *-*-* } }
// { dg-error ".E0308." "" { target *-*-* } .-1 }
}

fn inclusive2_from_to() {
    if let 0...3 = 0 {} // { dg-error "" "" { target *-*-* } }
    if let 0...Y = 0 {} // { dg-error "" "" { target *-*-* } }
    if let X...3 = 0 {} // { dg-error "" "" { target *-*-* } }
    if let X...Y = 0 {} // { dg-error "" "" { target *-*-* } }
    if let true...Y = 0 {} // { dg-error ".E0029." "" { target *-*-* } }
// { dg-error ".E0029." "" { target *-*-* } .-1 }
    if let X...true = 0 {} // { dg-error ".E0029." "" { target *-*-* } }
// { dg-error ".E0029." "" { target *-*-* } .-1 }
    if let .0...Y = 0 {} // { dg-error ".E0308." "" { target *-*-* } }
// { dg-error ".E0308." "" { target *-*-* } .-1 }
// { dg-error ".E0308." "" { target *-*-* } .-2 }
    if let X... .0 = 0 {} // { dg-error ".E0308." "" { target *-*-* } }
// { dg-error ".E0308." "" { target *-*-* } .-1 }
// { dg-error ".E0308." "" { target *-*-* } .-2 }
}

fn exclusive_from() {
    if let 0.. = 0 {}
    if let X.. = 0 {}
    if let true.. = 0 {}
// { dg-error ".E0029." "" { target *-*-* } .-1 }
    if let .0.. = 0 {}
// { dg-error ".E0308." "" { target *-*-* } .-1 }
// { dg-error ".E0308." "" { target *-*-* } .-2 }
}

fn inclusive_from() {
    if let 0..= = 0 {} // { dg-error ".E0586." "" { target *-*-* } }
    if let X..= = 0 {} // { dg-error ".E0586." "" { target *-*-* } }
    if let true..= = 0 {} // { dg-error ".E0029." "" { target *-*-* } }
// { dg-error ".E0586." "" { target *-*-* } .-2 }
    if let .0..= = 0 {} // { dg-error ".E0308." "" { target *-*-* } }
// { dg-error ".E0308." "" { target *-*-* } .-1 }
// { dg-error ".E0308." "" { target *-*-* } .-2 }
}

fn inclusive2_from() {
    if let 0... = 0 {} // { dg-error ".E0586." "" { target *-*-* } }
    if let X... = 0 {} // { dg-error ".E0586." "" { target *-*-* } }
    if let true... = 0 {} // { dg-error ".E0029." "" { target *-*-* } }
// { dg-error ".E0586." "" { target *-*-* } .-2 }
    if let .0... = 0 {} // { dg-error ".E0308." "" { target *-*-* } }
// { dg-error ".E0308." "" { target *-*-* } .-1 }
// { dg-error ".E0308." "" { target *-*-* } .-2 }
}

fn exclusive_to() {
    if let ..0 = 0 {}
    if let ..Y = 0 {}
    if let ..true = 0 {}
// { dg-error ".E0029." "" { target *-*-* } .-1 }
    if let .. .0 = 0 {}
// { dg-error ".E0308." "" { target *-*-* } .-1 }
// { dg-error ".E0308." "" { target *-*-* } .-2 }
}

fn inclusive_to() {
    if let ..=3 = 0 {}
    if let ..=Y = 0 {}
    if let ..=true = 0 {}
// { dg-error ".E0029." "" { target *-*-* } .-1 }
    if let ..=.0 = 0 {}
// { dg-error ".E0308." "" { target *-*-* } .-1 }
// { dg-error ".E0308." "" { target *-*-* } .-2 }
}

fn inclusive2_to() {
    if let ...3 = 0 {}
// { dg-error "" "" { target *-*-* } .-1 }
    if let ...Y = 0 {}
// { dg-error "" "" { target *-*-* } .-1 }
    if let ...true = 0 {}
// { dg-error ".E0029." "" { target *-*-* } .-1 }
// { dg-error ".E0029." "" { target *-*-* } .-2 }
    if let ....3 = 0 {}
// { dg-error ".E0308." "" { target *-*-* } .-1 }
// { dg-error ".E0308." "" { target *-*-* } .-2 }
// { dg-error ".E0308." "" { target *-*-* } .-3 }
}

fn with_macro_expr_var() {
    macro_rules! mac2 {
        ($e1:expr, $e2:expr) => {
            let $e1..$e2;
            let $e1...$e2;
// { dg-error "" "" { target *-*-* } .-1 }
            let $e1..=$e2;
        }
    }

    mac2!(0, 1);

    macro_rules! mac {
        ($e:expr) => {
            let ..$e;
            let ...$e;
// { dg-error "" "" { target *-*-* } .-1 }
            let ..=$e;
            let $e..;
            let $e...; // { dg-error ".E0586." "" { target *-*-* } }
            let $e..=; // { dg-error ".E0586." "" { target *-*-* } }
        }
    }

    mac!(0);
}

