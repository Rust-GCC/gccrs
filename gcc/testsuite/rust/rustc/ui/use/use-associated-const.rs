#![allow(unused_imports)]

pub mod foo {
    pub struct Foo;

    impl Foo {
        pub const BAR: i32 = 0;
    }
}

use foo::Foo::BAR; // { dg-error ".E0432." "" { target *-*-* } }

fn main() {}

