// run-pass

#![allow(dead_code)]
//
// See also: compile-fail/unsafe-fn-called-from-safe.rs

// pretty-expanded FIXME #23616

unsafe fn f() { return; }

unsafe fn g() {
    f();
}

pub fn main() {
    return;
}

