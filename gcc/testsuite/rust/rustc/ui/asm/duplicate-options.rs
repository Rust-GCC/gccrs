// only-x86_64
// run-rustfix

#![feature(asm)]

fn main() {
    unsafe {
        asm!("", options(nomem, nomem));
// { dg-error "" "" { target *-*-* } .-1 }
        asm!("", options(att_syntax, att_syntax));
// { dg-error "" "" { target *-*-* } .-1 }
        asm!("", options(nostack, att_syntax), options(nostack));
// { dg-error "" "" { target *-*-* } .-1 }
        asm!("", options(nostack, nostack), options(nostack), options(nostack));
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-error "" "" { target *-*-* } .-2 }
// { dg-error "" "" { target *-*-* } .-3 }
        asm!(
            "",
            options(nomem, noreturn),
            options(att_syntax, noreturn), // { dg-error "" "" { target *-*-* } }
            options(nomem, nostack), // { dg-error "" "" { target *-*-* } }
            options(noreturn), // { dg-error "" "" { target *-*-* } }
        );
    }
}

