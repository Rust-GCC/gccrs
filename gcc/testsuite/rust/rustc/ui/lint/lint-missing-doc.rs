// When denying at the crate level, be sure to not get random warnings from the
// injected intrinsics by the compiler.
#![deny(missing_docs)]
#![allow(dead_code)]
#![feature(associated_type_defaults, extern_types)]

//! Some garbage docs for the crate here
#![doc="More garbage"]

type Typedef = String;
pub type PubTypedef = String; // { dg-error "" "" { target *-*-* } }

struct Foo {
    a: isize,
    b: isize,
}

pub struct PubFoo { // { dg-error "" "" { target *-*-* } }
    pub a: isize,      // { dg-error "" "" { target *-*-* } }
    b: isize,
}

#[allow(missing_docs)]
pub struct PubFoo2 {
    pub a: isize,
    pub c: isize,
}

mod module_no_dox {}
pub mod pub_module_no_dox {} // { dg-error "" "" { target *-*-* } }

/// dox
pub fn foo() {}
pub fn foo2() {} // { dg-error "" "" { target *-*-* } }
fn foo3() {}
#[allow(missing_docs)] pub fn foo4() {}

/// dox
pub trait A {
    /// dox
    fn foo(&self);
    /// dox
    fn foo_with_impl(&self) {}
}

#[allow(missing_docs)]
trait B {
    fn foo(&self);
    fn foo_with_impl(&self) {}
}

pub trait C { // { dg-error "" "" { target *-*-* } }
    fn foo(&self); // { dg-error "" "" { target *-*-* } }
    fn foo_with_impl(&self) {} // { dg-error "" "" { target *-*-* } }
}

#[allow(missing_docs)]
pub trait D {
    fn dummy(&self) { }
}

/// dox
pub trait E: Sized {
    type AssociatedType; // { dg-error "" "" { target *-*-* } }
    type AssociatedTypeDef = Self; // { dg-error "" "" { target *-*-* } }

    /// dox
    type DocumentedType;
    /// dox
    type DocumentedTypeDef = Self;
    /// dox
    fn dummy(&self) {}
}

impl Foo {
    pub fn foo() {}
    fn bar() {}
}

impl PubFoo {
    pub fn foo() {} // { dg-error "" "" { target *-*-* } }
    /// dox
    pub fn foo1() {}
    fn foo2() {}
    #[allow(missing_docs)] pub fn foo3() {}
}

#[allow(missing_docs)]
trait F {
    fn a();
    fn b(&self);
}

// should need to redefine documentation for implementations of traits
impl F for Foo {
    fn a() {}
    fn b(&self) {}
}

// It sure is nice if doc(hidden) implies allow(missing_docs), and that it
// applies recursively
#[doc(hidden)]
mod a {
    pub fn baz() {}
    pub mod b {
        pub fn baz() {}
    }
}

enum Baz {
    BazA {
        a: isize,
        b: isize
    },
    BarB
}

pub enum PubBaz { // { dg-error "" "" { target *-*-* } }
    PubBazA { // { dg-error "" "" { target *-*-* } }
        a: isize, // { dg-error "" "" { target *-*-* } }
    },
}

/// dox
pub enum PubBaz2 {
    /// dox
    PubBaz2A {
        /// dox
        a: isize,
    },
}

#[allow(missing_docs)]
pub enum PubBaz3 {
    PubBaz3A {
        b: isize
    },
}

#[doc(hidden)]
pub fn baz() {}


const FOO: u32 = 0;
/// dox
pub const FOO1: u32 = 0;
#[allow(missing_docs)]
pub const FOO2: u32 = 0;
#[doc(hidden)]
pub const FOO3: u32 = 0;
pub const FOO4: u32 = 0; // { dg-error "" "" { target *-*-* } }


static BAR: u32 = 0;
/// dox
pub static BAR1: u32 = 0;
#[allow(missing_docs)]
pub static BAR2: u32 = 0;
#[doc(hidden)]
pub static BAR3: u32 = 0;
pub static BAR4: u32 = 0; // { dg-error "" "" { target *-*-* } }


mod internal_impl {
    /// dox
    pub fn documented() {}
    pub fn undocumented1() {} // { dg-error "" "" { target *-*-* } }
    pub fn undocumented2() {} // { dg-error "" "" { target *-*-* } }
    fn undocumented3() {}
    /// dox
    pub mod globbed {
        /// dox
        pub fn also_documented() {}
        pub fn also_undocumented1() {} // { dg-error "" "" { target *-*-* } }
        fn also_undocumented2() {}
    }
}
/// dox
pub mod public_interface {
    pub use internal_impl::documented as foo;
    pub use internal_impl::undocumented1 as bar;
    pub use internal_impl::{documented, undocumented2};
    pub use internal_impl::globbed::*;
}

extern "C" {
    /// dox
    pub fn extern_fn_documented(f: f32) -> f32;
    pub fn extern_fn_undocumented(f: f32) -> f32;
// { dg-error "" "" { target *-*-* } .-1 }

    /// dox
    pub static EXTERN_STATIC_DOCUMENTED: u8;
    pub static EXTERN_STATIC_UNDOCUMENTED: u8;
// { dg-error "" "" { target *-*-* } .-1 }

    /// dox
    pub type ExternTyDocumented;
    pub type ExternTyUndocumented;
// { dg-error "" "" { target *-*-* } .-1 }
}

fn main() {}

