#![allow(dead_code)]

#[repr(align(16.0))] // { dg-error ".E0589." "" { target *-*-* } }
// { dg-error ".E0589." "" { target *-*-* } .-2 }
struct S0(i32);

#[repr(align(15))] // { dg-error ".E0589." "" { target *-*-* } }
// { dg-error ".E0589." "" { target *-*-* } .-2 }
struct S1(i32);

#[repr(align(4294967296))] // { dg-error ".E0589." "" { target *-*-* } }
// { dg-error ".E0589." "" { target *-*-* } .-2 }
struct S2(i32);

#[repr(align(536870912))] // ok: this is the largest accepted alignment
struct S3(i32);

#[repr(align(16.0))] // { dg-error ".E0589." "" { target *-*-* } }
// { dg-error ".E0589." "" { target *-*-* } .-2 }
enum E0 { A, B }

#[repr(align(15))] // { dg-error ".E0589." "" { target *-*-* } }
// { dg-error ".E0589." "" { target *-*-* } .-2 }
enum E1 { A, B }

#[repr(align(4294967296))] // { dg-error ".E0589." "" { target *-*-* } }
// { dg-error ".E0589." "" { target *-*-* } .-2 }
enum E2 { A, B }

#[repr(align(536870912))] // ok: this is the largest accepted alignment
enum E3 { A, B }

fn main() {}

