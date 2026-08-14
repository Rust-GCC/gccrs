// only-x86_64

fn main() {
    unsafe {
        println!("{:?}", asm!(""));
// { dg-error ".E0658." "" { target *-*-* } .-1 }
        println!("{:?}", llvm_asm!(""));
// { dg-error ".E0658." "" { target *-*-* } .-1 }
    }
}

