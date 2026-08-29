#![deny(dead_code)]

union U1 {
    a: u8, // should not be reported
    b: u8, // should not be reported
    c: u8, // { dg-error "" "" { target *-*-* } }
}
union U2 {
    a: u8, // { dg-error "" "" { target *-*-* } }
    b: u8, // should not be reported
    c: u8, // should not be reported
}
union NoDropLike { a: u8 } // { dg-error "" "" { target *-*-* } }

union U {
    a: u8, // should not be reported
    b: u8, // should not be reported
    c: u8, // { dg-error "" "" { target *-*-* } }
}
type A = U;

fn main() {
    let u = U1 { a: 0 };
    let _a = unsafe { u.b };

    let u = U2 { c: 0 };
    let _b = unsafe { u.b };

    let _u = NoDropLike { a: 10 };
    let u = A { a: 0 };
    let _b = unsafe { u.b };
}

