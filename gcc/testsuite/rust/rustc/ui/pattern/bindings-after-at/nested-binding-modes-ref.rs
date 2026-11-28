#![feature(bindings_after_at)]

fn main() {
    let ref is_ref @ is_val = 42;
    *is_ref;
    *is_val;
// { dg-error ".E0614." "" { target *-*-* } .-1 }

    let is_val @ ref is_ref = 42;
    *is_ref;
    *is_val;
// { dg-error ".E0614." "" { target *-*-* } .-1 }
}

