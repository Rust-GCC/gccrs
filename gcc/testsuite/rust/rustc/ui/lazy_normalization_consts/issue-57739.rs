#![feature(lazy_normalization_consts)]
// { dg-warning "" "" { target *-*-* } .-1 }
trait ArraySizeTrait {
    const SIZE: usize = 0;
}

impl<T: ?Sized> ArraySizeTrait for T {
    const SIZE: usize = 1;
}

struct SomeArray<T: ArraySizeTrait> {
    array: [u8; T::SIZE],
// { dg-error "" "" { target *-*-* } .-1 }
    phantom: std::marker::PhantomData<T>,
}

fn main() {}

