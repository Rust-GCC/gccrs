fn main() {
    for &1 in [1].iter() {} // { dg-error ".E0005." "" { target *-*-* } }
}

