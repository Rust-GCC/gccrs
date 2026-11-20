#![feature(negative_impls)]

use std::marker::Copy;

enum TestE {
  A
}

struct MyType;

struct NotSync;
impl !Sync for NotSync {}

impl Sized for TestE {}
// { dg-error ".E0322." "" { target *-*-* } .-1 }

impl Sized for MyType {}
// { dg-error ".E0322." "" { target *-*-* } .-1 }

impl Sized for (MyType, MyType) {}
// { dg-error ".E0117." "" { target *-*-* } .-1 }
// { dg-error ".E0117." "" { target *-*-* } .-2 }

impl Sized for &'static NotSync {}
// { dg-error ".E0322." "" { target *-*-* } .-1 }

impl Sized for [MyType] {}
// { dg-error ".E0117." "" { target *-*-* } .-1 }
// { dg-error ".E0117." "" { target *-*-* } .-2 }

impl Sized for &'static [NotSync] {}
// { dg-error ".E0117." "" { target *-*-* } .-1 }
// { dg-error ".E0117." "" { target *-*-* } .-2 }

fn main() {
}

