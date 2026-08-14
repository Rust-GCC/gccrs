// ignore-emscripten

#![feature(llvm_asm)]

fn main() {
    let a; // { dg-error ".E0282." "" { target *-*-* } }
    llvm_asm!("nop" : "r"(a));
// { dg-error ".E0661." "" { target *-*-* } .-1 }
}

