fn main() {
    1.0f64 - 1.0;
    1.0f64 - 1 // { dg-error ".E0277." "" { target *-*-* } }
}

