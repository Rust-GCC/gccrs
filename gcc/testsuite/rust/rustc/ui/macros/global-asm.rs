#![feature(global_asm)]

fn main() {
    global_asm!();  // { dg-error "" "" { target *-*-* } }
    global_asm!(struct); // { dg-error "" "" { target *-*-* } }
    global_asm!(123); // { dg-error "" "" { target *-*-* } }
}

