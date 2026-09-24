fn main() {
    f1(|_: (), _: ()| {}); // { dg-error ".E0631." "" { target *-*-* } }
    f2(|_: (), _: ()| {}); // { dg-error ".E0631." "" { target *-*-* } }
    f3(|_: (), _: ()| {}); // { dg-error ".E0631." "" { target *-*-* } }
    f4(|_: (), _: ()| {}); // { dg-error ".E0631." "" { target *-*-* } }
    f5(|_: (), _: ()| {}); // { dg-error ".E0631." "" { target *-*-* } }
    g1(|_: (), _: ()| {}); // { dg-error ".E0631." "" { target *-*-* } }
    g2(|_: (), _: ()| {}); // { dg-error ".E0631." "" { target *-*-* } }
    g3(|_: (), _: ()| {}); // { dg-error ".E0631." "" { target *-*-* } }
    g4(|_: (), _: ()| {}); // { dg-error ".E0631." "" { target *-*-* } }
    h1(|_: (), _: (), _: (), _: ()| {}); // { dg-error ".E0631." "" { target *-*-* } }
    h2(|_: (), _: (), _: (), _: ()| {}); // { dg-error ".E0631." "" { target *-*-* } }
}

// Basic
fn f1<F>(_: F) where F: Fn(&(), &()) {}
fn f2<F>(_: F) where F: for<'a> Fn(&'a (), &()) {}
fn f3<'a, F>(_: F) where F: Fn(&'a (), &()) {}
fn f4<F>(_: F) where F: for<'r> Fn(&(), &'r ()) {}
fn f5<F>(_: F) where F: for<'r> Fn(&'r (), &'r ()) {}

// Nested
fn g1<F>(_: F) where F: Fn(&(), Box<dyn Fn(&())>) {}
fn g2<F>(_: F) where F: Fn(&(), fn(&())) {}
fn g3<F>(_: F) where F: for<'s> Fn(&'s (), Box<dyn Fn(&())>) {}
fn g4<F>(_: F) where F: Fn(&(), for<'r> fn(&'r ())) {}

// Mixed
fn h1<F>(_: F) where F: Fn(&(), Box<dyn Fn(&())>, &(), fn(&(), &())) {}
fn h2<F>(_: F) where F: for<'t0> Fn(&(), Box<dyn Fn(&())>, &'t0 (), fn(&(), &())) {}

