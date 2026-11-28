#![feature(const_extern_fn)]

const unsafe extern fn foo() -> usize { 5 }

fn main() {
    let a: [u8; foo()];
// { dg-error ".E0133." "" { target *-*-* } .-1 }
    foo();
// { dg-error ".E0133." "" { target *-*-* } .-1 }
}

