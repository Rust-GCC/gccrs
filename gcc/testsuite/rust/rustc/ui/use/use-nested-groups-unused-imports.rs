#![feature(use_nested_groups)]
#![allow(dead_code)]
#![deny(unused_imports)]

mod foo {
    pub mod bar {
        pub mod baz {
            pub struct Bar();
        }
        pub mod foobar {}
    }

    pub struct Foo();
}

use foo::{Foo, bar::{baz::{}, foobar::*}, *};
// { dg-error "" "" { target *-*-* } .-1 }
use foo::bar::baz::{*, *};
// { dg-error "" "" { target *-*-* } .-1 }
use foo::{};
// { dg-error "" "" { target *-*-* } .-1 }

fn main() {
    let _: Bar;
}

