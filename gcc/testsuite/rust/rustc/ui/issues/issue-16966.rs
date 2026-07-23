fn main() {
    panic!(std::default::Default::default());
// { dg-error ".E0282." "" { target *-*-* } .-1 }
}

