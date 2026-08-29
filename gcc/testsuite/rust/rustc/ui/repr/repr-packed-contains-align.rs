#![feature(untagged_unions)]
#![allow(dead_code)]

#[repr(align(16))]
struct SA(i32);

struct SB(SA);

#[repr(align(16))]
union UA {
    i: i32
}

union UB {
    a: UA
}

#[repr(packed)]
struct SC(SA); // { dg-error ".E0588." "" { target *-*-* } }

#[repr(packed)]
struct SD(SB); // { dg-error ".E0588." "" { target *-*-* } }

#[repr(packed)]
struct SE(UA); // { dg-error ".E0588." "" { target *-*-* } }

#[repr(packed)]
struct SF(UB); // { dg-error ".E0588." "" { target *-*-* } }

#[repr(packed)]
union UC { // { dg-error ".E0588." "" { target *-*-* } }
    a: UA
}

#[repr(packed)]
union UD { // { dg-error ".E0588." "" { target *-*-* } }
    n: UB
}

#[repr(packed)]
union UE { // { dg-error ".E0588." "" { target *-*-* } }
    a: SA
}

#[repr(packed)]
union UF { // { dg-error ".E0588." "" { target *-*-* } }
    n: SB
}

fn main() {}

