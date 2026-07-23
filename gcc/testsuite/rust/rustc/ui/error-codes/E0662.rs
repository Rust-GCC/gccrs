// ignore-emscripten

#![feature(llvm_asm)]

fn main() {
    llvm_asm!("xor %eax, %eax"
              :
              : "=test"("a") // { dg-error ".E0662." "" { target *-*-* } }
             );
}

