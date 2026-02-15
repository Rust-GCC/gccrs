struct S(u8, u8, u8);

fn main() {
    match (1, 2, 3) {
        (1, 2, 3, 4) => {} // { dg-error ".E0308." "" { target *-*-* } }
        (1, 2, .., 3, 4) => {} // { dg-error ".E0308." "" { target *-*-* } }
        _ => {}
    }
    match S(1, 2, 3) {
        S(1, 2, 3, 4) => {}
// { dg-error ".E0023." "" { target *-*-* } .-1 }
        S(1, 2, .., 3, 4) => {}
// { dg-error ".E0023." "" { target *-*-* } .-1 }
        _ => {}
    }
}

