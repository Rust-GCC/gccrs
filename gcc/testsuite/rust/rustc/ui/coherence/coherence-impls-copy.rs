#![feature(negative_impls)]

use std::marker::Copy;

impl Copy for i32 {}
// { dg-error ".E0117." "" { target *-*-* } .-1 }
// { dg-error ".E0117." "" { target *-*-* } .-2 }
enum TestE {
  A
}

struct MyType;

struct NotSync;
impl !Sync for NotSync {}

impl Copy for TestE {}
impl Clone for TestE { fn clone(&self) -> Self { *self } }

impl Copy for MyType {}

impl Copy for &'static mut MyType {}
// { dg-error ".E0206." "" { target *-*-* } .-1 }
impl Clone for MyType { fn clone(&self) -> Self { *self } }

impl Copy for (MyType, MyType) {}
// { dg-error ".E0117." "" { target *-*-* } .-1 }
// { dg-error ".E0117." "" { target *-*-* } .-2 }
impl Copy for &'static NotSync {}
// { dg-error ".E0119." "" { target *-*-* } .-1 }
impl Copy for [MyType] {}
// { dg-error ".E0117." "" { target *-*-* } .-1 }
// { dg-error ".E0117." "" { target *-*-* } .-2 }
impl Copy for &'static [NotSync] {}
// { dg-error ".E0117." "" { target *-*-* } .-1 }
// { dg-error ".E0117." "" { target *-*-* } .-2 }
fn main() {
}

