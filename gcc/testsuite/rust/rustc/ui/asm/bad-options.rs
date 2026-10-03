// only-x86_64

#![feature(asm)]

fn main() {
    let mut foo = 0;
    unsafe {
        asm!("", options(nomem, readonly));
// { dg-error "" "" { target *-*-* } .-1 }
        asm!("", options(pure, nomem, noreturn));
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-error "" "" { target *-*-* } .-2 }
        asm!("{}", in(reg) foo, options(pure, nomem));
// { dg-error "" "" { target *-*-* } .-1 }
        asm!("{}", out(reg) foo, options(noreturn));
// { dg-error "" "" { target *-*-* } .-1 }
    }
}

