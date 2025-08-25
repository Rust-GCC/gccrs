fn main() {
    let v = &[]; // { dg-error ".E0282." "" { target *-*-* } }
    let it = v.iter();
}

