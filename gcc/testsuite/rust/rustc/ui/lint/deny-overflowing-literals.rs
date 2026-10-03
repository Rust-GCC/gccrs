fn main() {
    let x: u8 = 256;
// { dg-error "" "" { target *-*-* } .-1 }

    for _ in 0..256u8 {}
// { dg-error "" "" { target *-*-* } .-1 }
}

