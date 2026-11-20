// compile-flags: -Zunleash-the-miri-inside-of-you
// only-x86_64
#![feature(asm,llvm_asm)]
#![allow(const_err)]

fn main() {}

// Make sure we catch executing inline assembly.
static TEST_BAD1: () = {
    unsafe { llvm_asm!("xor %eax, %eax" ::: "eax"); }
// { dg-error ".E0080." "" { target *-*-* } .-1 }
// { dg-note ".E0080." "" { target *-*-* } .-2 }
// { dg-note ".E0080." "" { target *-*-* } .-3 }
// { dg-note ".E0080." "" { target *-*-* } .-4 }
};

// Make sure we catch executing inline assembly.
static TEST_BAD2: () = {
    unsafe { asm!("nop"); }
// { dg-error ".E0080." "" { target *-*-* } .-1 }
// { dg-note ".E0080." "" { target *-*-* } .-2 }
};

