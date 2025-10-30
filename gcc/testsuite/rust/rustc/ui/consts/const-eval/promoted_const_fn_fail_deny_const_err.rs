#![feature(const_fn, const_fn_union)]

#![deny(const_err)]

#[repr(C)]
union Bar {
    a: &'static u8,
    b: usize,
}

const fn bar() -> u8 {
    unsafe {
        // This will error as long as this test is run on a system whose
        // pointers need more than 8 bits.
        Bar { a: &42 }.b as u8
    }
}

fn main() {
    // This will compile, but then hard-abort at runtime.
    // FIXME(oli-obk): this should instead panic (not hard-abort) at runtime.
    let x: &'static u8 = &(bar() + 1);
// { dg-error ".E0716." "" { target *-*-* } .-1 }
    let y = *x;
    unreachable!();
}

