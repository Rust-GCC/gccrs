// ignore-android
// ignore-arm
// ignore-aarch64
// ignore-s390x
// ignore-emscripten
// ignore-powerpc
// ignore-powerpc64
// ignore-powerpc64le
// ignore-riscv64
// ignore-sparc
// ignore-sparc64
// ignore-mips
// ignore-mips64

#![feature(llvm_asm)]

#[cfg(any(target_arch = "x86",
          target_arch = "x86_64"))]

pub fn main() {
    unsafe {
        // clobber formatted as register input/output
        llvm_asm!("xor %eax, %eax" : : : "{eax}");
// { dg-error ".E0664." "" { target *-*-* } .-1 }
    }
}

