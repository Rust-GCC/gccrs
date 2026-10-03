struct S(u8, u16);

fn main() {
    let s = S{0b1: 10, 0: 11};
// { dg-error ".E0560." "" { target *-*-* } .-1 }
    match s {
        S{0: a, 0x1: b, ..} => {}
// { dg-error ".E0026." "" { target *-*-* } .-1 }
    }
}

