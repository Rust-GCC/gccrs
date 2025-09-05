fn main() {
    0b101010f64;
// { dg-error "" "" { target *-*-* } .-1 }
    0b101.010;
// { dg-error "" "" { target *-*-* } .-1 }
    0b101p4f64;
// { dg-error "" "" { target *-*-* } .-1 }
}

