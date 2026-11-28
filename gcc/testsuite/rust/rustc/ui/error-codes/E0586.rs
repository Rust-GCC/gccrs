fn main() {
    let tmp = vec![0, 1, 2, 3, 4, 4, 3, 3, 2, 1];
    let x = &tmp[1..=]; // { dg-error ".E0586." "" { target *-*-* } }
}

