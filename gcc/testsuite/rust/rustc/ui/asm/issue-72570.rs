// compile-flags: -Zsave-analysis
// only-x86_64
// Also test for #72960

#![feature(asm)]

fn main() {
    unsafe {
        asm!("", in("invalid") "".len());
// { dg-error "" "" { target *-*-* } .-1 }
    }
}

