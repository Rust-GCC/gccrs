// only-x86_64

#![feature(asm)]

fn main() {
    let mut foo = 0;
    let mut bar = 0;
    unsafe {
        asm!();
// { dg-error "" "" { target *-*-* } .-1 }
        asm!(foo);
// { dg-error "" "" { target *-*-* } .-1 }
        asm!("{}" foo);
// { dg-error "" "" { target *-*-* } .-1 }
        asm!("{}", foo);
// { dg-error "" "" { target *-*-* } .-1 }
        asm!("{}", in foo);
// { dg-error "" "" { target *-*-* } .-1 }
        asm!("{}", in(reg foo));
// { dg-error "" "" { target *-*-* } .-1 }
        asm!("{}", in(reg));
// { dg-error "" "" { target *-*-* } .-1 }
        asm!("{}", inout(=) foo => bar);
// { dg-error "" "" { target *-*-* } .-1 }
        asm!("{}", inout(reg) foo =>);
// { dg-error "" "" { target *-*-* } .-1 }
        asm!("{}", in(reg) foo => bar);
// { dg-error "" "" { target *-*-* } .-1 }
        asm!("{}", sym foo + bar);
// { dg-error "" "" { target *-*-* } .-1 }
        asm!("", options(foo));
// { dg-error "" "" { target *-*-* } .-1 }
        asm!("", options(nomem foo));
// { dg-error "" "" { target *-*-* } .-1 }
        asm!("", options(nomem, foo));
// { dg-error "" "" { target *-*-* } .-1 }
        asm!("{}", options(), const foo);
// { dg-error "" "" { target *-*-* } .-1 }
        asm!("{a}", a = const foo, a = const bar);
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-error "" "" { target *-*-* } .-2 }
        asm!("", a = in("eax") foo);
// { dg-error "" "" { target *-*-* } .-1 }
        asm!("{a}", in("eax") foo, a = const bar);
// { dg-error "" "" { target *-*-* } .-1 }
        asm!("{a}", in("eax") foo, a = const bar);
// { dg-error "" "" { target *-*-* } .-1 }
        asm!("{1}", in("eax") foo, const bar);
// { dg-error "" "" { target *-*-* } .-1 }
        asm!("", options(), "");
// { dg-error "" "" { target *-*-* } .-1 }
        asm!("{}", in(reg) foo, "{}", out(reg) foo);
// { dg-error "" "" { target *-*-* } .-1 }
        asm!(format!("{{{}}}", 0), in(reg) foo);
// { dg-error "" "" { target *-*-* } .-1 }
        asm!("{1}", format!("{{{}}}", 0), in(reg) foo, out(reg) bar);
// { dg-error "" "" { target *-*-* } .-1 }
    }
}

