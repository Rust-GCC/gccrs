fn main() {
    let z = match 3 {
        x(1) => x(1) // { dg-error ".E0425." "" { target *-*-* } }
// { dg-error ".E0425." "" { target *-*-* } .-1 }
    };
    assert!(z == 3);
}

