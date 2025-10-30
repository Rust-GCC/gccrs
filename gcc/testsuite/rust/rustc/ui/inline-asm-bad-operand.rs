// Test that the compiler will catch passing invalid values to inline assembly
// operands.

// build-fail
// ignore-emscripten

#![feature(llvm_asm)]

#[repr(C)]
struct MyPtr(usize);

fn main() {
    issue_37433();
    issue_37437();
    issue_40187();
    issue_54067();
    multiple_errors();
}

fn issue_37433() {
    unsafe {
        llvm_asm!("" :: "r"("")); // { dg-error ".E0669." "" { target *-*-* } }
    }

    unsafe {
        let target = MyPtr(0);
        llvm_asm!("ret" : : "{rdi}"(target)); // { dg-error ".E0669." "" { target *-*-* } }
    }
}

fn issue_37437() {
    let hello: &str = "hello";
    // this should fail...
    unsafe { llvm_asm!("" :: "i"(hello)) }; // { dg-error ".E0669." "" { target *-*-* } }
    // but this should succeed.
    unsafe { llvm_asm!("" :: "r"(hello.as_ptr())) };
}

fn issue_40187() {
    let arr: [u8; 1] = [0; 1];
    unsafe {
        llvm_asm!("movups $1, %xmm0"::"m"(arr)); // { dg-error ".E0669." "" { target *-*-* } }
    }
}

fn issue_54067() {
    let addr: Option<u32> = Some(123);
    unsafe {
        llvm_asm!("mov sp, $0"::"r"(addr)); // { dg-error ".E0669." "" { target *-*-* } }
    }
}

fn multiple_errors() {
    let addr: (u32, u32) = (1, 2);
    unsafe {
        llvm_asm!("mov sp, $0"::"r"(addr), // { dg-error ".E0669." "" { target *-*-* } }
                                "r"("hello e0669")); // { dg-error ".E0669." "" { target *-*-* } }
    }
}

