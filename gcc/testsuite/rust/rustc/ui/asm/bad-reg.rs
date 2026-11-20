// only-x86_64
// compile-flags: -C target-feature=+avx2

#![feature(asm)]

fn main() {
    let mut foo = 0;
    let mut bar = 0;
    unsafe {
        // Bad register/register class

        asm!("{}", in(foo) foo);
// { dg-error "" "" { target *-*-* } .-1 }
        asm!("", in("foo") foo);
// { dg-error "" "" { target *-*-* } .-1 }
        asm!("{:z}", in(reg) foo);
// { dg-error "" "" { target *-*-* } .-1 }
        asm!("{:r}", in(xmm_reg) foo);
// { dg-error "" "" { target *-*-* } .-1 }
        asm!("{:a}", const 0);
// { dg-error "" "" { target *-*-* } .-1 }
        asm!("{:a}", sym main);
// { dg-error "" "" { target *-*-* } .-1 }
        asm!("{}", in(zmm_reg) foo);
// { dg-error "" "" { target *-*-* } .-1 }
        asm!("", in("zmm0") foo);
// { dg-error "" "" { target *-*-* } .-1 }
        asm!("", in("ebp") foo);
// { dg-error "" "" { target *-*-* } .-1 }
        asm!("", in("rsp") foo);
// { dg-error "" "" { target *-*-* } .-1 }
        asm!("", in("ip") foo);
// { dg-error "" "" { target *-*-* } .-1 }
        asm!("", in("st(2)") foo);
// { dg-error "" "" { target *-*-* } .-1 }
        asm!("", in("mm0") foo);
// { dg-error "" "" { target *-*-* } .-1 }
        asm!("", in("k0") foo);
// { dg-error "" "" { target *-*-* } .-1 }

        // Explicit register conflicts
        // (except in/lateout which don't conflict)

        asm!("", in("eax") foo, in("al") bar);
// { dg-error "" "" { target *-*-* } .-1 }
        asm!("", in("rax") foo, out("rax") bar);
// { dg-error "" "" { target *-*-* } .-1 }
        asm!("", in("al") foo, lateout("al") bar);
        asm!("", in("xmm0") foo, in("ymm0") bar);
// { dg-error "" "" { target *-*-* } .-1 }
        asm!("", in("xmm0") foo, out("ymm0") bar);
// { dg-error "" "" { target *-*-* } .-1 }
        asm!("", in("xmm0") foo, lateout("ymm0") bar);
    }
}

