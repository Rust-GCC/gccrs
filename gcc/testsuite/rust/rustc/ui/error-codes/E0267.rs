fn main() {
    let w = || { break; }; // { dg-error ".E0267." "" { target *-*-* } }
}

