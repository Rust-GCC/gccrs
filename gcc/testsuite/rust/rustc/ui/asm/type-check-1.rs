// only-x86_64

#![feature(asm)]

fn main() {
    unsafe {
        // Outputs must be place expressions

        asm!("{}", in(reg) 1 + 2);
        asm!("{}", out(reg) 1 + 2);
// { dg-error "" "" { target *-*-* } .-1 }
        asm!("{}", inout(reg) 1 + 2);
// { dg-error "" "" { target *-*-* } .-1 }

        // Operands must be sized

        let v: [u64; 3] = [0, 1, 2];
        asm!("{}", in(reg) v[..]);
// { dg-error ".E0277." "" { target *-*-* } .-1 }
        asm!("{}", out(reg) v[..]);
// { dg-error ".E0277." "" { target *-*-* } .-1 }
        asm!("{}", inout(reg) v[..]);
// { dg-error ".E0277." "" { target *-*-* } .-1 }
    }
}

