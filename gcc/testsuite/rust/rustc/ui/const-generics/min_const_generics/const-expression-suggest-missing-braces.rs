#![feature(min_const_generics)]

fn foo<const C: usize>() {}

const BAR: usize = 42;

fn a() {
    foo::<BAR + 3>(); // { dg-error "" "" { target *-*-* } }
}
fn b() {
    // FIXME(const_generics): these diagnostics are awful, because trait objects without `dyn` were
    // a terrible mistake.
    foo::<BAR + BAR>();
// { dg-error ".E0107." "" { target *-*-* } .-1 }
// { dg-error ".E0107." "" { target *-*-* } .-2 }
// { dg-error ".E0107." "" { target *-*-* } .-3 }
// { dg-error ".E0107." "" { target *-*-* } .-4 }
// { dg-warning ".E0107." "" { target *-*-* } .-5 }
}
fn c() {
    foo::<3 + 3>(); // { dg-error "" "" { target *-*-* } }
}
fn d() {
    foo::<BAR - 3>(); // { dg-error "" "" { target *-*-* } }
}
fn e() {
    foo::<BAR - BAR>(); // { dg-error "" "" { target *-*-* } }
}
fn f() {
    foo::<100 - BAR>(); // { dg-error "" "" { target *-*-* } }
}
fn g() {
    foo::<bar<i32>()>(); // { dg-error "" "" { target *-*-* } }
}
fn h() {
    foo::<bar::<i32>()>(); // { dg-error "" "" { target *-*-* } }
}
fn i() {
    foo::<bar::<i32>() + BAR>(); // { dg-error "" "" { target *-*-* } }
}
fn j() {
    foo::<bar::<i32>() - BAR>(); // { dg-error "" "" { target *-*-* } }
}
fn k() {
    foo::<BAR - bar::<i32>()>(); // { dg-error "" "" { target *-*-* } }
}
fn l() {
    foo::<BAR - bar::<i32>()>(); // { dg-error "" "" { target *-*-* } }
}

const fn bar<const C: usize>() -> usize {
    C
}

fn main() {}

