fn main() {
    let x = (1, 2, 3);
    match x {
        (_a, _x @ ..) => {}
        _ => {}
    }
}
// { dg-error ".E0308." "" { target *-*-* } .-4 }
// { dg-error "" "" { target *-*-* } .-2 }
// { dg-error ".E0308." "" { target *-*-* } .-3 }

