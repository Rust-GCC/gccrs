// Test that lifetimes must be declared for use on enums.
// See also regions-undeclared.rs

enum Yes0<'lt> {
    X3(&'lt usize)
}

enum Yes1<'a> {
    X4(&'a usize)
}

enum No0 {
    X5(&'foo usize) // { dg-error ".E0261." "" { target *-*-* } }
}

enum No1 {
    X6(&'a usize) // { dg-error ".E0261." "" { target *-*-* } }
}

fn main() {}

