// build-fail
// ignore-emscripten no asm! support

#![feature(llvm_asm)]

fn main() {
    unsafe {
        llvm_asm!("nop" : "+r"("r15"));
// { dg-error ".E0668." "" { target *-*-* } .-1 }
    }
}

