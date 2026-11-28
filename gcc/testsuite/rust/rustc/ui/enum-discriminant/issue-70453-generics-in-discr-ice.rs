#![feature(core_intrinsics)]

extern crate core;
use core::intrinsics::discriminant_value;

#[repr(usize)]
enum MyWeirdOption<T> {
// { dg-error ".E0392." "" { target *-*-* } .-1 }
    None = 0,
    Some = std::mem::size_of::<T>(),
// { dg-error "" "" { target *-*-* } .-1 }
}

fn main() {
    assert_eq!(discriminant_value(&MyWeirdOption::<u8>::None), 0);
    assert_eq!(discriminant_value(&MyWeirdOption::<u8>::Some), 1);
}

