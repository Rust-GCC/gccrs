// ignore-emscripten

#![feature(llvm_asm)]

fn main() {
    llvm_asm!("mov $$0x200, %eax"
              :
              :
              : "{eax}" // { dg-error ".E0664." "" { target *-*-* } }
             );
}

