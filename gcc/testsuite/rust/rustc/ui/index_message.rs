fn main() {
    let z = ();
    let _ = z[0]; // { dg-error ".E0608." "" { target *-*-* } }
}

