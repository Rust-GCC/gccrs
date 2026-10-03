fn main() {
    // Unconstrained type:
    format!("{:?}", None);
// { dg-error ".E0282." "" { target *-*-* } .-1 }
}

