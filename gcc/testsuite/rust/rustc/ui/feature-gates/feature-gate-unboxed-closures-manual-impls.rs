// Test that manual impls of the `Fn` traits are not possible without
// a feature gate. In fact, the specialized check for these cases
// never triggers (yet), because they encounter other problems around
// angle bracket vs parentheses notation.

#![feature(fn_traits)]

struct Foo;
impl Fn<()> for Foo {
// { dg-error ".E0183." "" { target *-*-* } .-1 }
// { dg-error ".E0183." "" { target *-*-* } .-2 }
    extern "rust-call" fn call(self, args: ()) -> () {}
// { dg-error ".E0658." "" { target *-*-* } .-1 }
}
struct Foo1;
impl FnOnce() for Foo1 {
// { dg-error ".E0183." "" { target *-*-* } .-1 }
// { dg-error ".E0183." "" { target *-*-* } .-2 }
    extern "rust-call" fn call_once(self, args: ()) -> () {}
// { dg-error ".E0658." "" { target *-*-* } .-1 }
}
struct Bar;
impl FnMut<()> for Bar {
// { dg-error ".E0183." "" { target *-*-* } .-1 }
// { dg-error ".E0183." "" { target *-*-* } .-2 }
    extern "rust-call" fn call_mut(&self, args: ()) -> () {}
// { dg-error ".E0658." "" { target *-*-* } .-1 }
}
struct Baz;
impl FnOnce<()> for Baz {
// { dg-error ".E0183." "" { target *-*-* } .-1 }
// { dg-error ".E0183." "" { target *-*-* } .-2 }
    extern "rust-call" fn call_once(&self, args: ()) -> () {}
// { dg-error ".E0658." "" { target *-*-* } .-1 }
}

fn main() {}

