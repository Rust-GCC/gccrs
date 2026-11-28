// Test that spans get only base in eager type resolution (structurally_resolve_type).

fn main() {
    let mut x = Default::default();
    x.0;
// { dg-error ".E0282." "" { target *-*-* } .-1 }
    x = 1;
}

fn foo() {
    let mut x = Default::default();
    x[0];
// { dg-error ".E0282." "" { target *-*-* } .-1 }
    x = 1;
}

