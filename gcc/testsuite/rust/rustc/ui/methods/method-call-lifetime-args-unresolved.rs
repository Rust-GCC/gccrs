fn main() {
    0.clone::<'a>(); // { dg-error ".E0261." "" { target *-*-* } }
}

