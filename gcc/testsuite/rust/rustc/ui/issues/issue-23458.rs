#![feature(llvm_asm)]

// build-fail
// only-x86_64

fn main() {
    unsafe {
        llvm_asm!("int $3"); // { dg-error "" "" { target *-*-* } }
// { dg-error "" "" { target *-*-* } .-2 }
    }
}

