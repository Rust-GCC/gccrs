fn main() {
    let x: bool;
    while x { } // { dg-error ".E0381." "" { target *-*-* } }
}

