#![feature(negative_impls)]

use std::marker::Copy;

enum TestE {
    A,
}

struct MyType;

struct NotSync;
impl !Sync for NotSync {}

unsafe impl Send for TestE {}
unsafe impl Send for MyType {}
unsafe impl Send for (MyType, MyType) {}
// { dg-error ".E0117." "" { target *-*-* } .-1 }

unsafe impl Send for &'static NotSync {}
// { dg-error ".E0321." "" { target *-*-* } .-1 }

unsafe impl Send for [MyType] {}
// { dg-error ".E0117." "" { target *-*-* } .-1 }

unsafe impl Send for &'static [NotSync] {}
// { dg-error ".E0117." "" { target *-*-* } .-1 }
// { dg-error ".E0117." "" { target *-*-* } .-2 }

fn main() {}

