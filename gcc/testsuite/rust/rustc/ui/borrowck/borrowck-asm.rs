// ignore-s390x
// ignore-emscripten
// ignore-powerpc
// ignore-powerpc64
// ignore-powerpc64le
// ignore-riscv64
// ignore-sparc
// ignore-sparc64

#![feature(llvm_asm)]

#[cfg(any(target_arch = "x86",
            target_arch = "x86_64",
            target_arch = "arm",
            target_arch = "aarch64",
            target_arch = "mips",
            target_arch = "mips64"))]
mod test_cases {
    fn is_move() {
        let y: &mut isize;
        let x = &mut 0isize;
        unsafe {
            llvm_asm!("nop" : : "r"(x));
        }
        let z = x;  // { dg-error ".E0382." "" { target *-*-* } }
    }

    fn in_is_read() {
        let mut x = 3;
        let y = &mut x;
        unsafe {
            llvm_asm!("nop" : : "r"(x)); // { dg-error ".E0503." "" { target *-*-* } }
        }
        let z = y;
    }

    fn out_is_assign() {
        let x = 3;
        unsafe {
            llvm_asm!("nop" : "=r"(x));  // { dg-error ".E0384." "" { target *-*-* } }
        }
        let mut a = &mut 3;
        let b = &*a;
        unsafe {
            llvm_asm!("nop" : "=r"(a));  // OK, Shallow write to `a`
        }
        let c = b;
        let d = *a;
    }

    fn rw_is_assign() {
        let x = 3;
        unsafe {
            llvm_asm!("nop" : "+r"(x));  // { dg-error ".E0384." "" { target *-*-* } }
        }
    }

    fn indirect_is_not_init() {
        let x: i32;
        unsafe {
            llvm_asm!("nop" : "=*r"(x)); // { dg-error ".E0381." "" { target *-*-* } }
        }
    }

    fn rw_is_read() {
        let mut x = &mut 3;
        let y = &*x;
        unsafe {
            llvm_asm!("nop" : "+r"(x));  // { dg-error ".E0506." "" { target *-*-* } }
        }
        let z = y;
    }

    fn two_moves() {
        let x = &mut 2;
        unsafe {
            llvm_asm!("nop" : : "r"(x), "r"(x) );    // { dg-error ".E0382." "" { target *-*-* } }
        }
    }
}

fn main() {}

