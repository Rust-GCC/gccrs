fn main() {
    let x = Some(1);
    let Some(y) = x; // { dg-error ".E0005." "" { target *-*-* } }
}

