// only-x86_64

#![feature(asm)]

macro_rules! m {
    ($in:ident $out:ident $lateout:ident $inout:ident $inlateout:ident $const:ident $sym:ident
     $pure:ident $nomem:ident $readonly:ident $preserves_flags:ident
     $noreturn:ident $nostack:ident $att_syntax:ident $options:ident) => {
        unsafe {
            asm!("", $in(x) x, $out(x) x, $lateout(x) x, $inout(x) x, $inlateout(x) x,
// { dg-error "" "" { target *-*-* } .-1 }
            const x, sym x,
            $options($pure, $nomem, $readonly, $preserves_flags, $noreturn, $nostack, $att_syntax));
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-error "" "" { target *-*-* } .-2 }
        }
    };
}

fn main() {
    m!(in out lateout inout inlateout const sym
       pure nomem readonly preserves_flags
       noreturn nostack att_syntax options);
}

