fn main() {
    let n: u32 = 1;
    let mut d: u64 = 2;
    d = d % n.into(); // { dg-error ".E0284." "" { target *-*-* } }
}

