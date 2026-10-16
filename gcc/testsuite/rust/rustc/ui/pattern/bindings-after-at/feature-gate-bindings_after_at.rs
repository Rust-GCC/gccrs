fn main() {
    let x @ y = 0; // { dg-error ".E0658." "" { target *-*-* } }
}

