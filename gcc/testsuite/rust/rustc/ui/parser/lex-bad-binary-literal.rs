fn main() {
    0b121; // { dg-error "" "" { target *-*-* } }
    0b10_10301; // { dg-error "" "" { target *-*-* } }
    0b30; // { dg-error "" "" { target *-*-* } }
    0b41; // { dg-error "" "" { target *-*-* } }
    0b5; // { dg-error "" "" { target *-*-* } }
    0b6; // { dg-error "" "" { target *-*-* } }
    0b7; // { dg-error "" "" { target *-*-* } }
    0b8; // { dg-error "" "" { target *-*-* } }
    0b9; // { dg-error "" "" { target *-*-* } }
}

