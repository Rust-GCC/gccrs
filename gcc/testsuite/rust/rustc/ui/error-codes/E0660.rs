#![feature(llvm_asm)]

fn main() {
    let a;
    llvm_asm!("nop" "nop");
// { dg-error ".E0660." "" { target *-*-* } .-1 }
    llvm_asm!("nop" "nop" : "=r"(a));
// { dg-error ".E0660." "" { target *-*-* } .-1 }
}

