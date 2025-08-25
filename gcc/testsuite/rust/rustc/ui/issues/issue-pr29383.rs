enum E {
    A,
    B,
}

fn main() {
    match None {
        None => {}
        Some(E::A(..)) => {}
// { dg-error ".E0532." "" { target *-*-* } .-1 }
        Some(E::B(..)) => {}
// { dg-error ".E0532." "" { target *-*-* } .-1 }
    }
}

