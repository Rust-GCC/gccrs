#![feature(never_type)]
#![deny(uninhabited_static)]

enum Void {}
extern {
    static VOID: Void; // { dg-error "" "" { target *-*-* } }
// { dg-warning "" "" { target *-*-* } .-2 }
    static NEVER: !; // { dg-error "" "" { target *-*-* } }
// { dg-warning "" "" { target *-*-* } .-2 }
}

static VOID2: Void = unsafe { std::mem::transmute(()) }; // { dg-error "" "" { target *-*-* } }
// { dg-warning "" "" { target *-*-* } .-2 }
static NEVER2: Void = unsafe { std::mem::transmute(()) }; // { dg-error "" "" { target *-*-* } }
// { dg-warning "" "" { target *-*-* } .-2 }

fn main() {}

