#![allow(dead_code)]
#![allow(unused_variables)]
#![allow(non_snake_case)]
#![deny(unreachable_patterns)]

#[derive(Clone, Copy)]
enum Enum {
    Var1,
    Var2,
}

fn main() {
    use Enum::*;
    let s = Var1;
    match s {
        Var1 => (),
        Var3 => (),
        Var2 => (),
// { dg-error "" "" { target *-*-* } .-1 }
    };
    match &s {
        &Var1 => (),
        &Var3 => (),
        &Var2 => (),
// { dg-error "" "" { target *-*-* } .-1 }
    };
    let t = (Var1, Var1);
    match t {
        (Var1, b) => (),
        (c, d) => (),
        anything => ()
// { dg-error "" "" { target *-*-* } .-1 }
    };
}

