// only-x86_64

#![feature(asm)]

fn main() {
    unsafe {
        // Can't output to borrowed values.

        let mut a = 0isize;
        let p = &a;
        asm!("{}", out(reg) a);
// { dg-error ".E0506." "" { target *-*-* } .-1 }
        println!("{}", p);

        // Can't read from mutable borrowed values.

        let mut a = 0isize;
        let p = &mut a;
        asm!("{}", in(reg) a);
// { dg-error ".E0503." "" { target *-*-* } .-1 }
        println!("{}", p);
    }
}

