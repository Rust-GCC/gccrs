extern {
    #[linkage = "extern_weak"] static foo: isize;
// { dg-error ".E0658." "" { target *-*-* } .-1 }
}

fn main() {}

