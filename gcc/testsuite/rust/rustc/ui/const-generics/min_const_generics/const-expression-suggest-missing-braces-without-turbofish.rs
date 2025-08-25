#![feature(min_const_generics)]

fn foo<const C: usize>() {}

const BAR: usize = 42;

fn a() {
    foo<BAR + 3>(); // { dg-error "" "" { target *-*-* } }
}
fn b() {
    foo<BAR + BAR>(); // { dg-error "" "" { target *-*-* } }
}
fn c() {
    foo<3 + 3>(); // { dg-error "" "" { target *-*-* } }
}
fn d() {
    foo<BAR - 3>(); // { dg-error "" "" { target *-*-* } }
}
fn e() {
    foo<BAR - BAR>(); // { dg-error "" "" { target *-*-* } }
}
fn f() {
    foo<100 - BAR>(); // { dg-error "" "" { target *-*-* } }
}
fn g() {
    foo<bar<i32>()>(); // { dg-error "" "" { target *-*-* } }
// { dg-error "" "" { target *-*-* } .-1 }
}
fn h() {
    foo<bar::<i32>()>(); // { dg-error "" "" { target *-*-* } }
}
fn i() {
    foo<bar::<i32>() + BAR>(); // { dg-error "" "" { target *-*-* } }
}
fn j() {
    foo<bar::<i32>() - BAR>(); // { dg-error "" "" { target *-*-* } }
}
fn k() {
    foo<BAR - bar::<i32>()>(); // { dg-error "" "" { target *-*-* } }
}
fn l() {
    foo<BAR - bar::<i32>()>(); // { dg-error "" "" { target *-*-* } }
}

const fn bar<const C: usize>() -> usize {
    C
}

fn main() {}

