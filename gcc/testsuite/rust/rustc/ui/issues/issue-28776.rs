use std::ptr;

fn main() {
    (&ptr::write)(1 as *mut _, 42);
// { dg-error ".E0133." "" { target *-*-* } .-1 }
}

