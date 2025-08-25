// Test the "defined here" and "not covered" diagnostic hints.
// We also make sure that references are peeled off from the scrutinee type
// so that the diagnostics work better with default binding modes.

#[derive(Clone)]
enum E {
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-error "" "" { target *-*-* } .-2 }
// { dg-error "" "" { target *-*-* } .-3 }
// { dg-error "" "" { target *-*-* } .-4 }
// { dg-error "" "" { target *-*-* } .-5 }
// { dg-error "" "" { target *-*-* } .-6 }
    A,
    B,
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-error "" "" { target *-*-* } .-2 }
// { dg-error "" "" { target *-*-* } .-3 }
// { dg-error "" "" { target *-*-* } .-4 }
// { dg-error "" "" { target *-*-* } .-5 }
// { dg-error "" "" { target *-*-* } .-6 }
    C
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-error "" "" { target *-*-* } .-2 }
// { dg-error "" "" { target *-*-* } .-3 }
// { dg-error "" "" { target *-*-* } .-4 }
// { dg-error "" "" { target *-*-* } .-5 }
// { dg-error "" "" { target *-*-* } .-6 }
}

fn by_val(e: E) {
    let e1 = e.clone();
    match e1 { // { dg-error ".E0004." "" { target *-*-* } }
        E::A => {}
    }

    let E::A = e; // { dg-error ".E0005." "" { target *-*-* } }
}

fn by_ref_once(e: &E) {
    match e { // { dg-error ".E0004." "" { target *-*-* } }
        E::A => {}
    }

    let E::A = e; // { dg-error ".E0005." "" { target *-*-* } }
}

fn by_ref_thrice(e: & &mut &E) {
    match e { // { dg-error ".E0004." "" { target *-*-* } }
        E::A => {}
    }

    let E::A = e;
// { dg-error ".E0005." "" { target *-*-* } .-1 }
}

enum Opt {
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-error "" "" { target *-*-* } .-2 }
    Some(u8),
    None,
// { dg-error "" "" { target *-*-* } .-1 }
}

fn ref_pat(e: Opt) {
    match e {// { dg-error ".E0004." "" { target *-*-* } }
        Opt::Some(ref _x) => {}
    }

    let Opt::Some(ref _x) = e; // { dg-error ".E0005." "" { target *-*-* } }
}

fn main() {}

