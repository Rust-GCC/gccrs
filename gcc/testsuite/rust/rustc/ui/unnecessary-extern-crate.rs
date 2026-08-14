// edition:2018

#![deny(unused_extern_crates)]
#![feature(test, rustc_private, crate_visibility_modifier)]

extern crate libc;
// { dg-error "" "" { target *-*-* } .-1 }
// { help "" "" { target *-*-* } .-2 }
extern crate libc as x;
// { dg-error "" "" { target *-*-* } .-1 }
// { help "" "" { target *-*-* } .-2 }

extern crate proc_macro;

#[macro_use]
extern crate test;

pub extern crate test as y;

pub extern crate alloc;

pub(crate) extern crate alloc as a;

crate extern crate alloc as b;

mod foo {
    pub(in crate::foo) extern crate alloc as c;

    pub(super) extern crate alloc as d;

    extern crate libc;
// { dg-error "" "" { target *-*-* } .-1 }
// { help "" "" { target *-*-* } .-2 }

    extern crate libc as x;
// { dg-error "" "" { target *-*-* } .-1 }
// { help "" "" { target *-*-* } .-2 }

    pub extern crate test;

    pub extern crate test as y;

    mod bar {
        extern crate libc;
// { dg-error "" "" { target *-*-* } .-1 }
// { help "" "" { target *-*-* } .-2 }

        extern crate libc as x;
// { dg-error "" "" { target *-*-* } .-1 }
// { help "" "" { target *-*-* } .-2 }

        pub(in crate::foo::bar) extern crate alloc as e;

        fn dummy() {
            e::string::String::new();
        }
    }

    fn dummy() {
        c::string::String::new();
        d::string::String::new();
    }
}


fn main() {
    a::string::String::new();
    b::string::String::new();

    proc_macro::TokenStream::new();
}

