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

fn foo(x: isize) { println!("{}", x); }

#[cfg(any(target_arch = "x86",
          target_arch = "x86_64",
          target_arch = "arm",
          target_arch = "aarch64"))]
pub fn main() {
    let x: isize;
    x = 1;
    foo(x);
    unsafe {
        llvm_asm!("mov $1, $0" : "=r"(x) : "r"(5));
// { dg-error ".E0384." "" { target *-*-* } .-1 }
    }
    foo(x);
}

#[cfg(not(any(target_arch = "x86",
              target_arch = "x86_64",
              target_arch = "arm",
              target_arch = "aarch64")))]
pub fn main() {}

