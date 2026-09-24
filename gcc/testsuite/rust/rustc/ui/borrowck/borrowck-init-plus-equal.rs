fn test() {
    let mut v: isize;
    v = v + 1; // { dg-error ".E0381." "" { target *-*-* } }
    v.clone();
}

fn main() {
}

