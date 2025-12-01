// ignore-tidy-linelength
#![allow(const_err, invalid_value)] // make sure we cannot allow away the errors tested here

use std::mem;

const UNALIGNED: &u16 = unsafe { mem::transmute(&[0u8; 4]) };
// { dg-error ".E0080." "" { target *-*-* } .-1 }
// { dg-error ".E0080." "" { target *-*-* } .-2 }

const UNALIGNED_BOX: Box<u16> = unsafe { mem::transmute(&[0u8; 4]) };
// { dg-error ".E0080." "" { target *-*-* } .-1 }
// { dg-error ".E0080." "" { target *-*-* } .-2 }

const NULL: &u16 = unsafe { mem::transmute(0usize) };
// { dg-error ".E0080." "" { target *-*-* } .-1 }

const NULL_BOX: Box<u16> = unsafe { mem::transmute(0usize) };
// { dg-error ".E0080." "" { target *-*-* } .-1 }

// It is very important that we reject this: We do promote `&(4 * REF_AS_USIZE)`,
// but that would fail to compile; so we ended up breaking user code that would
// have worked fine had we not promoted.
const REF_AS_USIZE: usize = unsafe { mem::transmute(&0) };
// { dg-error ".E0080." "" { target *-*-* } .-1 }

const REF_AS_USIZE_SLICE: &[usize] = &[unsafe { mem::transmute(&0) }];
// { dg-error ".E0080." "" { target *-*-* } .-1 }

const REF_AS_USIZE_BOX_SLICE: Box<[usize]> = unsafe { mem::transmute::<&[usize], _>(&[mem::transmute(&0)]) };
// { dg-error ".E0080." "" { target *-*-* } .-1 }

const USIZE_AS_REF: &'static u8 = unsafe { mem::transmute(1337usize) };
// { dg-error ".E0080." "" { target *-*-* } .-1 }

const USIZE_AS_BOX: Box<u8> = unsafe { mem::transmute(1337usize) };
// { dg-error ".E0080." "" { target *-*-* } .-1 }

fn main() {}

