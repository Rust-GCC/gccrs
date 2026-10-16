#![feature(specialization, associated_type_defaults)]
// { dg-warning "" "" { target *-*-* } .-1 }

// Test that attempting to override a non-default method or one not in the
// parent impl causes an error.

trait Foo {
    type Ty = ();
    const CONST: u8 = 123;
    fn foo(&self) -> bool { true }
}

// Specialization tree for Foo:
//
//       Box<T>              Vec<T>
//        / \                 / \
// Box<i32>  Box<i64>   Vec<()>  Vec<bool>

impl<T> Foo for Box<T> {
    type Ty = bool;
    const CONST: u8 = 0;
    fn foo(&self) -> bool { false }
}

// Allowed
impl Foo for Box<i32> {}

// Can't override a non-`default` fn
impl Foo for Box<i64> {
    type Ty = Vec<()>;
// { dg-error ".E0520." "" { target *-*-* } .-1 }
    const CONST: u8 = 42;
// { dg-error ".E0520." "" { target *-*-* } .-1 }
    fn foo(&self) -> bool { true }
// { dg-error ".E0520." "" { target *-*-* } .-1 }
}


// Doesn't mention the item = provided body/value is used and the method is final.
impl<T> Foo for Vec<T> {}

// Allowed
impl Foo for Vec<()> {}

impl Foo for Vec<bool> {
    type Ty = Vec<()>;
// { dg-error ".E0520." "" { target *-*-* } .-1 }
    const CONST: u8 = 42;
// { dg-error ".E0520." "" { target *-*-* } .-1 }
    fn foo(&self) -> bool { true }
// { dg-error ".E0520." "" { target *-*-* } .-1 }
}

fn main() {}

