// Semantically, we do not allow e.g., `static X: u8 = 0;` as an associated item.

#![feature(specialization)]
// { dg-warning "" "" { target *-*-* } .-1 }

fn main() {}

struct S;
impl S {
    static IA: u8 = 0;
// { dg-error "" "" { target *-*-* } .-1 }
    static IB: u8;
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-error "" "" { target *-*-* } .-2 }
    default static IC: u8 = 0;
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-error "" "" { target *-*-* } .-2 }
    pub(crate) default static ID: u8;
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-error "" "" { target *-*-* } .-2 }
// { dg-error "" "" { target *-*-* } .-3 }
}

trait T {
    static TA: u8 = 0;
// { dg-error "" "" { target *-*-* } .-1 }
    static TB: u8;
// { dg-error "" "" { target *-*-* } .-1 }
    default static TC: u8 = 0;
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-error "" "" { target *-*-* } .-2 }
    pub(crate) default static TD: u8;
// { dg-error ".E0449." "" { target *-*-* } .-1 }
// { dg-error ".E0449." "" { target *-*-* } .-2 }
// { dg-error ".E0449." "" { target *-*-* } .-3 }
}

impl T for S {
    static TA: u8 = 0;
// { dg-error "" "" { target *-*-* } .-1 }
    static TB: u8;
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-error "" "" { target *-*-* } .-2 }
    default static TC: u8 = 0;
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-error "" "" { target *-*-* } .-2 }
    pub default static TD: u8;
// { dg-error ".E0449." "" { target *-*-* } .-1 }
// { dg-error ".E0449." "" { target *-*-* } .-2 }
// { dg-error ".E0449." "" { target *-*-* } .-3 }
// { dg-error ".E0449." "" { target *-*-* } .-4 }
}

