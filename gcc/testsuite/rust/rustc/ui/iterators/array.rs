fn main() {
    for _ in [1, 2] {}
// { dg-error ".E0277." "" { target *-*-* } .-1 }
    let x = [1, 2];
    for _ in x {}
// { dg-error ".E0277." "" { target *-*-* } .-1 }
    for _ in [1.0, 2.0] {}
// { dg-error ".E0277." "" { target *-*-* } .-1 }
}

