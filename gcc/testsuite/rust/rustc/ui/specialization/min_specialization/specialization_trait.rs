// Test that `rustc_specialization_trait` requires always applicable impls.

#![feature(min_specialization)]
#![feature(rustc_attrs)]

#[rustc_specialization_trait]
trait SpecMarker {
    fn f();
}

impl SpecMarker for &'static u8 {
// { dg-error "" "" { target *-*-* } .-1 }
    fn f() {}
}

impl<T> SpecMarker for (T, T) {
// { dg-error "" "" { target *-*-* } .-1 }
    fn f() {}
}

impl<T: Clone> SpecMarker for [T] {
// { dg-error "" "" { target *-*-* } .-1 }
    fn f() {}
}

fn main() {}

