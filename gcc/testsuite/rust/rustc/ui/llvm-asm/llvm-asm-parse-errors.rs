#![feature(llvm_asm)]

fn main() {
    llvm_asm!(); // { dg-error "" "" { target *-*-* } }
    llvm_asm!("nop" : struct); // { dg-error "" "" { target *-*-* } }
    llvm_asm!("mov %eax, $$0x2" : struct); // { dg-error "" "" { target *-*-* } }
    llvm_asm!("mov %eax, $$0x2" : "={eax}" struct); // { dg-error "" "" { target *-*-* } }
    llvm_asm!("mov %eax, $$0x2" : "={eax}"(struct)); // { dg-error "" "" { target *-*-* } }
    llvm_asm!("in %dx, %al" : "={al}"(result) : struct); // { dg-error "" "" { target *-*-* } }
    llvm_asm!("in %dx, %al" : "={al}"(result) : "{dx}" struct); // { dg-error "" "" { target *-*-* } }
    llvm_asm!("in %dx, %al" : "={al}"(result) : "{dx}"(struct)); // { dg-error "" "" { target *-*-* } }
    llvm_asm!("mov $$0x200, %eax" : : : struct); // { dg-error "" "" { target *-*-* } }
    llvm_asm!("mov eax, 2" : "={eax}"(foo) : : : struct); // { dg-error "" "" { target *-*-* } }
    llvm_asm!(123); // { dg-error "" "" { target *-*-* } }
}

