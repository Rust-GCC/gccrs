#![deny(unreachable_patterns)]

#[non_exhaustive]
pub enum NonExhaustiveEnum {
    Unit,
// { dg-error "" "" { target *-*-* } .-1 }
    Tuple(u32),
// { dg-error "" "" { target *-*-* } .-1 }
    Struct { field: u32 }
// { dg-error "" "" { target *-*-* } .-1 }
}

pub enum NormalEnum {
    Unit,
// { dg-error "" "" { target *-*-* } .-1 }
    Tuple(u32),
// { dg-error "" "" { target *-*-* } .-1 }
    Struct { field: u32 }
// { dg-error "" "" { target *-*-* } .-1 }
}

#[non_exhaustive]
pub enum EmptyNonExhaustiveEnum {}

fn empty_non_exhaustive(x: EmptyNonExhaustiveEnum) {
    match x {}
    match x {
        _ => {} // not detected as unreachable
    }
}

fn main() {
    match NonExhaustiveEnum::Unit {}
// { dg-error ".E0004." "" { target *-*-* } .-1 }
    match NormalEnum::Unit {}
// { dg-error ".E0004." "" { target *-*-* } .-1 }
}

