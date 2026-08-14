#![feature(const_fn_fn_ptr_basics)]

const fn foo() { (||{})() }
// { dg-error ".E0015." "" { target *-*-* } .-1 }

const fn bad(input: fn()) {
    input()
// { dg-error "" "" { target *-*-* } .-1 }
}

fn main() {
}

