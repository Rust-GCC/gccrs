fn main() {
    0.clone::<T = u8>(); // { dg-error ".E0229." "" { target *-*-* } }
}

