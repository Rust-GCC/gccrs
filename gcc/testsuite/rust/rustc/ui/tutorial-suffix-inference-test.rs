fn main() {
    let x = 3;
    let y: i32 = 3;

    fn identity_u8(n: u8) -> u8 { n }
    fn identity_u16(n: u16) -> u16 { n }

    identity_u8(x);  // after this, `x` is assumed to have type `u8`
    identity_u16(x);
// { dg-error ".E0308." "" { target *-*-* } .-1 }
// { dg-error ".E0308." "" { target *-*-* } .-2 }
    identity_u16(y);
// { dg-error ".E0308." "" { target *-*-* } .-1 }
// { dg-error ".E0308." "" { target *-*-* } .-2 }

    let a = 3;

    fn identity_i(n: isize) -> isize { n }

    identity_i(a); // ok
    identity_u16(a);
// { dg-error ".E0308." "" { target *-*-* } .-1 }
// { dg-error ".E0308." "" { target *-*-* } .-2 }
}

