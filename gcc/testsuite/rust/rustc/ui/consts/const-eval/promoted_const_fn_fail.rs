#![feature(const_fn, const_fn_union)]

#![allow(const_err)]

#[repr(C)]
union Bar {
    a: &'static u8,
    b: usize,
}

const fn bar() -> u8 {
    unsafe {
        // this will error as long as this test
        // is run on a system whose pointers need more
        // than 8 bits
        Bar { a: &42 }.b as u8
    }
}

fn main() {
    let x: &'static u8 = &(bar() + 1); // { dg-error ".E0716." "" { target *-*-* } }
    let y = *x;
    unreachable!();
}

