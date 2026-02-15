fn main() {
    auto n = 0;// { dg-error "" "" { target *-*-* } }
// { help "" "" { target *-*-* } .-1 }
    auto m;// { dg-error "" "" { target *-*-* } }
// { help "" "" { target *-*-* } .-1 }
    m = 0;

    var n = 0;// { dg-error "" "" { target *-*-* } }
// { help "" "" { target *-*-* } .-1 }
    var m;// { dg-error "" "" { target *-*-* } }
// { help "" "" { target *-*-* } .-1 }
    m = 0;

    mut n = 0;// { dg-error "" "" { target *-*-* } }
// { help "" "" { target *-*-* } .-1 }
    mut var;// { dg-error "" "" { target *-*-* } }
// { help "" "" { target *-*-* } .-1 }
    var = 0;

    let _recovery_witness: () = 0; // { dg-error ".E0308." "" { target *-*-* } }
}

