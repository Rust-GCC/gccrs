fn main() {
    let x = Some(1);

    match x { } // { dg-error ".E0004." "" { target *-*-* } }
}

