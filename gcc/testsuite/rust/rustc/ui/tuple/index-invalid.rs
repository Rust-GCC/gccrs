fn main() {
    let _ = (((),),).1.0; // { dg-error ".E0609." "" { target *-*-* } }

    let _ = (((),),).0.1; // { dg-error ".E0609." "" { target *-*-* } }

    let _ = (((),),).000.000; // { dg-error ".E0609." "" { target *-*-* } }
}

