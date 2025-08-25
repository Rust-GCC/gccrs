#![feature(negative_impls)]

struct Managed;
impl !Send for Managed {}
impl !Sync for Managed {}

use std::cell::UnsafeCell;

struct MySync {
   t: *mut u8
}

unsafe impl Sync for MySync {}

struct MyNotSync {
   t: *mut u8
}

impl !Sync for MyNotSync {}

struct MyTypeWUnsafe {
   t: UnsafeCell<u8>
}

struct MyTypeManaged {
   t: Managed
}

fn is_sync<T: Sync>() {}

fn main() {
    is_sync::<MySync>();
    is_sync::<MyNotSync>();
// { dg-error ".E0277." "" { target *-*-* } .-1 }

    is_sync::<MyTypeWUnsafe>();
// { dg-error ".E0277." "" { target *-*-* } .-1 }

    is_sync::<MyTypeManaged>();
// { dg-error ".E0277." "" { target *-*-* } .-1 }
}

