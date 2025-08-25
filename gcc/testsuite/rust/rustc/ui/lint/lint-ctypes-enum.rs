#![feature(transparent_unions)]
#![feature(ptr_internals)]
#![deny(improper_ctypes)]
#![allow(dead_code)]

use std::num;

enum Z { }
enum U { A }
enum B { C, D }
enum T { E, F, G }

#[repr(C)]
enum ReprC { A, B, C }

#[repr(u8)]
enum U8 { A, B, C }

#[repr(isize)]
enum Isize { A, B, C }

#[repr(transparent)]
struct TransparentStruct<T>(T, std::marker::PhantomData<Z>);

#[repr(transparent)]
enum TransparentEnum<T> {
   Variant(T, std::marker::PhantomData<Z>),
}

#[repr(transparent)]
union TransparentUnion<T: Copy> {
   field: T,
}

struct Rust<T>(T);

extern {
   fn zf(x: Z);
   fn uf(x: U); // { dg-error "" "" { target *-*-* } }
   fn bf(x: B); // { dg-error "" "" { target *-*-* } }
   fn tf(x: T); // { dg-error "" "" { target *-*-* } }
   fn repr_c(x: ReprC);
   fn repr_u8(x: U8);
   fn repr_isize(x: Isize);
   fn option_ref(x: Option<&'static u8>);
   fn option_fn(x: Option<extern "C" fn()>);
   fn nonnull(x: Option<std::ptr::NonNull<u8>>);
   fn unique(x: Option<std::ptr::Unique<u8>>);
// { dg-error "" "" { target *-*-* } .-1 }
   fn nonzero_u8(x: Option<num::NonZeroU8>);
   fn nonzero_u16(x: Option<num::NonZeroU16>);
   fn nonzero_u32(x: Option<num::NonZeroU32>);
   fn nonzero_u64(x: Option<num::NonZeroU64>);
   fn nonzero_u128(x: Option<num::NonZeroU128>);
// { dg-error "" "" { target *-*-* } .-1 }
   fn nonzero_usize(x: Option<num::NonZeroUsize>);
   fn nonzero_i8(x: Option<num::NonZeroI8>);
   fn nonzero_i16(x: Option<num::NonZeroI16>);
   fn nonzero_i32(x: Option<num::NonZeroI32>);
   fn nonzero_i64(x: Option<num::NonZeroI64>);
   fn nonzero_i128(x: Option<num::NonZeroI128>);
// { dg-error "" "" { target *-*-* } .-1 }
   fn nonzero_isize(x: Option<num::NonZeroIsize>);
   fn transparent_struct(x: Option<TransparentStruct<num::NonZeroU8>>);
   fn transparent_enum(x: Option<TransparentEnum<num::NonZeroU8>>);
   fn transparent_union(x: Option<TransparentUnion<num::NonZeroU8>>);
// { dg-error "" "" { target *-*-* } .-1 }
   fn repr_rust(x: Option<Rust<num::NonZeroU8>>); // { dg-error "" "" { target *-*-* } }
   fn no_result(x: Result<(), num::NonZeroI32>); // { dg-error "" "" { target *-*-* } }
}

pub fn main() {}

