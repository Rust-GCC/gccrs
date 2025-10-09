// Syntactically, we do allow e.g., `static X: u8 = 0;` as an associated item.

fn main() {}

#[cfg(FALSE)]
impl S {
    static IA: u8 = 0; // { dg-error "" "" { target *-*-* } }
    static IB: u8; // { dg-error "" "" { target *-*-* } }
    default static IC: u8 = 0; // { dg-error "" "" { target *-*-* } }
// { dg-error "" "" { target *-*-* } .-1 }
    pub(crate) default static ID: u8; // { dg-error "" "" { target *-*-* } }
// { dg-error "" "" { target *-*-* } .-1 }
}

#[cfg(FALSE)]
trait T {
    static TA: u8 = 0; // { dg-error "" "" { target *-*-* } }
    static TB: u8; // { dg-error "" "" { target *-*-* } }
    default static TC: u8 = 0; // { dg-error "" "" { target *-*-* } }
// { dg-error "" "" { target *-*-* } .-1 }
    pub(crate) default static TD: u8; // { dg-error "" "" { target *-*-* } }
// { dg-error "" "" { target *-*-* } .-1 }
}

#[cfg(FALSE)]
impl T for S {
    static TA: u8 = 0; // { dg-error "" "" { target *-*-* } }
    static TB: u8; // { dg-error "" "" { target *-*-* } }
    default static TC: u8 = 0; // { dg-error "" "" { target *-*-* } }
// { dg-error "" "" { target *-*-* } .-1 }
    pub default static TD: u8; // { dg-error "" "" { target *-*-* } }
// { dg-error "" "" { target *-*-* } .-1 }
}

