#![feature(const_extern_fn)]

extern "C" {
    fn regular_in_block();
}

const extern fn bar() {
    unsafe {
        regular_in_block();
// { dg-error ".E0015." "" { target *-*-* } .-1 }
    }
}

extern fn regular() {}

const extern fn foo() {
    unsafe {
        regular();
// { dg-error ".E0015." "" { target *-*-* } .-1 }
    }
}

fn main() {}

