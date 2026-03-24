#![feature(no_core)]
#![no_core]

mod inner {
    pub mod error {
        pub enum IntErrorKind {
            A,
            B,
            C,
        }
    }
}

pub use inner::error::IntErrorKind;

fn foo() /* { dg-warning "never used" } */
{
    use self::IntErrorKind::*;

    let _ = A;
    let _ = IntErrorKind::B;
}
