fn main() {
    let v = vec![0];
    const l: usize = v.count(); // { dg-error ".E0435." "" { target *-*-* } }
    let s: [u32; l] = v.into_iter().collect();
}

