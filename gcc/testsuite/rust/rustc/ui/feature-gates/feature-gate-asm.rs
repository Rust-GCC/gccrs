// only-x86_64

fn main() {
    unsafe {
        asm!("");
// { dg-error ".E0658." "" { target *-*-* } .-1 }
        llvm_asm!("");
// { dg-error ".E0658." "" { target *-*-* } .-1 }
    }
}

