fn main() {}

const fn unsize(x: &[u8; 3]) -> &[u8] { x }
const fn closure() -> fn() { || {} }
// { dg-error ".E0658." "" { target *-*-* } .-1 }
// { dg-error ".E0658." "" { target *-*-* } .-2 }
const fn closure2() {
    (|| {}) as fn();
// { dg-error ".E0658." "" { target *-*-* } .-1 }
}
const fn reify(f: fn()) -> unsafe fn() { f }
// { dg-error ".E0658." "" { target *-*-* } .-1 }
// { dg-error ".E0658." "" { target *-*-* } .-2 }
// { dg-error ".E0658." "" { target *-*-* } .-3 }
const fn reify2() { main as unsafe fn(); }
// { dg-error ".E0658." "" { target *-*-* } .-1 }
// { dg-error ".E0658." "" { target *-*-* } .-2 }

