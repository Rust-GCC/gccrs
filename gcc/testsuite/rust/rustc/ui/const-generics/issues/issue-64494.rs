// revisions: full min
#![cfg_attr(full, feature(const_generics))]
#![cfg_attr(full, allow(incomplete_features))]
#![cfg_attr(min, feature(min_const_generics))]

trait Foo {
    const VAL: usize;
}

trait MyTrait {}

trait True {}
struct Is<const T: bool>;
impl True for Is<{true}> {}

impl<T: Foo> MyTrait for T where Is<{T::VAL == 5}>: True {}
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-error "" "" { target *-*-* } .-2 }
impl<T: Foo> MyTrait for T where Is<{T::VAL == 6}>: True {}
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-error "" "" { target *-*-* } .-2 }
// { dg-error "" "" { target *-*-* } .-2 }

fn main() {}

