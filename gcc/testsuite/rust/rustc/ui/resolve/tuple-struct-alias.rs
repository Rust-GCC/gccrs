struct S(u8, u16);
type A = S;

fn main() {
    let s = A(0, 1); // { dg-error ".E0423." "" { target *-*-* } }
    match s {
        A(..) => {} // { dg-error ".E0532." "" { target *-*-* } }
    }
}

