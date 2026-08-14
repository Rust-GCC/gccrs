struct S(u8);
const C: S = S(10);

fn main() {
    let C(a) = S(11); // { dg-error ".E0532." "" { target *-*-* } }
    let C(..) = S(11); // { dg-error ".E0532." "" { target *-*-* } }
}

