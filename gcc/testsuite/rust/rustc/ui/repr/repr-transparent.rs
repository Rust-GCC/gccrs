// This file tests repr(transparent)-related errors reported during typeck. Other errors
// that are reported earlier and therefore preempt these are tested in:
// - repr-transparent-other-reprs.rs
// - repr-transparent-other-items.rs

#![feature(transparent_unions)]

use std::marker::PhantomData;

#[repr(transparent)]
struct NoFields; // { dg-error ".E0690." "" { target *-*-* } }

#[repr(transparent)]
struct ContainsOnlyZst(()); // { dg-error ".E0690." "" { target *-*-* } }

#[repr(transparent)]
struct ContainsOnlyZstArray([bool; 0]); // { dg-error ".E0690." "" { target *-*-* } }

#[repr(transparent)]
struct ContainsMultipleZst(PhantomData<*const i32>, NoFields);
// { dg-error ".E0690." "" { target *-*-* } .-1 }

#[repr(transparent)]
struct MultipleNonZst(u8, u8); // { dg-error ".E0690." "" { target *-*-* } }

trait Mirror { type It: ?Sized; }
impl<T: ?Sized> Mirror for T { type It = Self; }

#[repr(transparent)]
pub struct StructWithProjection(f32, <f32 as Mirror>::It);
// { dg-error ".E0690." "" { target *-*-* } .-1 }

#[repr(transparent)]
struct NontrivialAlignZst(u32, [u16; 0]); // { dg-error ".E0691." "" { target *-*-* } }

#[repr(align(32))]
struct ZstAlign32<T>(PhantomData<T>);

#[repr(transparent)]
struct GenericAlign<T>(ZstAlign32<T>, u32); // { dg-error ".E0691." "" { target *-*-* } }

#[repr(transparent)] // { dg-error ".E0084." "" { target *-*-* } }
enum Void {}
// { dg-error ".E0731." "" { target *-*-* } .-1 }

#[repr(transparent)]
enum FieldlessEnum { // { dg-error ".E0690." "" { target *-*-* } }
    Foo,
}

#[repr(transparent)]
enum TooManyFieldsEnum {
    Foo(u32, String),
}
// { dg-error ".E0690." "" { target *-*-* } .-3 }

#[repr(transparent)]
enum TooManyVariants { // { dg-error ".E0731." "" { target *-*-* } }
    Foo(String),
    Bar,
}

#[repr(transparent)]
enum NontrivialAlignZstEnum {
    Foo(u32, [u16; 0]), // { dg-error ".E0691." "" { target *-*-* } }
}

#[repr(transparent)]
enum GenericAlignEnum<T> {
    Foo { bar: ZstAlign32<T>, baz: u32 } // { dg-error ".E0691." "" { target *-*-* } }
}

#[repr(transparent)]
union UnitUnion { // { dg-error ".E0690." "" { target *-*-* } }
    u: (),
}

#[repr(transparent)]
union TooManyFields { // { dg-error ".E0690." "" { target *-*-* } }
    u: u32,
    s: i32
}

fn main() {}

