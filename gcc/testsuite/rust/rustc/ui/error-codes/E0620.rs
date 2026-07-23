fn main() {
    let _foo = &[1_usize, 2] as [usize]; // { dg-error ".E0620." "" { target *-*-* } }
}

