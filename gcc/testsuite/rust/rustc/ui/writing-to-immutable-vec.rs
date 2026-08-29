fn main() {
    let v: Vec<isize> = vec![1, 2, 3];
    v[1] = 4; // { dg-error ".E0596." "" { target *-*-* } }
}

