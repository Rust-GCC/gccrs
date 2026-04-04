#![feature(no_core)]
#![no_core]

mod inner {
    mod intrinsics {
        pub fn foo() {}
        pub fn bar() {}
        pub fn baz() {}
        pub fn qux() {}
    }
}

pub use inner::intrinsics::{self, bar};

fn foo() /* { dg-warning "function is never used" } */
{
    use intrinsics::{baz, foo, qux};
}
