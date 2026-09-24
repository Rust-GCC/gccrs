#![feature(const_fn)]
#![feature(const_fn_transmute)]

const fn foo() -> ! {
    unsafe { std::mem::transmute(()) }
// { dg-warning "" "" { target *-*-* } .-1 }
// { dg-warning "" "" { target *-*-* } .-2 }
}

#[derive(Clone, Copy)]
enum Empty { }

#[warn(const_err)]
const FOO: [Empty; 3] = [foo(); 3];

#[warn(const_err)]
const BAR: [Empty; 3] = [unsafe { std::mem::transmute(()) }; 3];
// { dg-error ".E0080." "" { target *-*-* } .-1 }
// { dg-warning ".E0080." "" { target *-*-* } .-2 }

fn main() {
    FOO;
    BAR;
}

