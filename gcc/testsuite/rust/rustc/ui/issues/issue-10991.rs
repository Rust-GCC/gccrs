fn main() {
    let nil = ();
    let _t = nil as usize; // { dg-error ".E0605." "" { target *-*-* } }
}

