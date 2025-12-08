fn main() {
    let x = Some(0);

    match x {
        Some(y) | None => {} // { dg-error ".E0408." "" { target *-*-* } }
        _ => ()
    }
}

