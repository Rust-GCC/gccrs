enum E {
    A(u8, u8),
}

fn main() {
    let e = E::A(2, 3);
    match e {
        E::A(x @ ..) => {
// { dg-error ".E0023." "" { target *-*-* } .-1 }
// { dg-error ".E0023." "" { target *-*-* } .-2 }
// { dg-error ".E0023." "" { target *-*-* } .-3 }
            x
        }
    };
}

