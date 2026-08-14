#![feature(c_variadic)]

fn main() {}

fn f1_1(x: isize, ...) {}
// { dg-error "" "" { target *-*-* } .-1 }

fn f1_2(...) {}
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-error "" "" { target *-*-* } .-2 }

extern "C" fn f2_1(x: isize, ...) {}
// { dg-error "" "" { target *-*-* } .-1 }

extern "C" fn f2_2(...) {}
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-error "" "" { target *-*-* } .-2 }

extern "C" fn f2_3(..., x: isize) {}
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-error "" "" { target *-*-* } .-2 }

extern fn f3_1(x: isize, ...) {}
// { dg-error "" "" { target *-*-* } .-1 }

extern fn f3_2(...) {}
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-error "" "" { target *-*-* } .-2 }

extern fn f3_3(..., x: isize) {}
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-error "" "" { target *-*-* } .-2 }

extern {
    fn e_f1(...);
// { dg-error "" "" { target *-*-* } .-1 }
    fn e_f2(..., x: isize);
// { dg-error "" "" { target *-*-* } .-1 }
}

struct X;

impl X {
    fn i_f1(x: isize, ...) {}
// { dg-error "" "" { target *-*-* } .-1 }
    fn i_f2(...) {}
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-error "" "" { target *-*-* } .-2 }
    fn i_f3(..., x: isize, ...) {}
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-error "" "" { target *-*-* } .-2 }
// { dg-error "" "" { target *-*-* } .-3 }
    fn i_f4(..., x: isize, ...) {}
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-error "" "" { target *-*-* } .-2 }
// { dg-error "" "" { target *-*-* } .-3 }
}

trait T {
    fn t_f1(x: isize, ...) {}
// { dg-error "" "" { target *-*-* } .-1 }
    fn t_f2(x: isize, ...);
// { dg-error "" "" { target *-*-* } .-1 }
    fn t_f3(...) {}
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-error "" "" { target *-*-* } .-2 }
    fn t_f4(...);
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-error "" "" { target *-*-* } .-2 }
    fn t_f5(..., x: isize) {}
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-error "" "" { target *-*-* } .-2 }
    fn t_f6(..., x: isize);
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-error "" "" { target *-*-* } .-2 }
}

