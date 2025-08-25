// For each of these, we should get the appropriate type mismatch error message,
// and the function should be echoed.

// aux-build:test-macros.rs

#[macro_use]
extern crate test_macros;

#[recollect_attr]
fn a() {
    let x: usize = "hello"; // { dg-error ".E0308." "" { target *-*-* } }
}

#[recollect_attr]
fn b(x: Option<isize>) -> usize {
    match x {
        Some(x) => { return x }, // { dg-error ".E0308." "" { target *-*-* } }
        None => 10
    }
}

#[recollect_attr]
fn c() {
    struct Foo {
        a: usize
    }

    struct Bar {
        a: usize,
        b: usize
    }

    let x = Foo { a: 10isize }; // { dg-error ".E0308." "" { target *-*-* } }
    let y = Foo { a: 10, b: 10isize }; // { dg-error ".E0560." "" { target *-*-* } }
}

#[recollect_attr]
extern fn bar() {
    0 // { dg-error ".E0308." "" { target *-*-* } }
}

#[recollect_attr]
extern "C" fn baz() {
    0 // { dg-error ".E0308." "" { target *-*-* } }
}

#[recollect_attr]
extern "Rust" fn rust_abi() {
    0 // { dg-error ".E0308." "" { target *-*-* } }
}

#[recollect_attr]
extern "\x43" fn c_abi_escaped() {
    0 // { dg-error ".E0308." "" { target *-*-* } }
}

fn main() {}

