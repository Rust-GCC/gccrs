// revisions: full min
#![cfg_attr(full, feature(const_generics))]
#![cfg_attr(full, allow(incomplete_features))]
#![cfg_attr(min, feature(min_const_generics))]

trait A {}
struct B;
impl A for B {}

fn test<const T: &'static dyn A>() {
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-error "" "" { target *-*-* } .-2 }
    unimplemented!()
}

fn main() {
    test::<{ &B }>();
}

