// Test various ways to construct a pointer with a longer lifetime
// than the thing it points at and ensure that they result in
// errors. See also regions-free-region-ordering-callee.rs

fn call1<'a>(x: &'a usize) {
    // Test that creating a pointer like
    // &'a &'z usize requires that 'a <= 'z:
    let y: usize = 3;
    let z: &'a & usize = &(&y);
// { dg-error ".E0597." "" { target *-*-* } .-1 }
// { dg-error ".E0597." "" { target *-*-* } .-2 }
}

fn main() {}

