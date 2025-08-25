#![allow(unused_unsafe)]
#![allow(dead_code)]
#![deny(unsafe_code)]

struct Bar;
struct Bar2;
struct Bar3;

#[allow(unsafe_code)]
mod allowed_unsafe {
    fn allowed() { unsafe {} }
    unsafe fn also_allowed() {}
    unsafe trait AllowedUnsafe { }
    unsafe impl AllowedUnsafe for super::Bar {}
}

macro_rules! unsafe_in_macro {
    () => {
        unsafe {} // { dg-error "" "" { target *-*-* } }
    }
}

unsafe fn baz() {} // { dg-error "" "" { target *-*-* } }
unsafe trait Foo {} // { dg-error "" "" { target *-*-* } }
unsafe impl Foo for Bar {} // { dg-error "" "" { target *-*-* } }

trait Baz {
    unsafe fn baz(&self); // { dg-error "" "" { target *-*-* } }
    unsafe fn provided(&self) {} // { dg-error "" "" { target *-*-* } }
    unsafe fn provided_override(&self) {} // { dg-error "" "" { target *-*-* } }
}

impl Baz for Bar {
    unsafe fn baz(&self) {} // { dg-error "" "" { target *-*-* } }
    unsafe fn provided_override(&self) {} // { dg-error "" "" { target *-*-* } }
}


#[allow(unsafe_code)]
trait A {
    unsafe fn allowed_unsafe(&self);
    unsafe fn allowed_unsafe_provided(&self) {}
}

#[allow(unsafe_code)]
impl Baz for Bar2 {
    unsafe fn baz(&self) {}
    unsafe fn provided_override(&self) {}
}

impl Baz for Bar3 {
    #[allow(unsafe_code)]
    unsafe fn baz(&self) {}
    unsafe fn provided_override(&self) {} // { dg-error "" "" { target *-*-* } }
}

#[allow(unsafe_code)]
unsafe trait B {
    fn dummy(&self) {}
}

trait C {
    #[allow(unsafe_code)]
    unsafe fn baz(&self);
    unsafe fn provided(&self) {} // { dg-error "" "" { target *-*-* } }
}

impl C for Bar {
    #[allow(unsafe_code)]
    unsafe fn baz(&self) {}
    unsafe fn provided(&self) {} // { dg-error "" "" { target *-*-* } }
}

impl C for Bar2 {
    unsafe fn baz(&self) {} // { dg-error "" "" { target *-*-* } }
}

trait D {
    #[allow(unsafe_code)]
    unsafe fn unsafe_provided(&self) {}
}

impl D for Bar {}

fn main() {
    unsafe {} // { dg-error "" "" { target *-*-* } }

    unsafe_in_macro!()
}

