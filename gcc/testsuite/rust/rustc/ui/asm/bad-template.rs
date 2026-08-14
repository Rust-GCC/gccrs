// only-x86_64

#![feature(asm)]

fn main() {
    let mut foo = 0;
    unsafe {
        asm!("{}");
// { dg-error "" "" { target *-*-* } .-1 }
        asm!("{1}", in(reg) foo);
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-error "" "" { target *-*-* } .-2 }
        asm!("{a}");
// { dg-error "" "" { target *-*-* } .-1 }
        asm!("{}", a = in(reg) foo);
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-error "" "" { target *-*-* } .-2 }
        asm!("{1}", a = in(reg) foo);
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-error "" "" { target *-*-* } .-2 }
        asm!("{}", in("eax") foo);
// { dg-error "" "" { target *-*-* } .-1 }
        asm!("{:foo}", in(reg) foo);
// { dg-error "" "" { target *-*-* } .-1 }
        asm!("", in(reg) 0, in(reg) 1);
// { dg-error "" "" { target *-*-* } .-1 }
    }
}

