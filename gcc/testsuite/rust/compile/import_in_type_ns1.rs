#![feature(no_core)]
#![no_core]

pub mod foo {
    pub mod bar {
        pub struct Q;
    }
}

use foo::bar; // { dg-error ".bar. defined multiple times" }

struct bar;
