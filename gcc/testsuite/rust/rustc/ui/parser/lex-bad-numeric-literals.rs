fn main() {
    0o1.0; // { dg-error "" "" { target *-*-* } }
    0o2f32; // { dg-error "" "" { target *-*-* } }
    0o3.0f32; // { dg-error "" "" { target *-*-* } }
    0o4e4; // { dg-error "" "" { target *-*-* } }
    0o5.0e5; // { dg-error "" "" { target *-*-* } }
    0o6e6f32; // { dg-error "" "" { target *-*-* } }
    0o7.0e7f64; // { dg-error "" "" { target *-*-* } }
    0x8.0e+9; // { dg-error "" "" { target *-*-* } }
    0x9.0e-9; // { dg-error "" "" { target *-*-* } }
    0o; // { dg-error ".E0768." "" { target *-*-* } }
    1e+; // { dg-error "" "" { target *-*-* } }
    0x539.0; // { dg-error "" "" { target *-*-* } }
    9900000000000000000000000000999999999999999999999999999999;
// { dg-error "" "" { target *-*-* } .-1 }
    9900000000000000000000000000999999999999999999999999999999;
// { dg-error "" "" { target *-*-* } .-1 }
    0x; // { dg-error ".E0768." "" { target *-*-* } }
    0xu32; // { dg-error ".E0768." "" { target *-*-* } }
    0ou32; // { dg-error ".E0768." "" { target *-*-* } }
    0bu32; // { dg-error ".E0768." "" { target *-*-* } }
    0b; // { dg-error ".E0768." "" { target *-*-* } }
    0o123f64; // { dg-error "" "" { target *-*-* } }
    0o123.456; // { dg-error "" "" { target *-*-* } }
    0b101f64; // { dg-error "" "" { target *-*-* } }
    0b111.101; // { dg-error "" "" { target *-*-* } }
}

