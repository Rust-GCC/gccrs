const fn x() {
    let t = true;
    let x = || t; // { dg-error ".E0658." "" { target *-*-* } }
}

fn main() {}

