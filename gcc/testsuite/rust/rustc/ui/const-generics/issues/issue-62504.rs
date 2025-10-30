// revisions: full min
#![allow(incomplete_features)]
#![cfg_attr(full, feature(const_generics))]
#![cfg_attr(full, allow(incomplete_features))]
#![cfg_attr(min, feature(min_const_generics))]

trait HasSize {
    const SIZE: usize;
}

impl<const X: usize> HasSize for ArrayHolder<X> {
    const SIZE: usize = X;
}

struct ArrayHolder<const X: usize>([u32; X]);

impl<const X: usize> ArrayHolder<X> {
    pub const fn new() -> Self {
        ArrayHolder([0; Self::SIZE])
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-error "" "" { target *-*-* } .-2 }
    }
}

fn main() {
    let mut array = ArrayHolder::new();
}

