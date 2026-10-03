fn main() {
    let x = 0;
    let _ = x.foo; // { dg-error ".E0610." "" { target *-*-* } }
}

