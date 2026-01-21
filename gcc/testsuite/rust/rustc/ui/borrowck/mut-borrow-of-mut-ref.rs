// Suggest not mutably borrowing a mutable reference
#![crate_type = "rlib"]

pub fn f(b: &mut i32) {
    g(&mut b);
// { dg-error ".E0596." "" { target *-*-* } .-1 }
    g(&mut &mut b);
// { dg-error ".E0596." "" { target *-*-* } .-1 }
}

pub fn g(_: &mut i32) {}

