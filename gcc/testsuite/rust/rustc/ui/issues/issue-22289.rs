fn main() {
    0 as &dyn std::any::Any; // { dg-error ".E0605." "" { target *-*-* } }
}

