fn main() {
    for _ in ..10 {}
// { dg-error ".E0277." "" { target *-*-* } .-1 }
    for _ in ..=10 {}
// { dg-error ".E0277." "" { target *-*-* } .-1 }
    for _ in 0..10 {}
    for _ in 0..=10 {}
    for _ in 0.. {}
}

