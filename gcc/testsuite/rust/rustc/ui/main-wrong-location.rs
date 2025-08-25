mod m {
// { dg-error ".E0601." "" { target *-*-* } .-1 }
    // An inferred main entry point (that doesn't use #[main])
    // must appear at the top of the crate
    fn main() { }
}

