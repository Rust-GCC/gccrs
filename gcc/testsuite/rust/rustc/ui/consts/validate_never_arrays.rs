#![feature(const_raw_ptr_deref, never_type)]

const _: &[!; 1] = unsafe { &*(1_usize as *const [!; 1]) }; // { dg-error ".E0080." "" { target *-*-* } }
const _: &[!; 0] = unsafe { &*(1_usize as *const [!; 0]) }; // ok
const _: &[!] = unsafe { &*(1_usize as *const [!; 0]) }; // ok
const _: &[!] = unsafe { &*(1_usize as *const [!; 1]) }; // { dg-error ".E0080." "" { target *-*-* } }
const _: &[!] = unsafe { &*(1_usize as *const [!; 42]) }; // { dg-error ".E0080." "" { target *-*-* } }

fn main() {}

