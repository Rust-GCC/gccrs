// Regression test for Issue #53787: Fix ICE when creating a label in inline assembler with macros.

// build-fail
// ignore-emscripten

#![feature(llvm_asm)]

macro_rules! fake_jump {
    ($id:expr) => {
        unsafe {
            llvm_asm!(
            "
            jmp $0
            lea eax, [ebx]
            xor eax, 0xDEADBEEF
            retn
            $0:
            "::"0"($id)::"volatile", "intel");
        }
    };
}

fn main() {
    fake_jump!("FirstFunc"); // { dg-error ".E0669." "" { target *-*-* } }
    println!("Hello, world!");
}

