// Checks that we report ABI mismatches for "const extern fn"
// compile-flags: -Z unleash-the-miri-inside-of-you

#![feature(const_extern_fn)]
#![allow(const_err)]

const extern "C" fn c_fn() {}

const fn call_rust_fn(my_fn: extern "Rust" fn()) {
    my_fn();
// { dg-error ".E0080." "" { target *-*-* } .-1 }
// { dg-note ".E0080." "" { target *-*-* } .-2 }
// { dg-note ".E0080." "" { target *-*-* } .-3 }
}

static VAL: () = call_rust_fn(unsafe { std::mem::transmute(c_fn as extern "C" fn()) });
// { dg-note "" "" { target *-*-* } .-1 }

fn main() {}

