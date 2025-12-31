fn main() {
    let y = 0u32;
    *y; // { dg-error ".E0614." "" { target *-*-* } }
}

