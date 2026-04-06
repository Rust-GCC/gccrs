// { dg-options "-frust-dump-lex" }
#![feature(no_core)]
#![no_core]

fn main() {
    let _a: u32 = 1_000u32;
    let _b: u8 = 0xFF_u8;
    let _c: f64 = 3.14_15_f64;

    let _d = 0b1010;
    let _e = 10.5;

    let _f = 2.71e+10_f32;
}

// { dg-final { scan-file gccrs.lex.dump "1_000u32" } }
// { dg-final { scan-file gccrs.lex.dump "0xFF_u8" } }
// { dg-final { scan-file gccrs.lex.dump "3.14_15_f64" } }
// { dg-final { scan-file gccrs.lex.dump "0b1010" } }
// { dg-final { scan-file gccrs.lex.dump "10.5" } }
// { dg-final { scan-file gccrs.lex.dump "2.71e\\+10_f32" } }
