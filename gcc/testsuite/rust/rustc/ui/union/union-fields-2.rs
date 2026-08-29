union U {
    a: u8,
    b: u16,
}

fn main() {
    let u = U {}; // { dg-error "" "" { target *-*-* } }
    let u = U { a: 0 }; // OK
    let u = U { a: 0, b: 1 }; // { dg-error "" "" { target *-*-* } }
    let u = U { a: 0, b: 1, c: 2 }; // { dg-error ".E0560." "" { target *-*-* } }
// { dg-error ".E0560." "" { target *-*-* } .-1 }
    let u = U { ..u }; // { dg-error ".E0436." "" { target *-*-* } }
// { dg-error ".E0436." "" { target *-*-* } .-1 }

    let U {} = u; // { dg-error "" "" { target *-*-* } }
    let U { a } = u; // OK
    let U { a, b } = u; // { dg-error "" "" { target *-*-* } }
    let U { a, b, c } = u; // { dg-error ".E0026." "" { target *-*-* } }
// { dg-error ".E0026." "" { target *-*-* } .-1 }
    let U { .. } = u; // { dg-error "" "" { target *-*-* } }
// { dg-error "" "" { target *-*-* } .-1 }
    let U { a, .. } = u; // { dg-error "" "" { target *-*-* } }
}

