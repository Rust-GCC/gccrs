// run-rustfix
// only-x86_64

#![feature(asm, llvm_asm)]

fn main() {
    unsafe {
        let x = 1;
        let y: i32;
        asm!("" :: "r" (x));
// { dg-error "" "" { target *-*-* } .-1 }
        asm!("" : "=r" (y));
// { dg-error "" "" { target *-*-* } .-1 }
        let _ = y;
    }
}

