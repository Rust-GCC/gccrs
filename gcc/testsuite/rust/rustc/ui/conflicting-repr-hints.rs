#![allow(dead_code)]

#[repr(C)]
enum A {
    A,
}

#[repr(u64)]
enum B {
    B,
}

#[repr(C, u64)] // { dg-error ".E0566." "" { target *-*-* } }
// { dg-warning ".E0566." "" { target *-*-* } .-1 }
enum C {
    C,
}

#[repr(u32, u64)] // { dg-error ".E0566." "" { target *-*-* } }
// { dg-warning ".E0566." "" { target *-*-* } .-1 }
enum D {
    D,
}

#[repr(C, packed)]
struct E(i32);

#[repr(packed, align(8))]
struct F(i32); // { dg-error ".E0587." "" { target *-*-* } }

#[repr(packed)]
#[repr(align(8))]
struct G(i32); // { dg-error ".E0587." "" { target *-*-* } }

#[repr(align(8))]
#[repr(packed)]
struct H(i32); // { dg-error ".E0587." "" { target *-*-* } }

#[repr(packed, packed(2))]
struct I(i32); // { dg-error ".E0634." "" { target *-*-* } }

#[repr(packed(2))]
#[repr(packed)]
struct J(i32); // { dg-error ".E0634." "" { target *-*-* } }

#[repr(packed, packed(1))]
struct K(i32);

#[repr(packed, align(8))]
union X {
// { dg-error ".E0587." "" { target *-*-* } .-1 }
    i: i32,
}

#[repr(packed)]
#[repr(align(8))]
union Y {
// { dg-error ".E0587." "" { target *-*-* } .-1 }
    i: i32,
}

#[repr(align(8))]
#[repr(packed)]
union Z {
// { dg-error ".E0587." "" { target *-*-* } .-1 }
    i: i32,
}

fn main() {}

