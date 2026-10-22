fn main() {
    let (a, b) = (0, 1);
    (a, b) = (2, 3); // { dg-error ".E0658." "" { target *-*-* } }
}

