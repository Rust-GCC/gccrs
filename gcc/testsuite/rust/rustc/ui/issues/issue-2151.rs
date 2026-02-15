fn main() {
    let x = panic!();
    x.clone(); // { dg-error ".E0282." "" { target *-*-* } }
}

