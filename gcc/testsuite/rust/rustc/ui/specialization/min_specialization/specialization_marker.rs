// Test that `rustc_unsafe_specialization_marker` is only allowed on marker traits.

#![feature(rustc_attrs)]

#[rustc_unsafe_specialization_marker]
trait SpecMarker {
    fn f();
// { dg-error ".E0714." "" { target *-*-* } .-1 }
}

#[rustc_unsafe_specialization_marker]
trait SpecMarker2 {
    type X;
// { dg-error ".E0714." "" { target *-*-* } .-1 }
}

fn main() {}

