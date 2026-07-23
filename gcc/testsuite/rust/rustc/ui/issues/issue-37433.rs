// build-fail
// ignore-emscripten no llvm_asm! support

#![feature(llvm_asm)]

fn main() {
    unsafe {
        llvm_asm!("" :: "r"(""));
// { dg-error ".E0669." "" { target *-*-* } .-1 }
    }
}

