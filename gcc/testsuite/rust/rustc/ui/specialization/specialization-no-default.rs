#![feature(specialization)] // { dg-warning "" "" { target *-*-* } }

// Check a number of scenarios in which one impl tries to override another,
// without correctly using `default`.

// Test 1: one layer of specialization, multiple methods, missing `default`

trait Foo {
    fn foo(&self);
    fn bar(&self);
}

impl<T> Foo for T {
    fn foo(&self) {}
    fn bar(&self) {}
}

impl Foo for u8 {}
impl Foo for u16 {
    fn foo(&self) {} // { dg-error ".E0520." "" { target *-*-* } }
}
impl Foo for u32 {
    fn bar(&self) {} // { dg-error ".E0520." "" { target *-*-* } }
}

// Test 2: one layer of specialization, missing `default` on associated type

trait Bar {
    type T;
}

impl<T> Bar for T {
    type T = u8;
}

impl Bar for u8 {
    type T = (); // { dg-error ".E0520." "" { target *-*-* } }
}

// Test 3a: multiple layers of specialization, missing interior `default`

trait Baz {
    fn baz(&self);
}

impl<T> Baz for T {
    default fn baz(&self) {}
}

impl<T: Clone> Baz for T {
    fn baz(&self) {}
}

impl Baz for i32 {
    fn baz(&self) {} // { dg-error ".E0520." "" { target *-*-* } }
}

// Test 3b: multiple layers of specialization, missing interior `default`,
// redundant `default` in bottom layer.

trait Redundant {
    fn redundant(&self);
}

impl<T> Redundant for T {
    default fn redundant(&self) {}
}

impl<T: Clone> Redundant for T {
    fn redundant(&self) {}
}

impl Redundant for i32 {
    default fn redundant(&self) {} // { dg-error ".E0520." "" { target *-*-* } }
}

fn main() {}

