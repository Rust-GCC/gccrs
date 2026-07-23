// Make sure that underscore imports have the same hygiene considerations as
// other imports.

#![feature(decl_macro)]

mod x {
    pub use std::ops::Deref as _;
}


macro glob_import() {
    pub use crate::x::*;
}

macro underscore_import() {
    use std::ops::DerefMut as _;
}

mod y {
    crate::glob_import!();
    crate::underscore_import!();
}

macro create_module($y:ident) {
    mod $y {
        crate::glob_import!();
        crate::underscore_import!();
    }
}

create_module!(z);

fn main() {
    use crate::y::*;
    use crate::z::*;
    glob_import!();
    underscore_import!();
    (&()).deref();              // { dg-error ".E0599." "" { target *-*-* } }
    (&mut ()).deref_mut();      // { dg-error ".E0599." "" { target *-*-* } }
}

