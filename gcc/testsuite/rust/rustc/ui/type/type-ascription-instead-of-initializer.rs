fn main() {
    let x: Vec::with_capacity(10, 20);  // { dg-error ".E0061." "" { target *-*-* } }
// { dg-error ".E0061." "" { target *-*-* } .-1 }
}

