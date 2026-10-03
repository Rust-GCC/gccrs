#[repr(simd)] // { dg-error ".E0658." "" { target *-*-* } }
struct Foo(u64, u64);

#[repr(C)] // { dg-error ".E0566." "" { target *-*-* } }
// { dg-warning ".E0566." "" { target *-*-* } .-1 }
#[repr(simd)] // { dg-error ".E0658." "" { target *-*-* } }
struct Bar(u64, u64);

fn main() {}

