// Moving from a reference/raw pointer should be an error, even when they're
// the field of a union.

#![feature(untagged_unions)]

union Pointers {
    a: &'static String,
    b: &'static mut String,
    c: *const String,
    d: *mut String,
}

unsafe fn move_ref(u: Pointers) -> String {
    *u.a
// { dg-error ".E0507." "" { target *-*-* } .-1 }
}
unsafe fn move_ref_mut(u: Pointers) -> String {
    *u.b
// { dg-error ".E0507." "" { target *-*-* } .-1 }
}
unsafe fn move_ptr(u: Pointers) -> String {
    *u.c
// { dg-error ".E0507." "" { target *-*-* } .-1 }
}
unsafe fn move_ptr_mut(u: Pointers) -> String {
    *u.d
// { dg-error ".E0507." "" { target *-*-* } .-1 }
}

fn main() {}

