// run-pass
#![feature(arbitrary_enum_discriminant, core_intrinsics)]

extern crate core;
use core::intrinsics::discriminant_value;

#[repr(usize)]
enum MyWeirdOption<T> {
    None = 0,
    Some(T) = core::mem::size_of::<*mut T>(),
// { dg-warning "" "" { target *-*-* } .-1 }
// { dg-warning "" "" { target *-*-* } .-2 }
}

fn main() {
    assert_eq!(discriminant_value(&MyWeirdOption::<()>::None), 0);
    assert_eq!(discriminant_value(&MyWeirdOption::Some(())), core::mem::size_of::<usize>());
}

