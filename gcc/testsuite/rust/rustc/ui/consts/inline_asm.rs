#![feature(llvm_asm)]

const _: () = unsafe { llvm_asm!("nop") };
// { dg-error ".E0015." "" { target *-*-* } .-1 }

fn main() {}

