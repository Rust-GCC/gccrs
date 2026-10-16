#![feature(unwind_attributes, const_panic)]

#[unwind(aborts)]
const fn foo() {
    panic!() // { dg-error ".E0080." "" { target *-*-* } }
}

const _: () = foo(); // { dg-error "" "" { target *-*-* } }
// Ensure that the CTFE engine handles calls to `#[unwind(aborts)]` gracefully

fn main() {
    let _ = foo();
}

