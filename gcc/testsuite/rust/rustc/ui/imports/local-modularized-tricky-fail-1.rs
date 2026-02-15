#![feature(decl_macro)]

macro_rules! define_exported { () => {
    #[macro_export]
    macro_rules! exported {
        () => ()
    }
}}
macro_rules! define_panic { () => {
    #[macro_export]
    macro_rules! panic {
        () => ()
    }
}}
macro_rules! define_include { () => {
    #[macro_export]
    macro_rules! include {
        () => ()
    }
}}

use inner1::*;

mod inner1 {
    pub macro exported() {}
}

exported!(); // { dg-error ".E0659." "" { target *-*-* } }
// { dg-error "" "" { target *-*-* } .-2 }

mod inner2 {
    define_exported!();
}

fn main() {
    panic!(); // { dg-error ".E0659." "" { target *-*-* } }
}

mod inner3 {
    define_panic!();
}

mod inner4 {
    define_include!();
}

include!(); // { dg-error ".E0659." "" { target *-*-* } }

