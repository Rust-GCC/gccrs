#![feature(no_core)]
#![no_core]

mod error {
    pub enum IntErrorKind {
        Foo,
        Bar,
    }
}

pub use error::IntErrorKind;

fn foo() /* { dg-warning "function is never used" } */
{
    use self::IntErrorKind::*;
}
