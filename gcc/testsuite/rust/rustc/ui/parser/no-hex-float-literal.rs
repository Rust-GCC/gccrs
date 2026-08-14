fn main() {
    0xABC.Df;
// { dg-error ".E0610." "" { target *-*-* } .-1 }
    0x567.89;
// { dg-error "" "" { target *-*-* } .-1 }
    0xDEAD.BEEFp-2f;
// { dg-error ".E0610." "" { target *-*-* } .-1 }
// { dg-error ".E0610." "" { target *-*-* } .-2 }
}

