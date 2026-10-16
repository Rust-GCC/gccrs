#![feature(repr_simd)]

#[repr(C)] // { dg-error ".E0517." "" { target *-*-* } }
fn f() {}

#[repr(C)]
struct SExtern(f64, f64);

#[repr(packed)]
struct SPacked(f64, f64);

#[repr(simd)]
struct SSimd(f64, f64);

#[repr(i8)] // { dg-error ".E0517." "" { target *-*-* } }
struct SInt(f64, f64);

#[repr(C)]
enum EExtern { A, B }

#[repr(align(8))]
enum EAlign { A, B }

#[repr(packed)] // { dg-error ".E0517." "" { target *-*-* } }
enum EPacked { A, B }

#[repr(simd)] // { dg-error ".E0517." "" { target *-*-* } }
enum ESimd { A, B }

#[repr(i8)]
enum EInt { A, B }

fn main() {}

