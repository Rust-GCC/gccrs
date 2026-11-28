#![feature(start, no_core)]
#![no_core] // makes debugging this test *a lot* easier (during resolve)

// Test to make sure that private items imported through globs remain private
// when  they're used.

mod bar {
    pub use self::glob::*;

    mod glob {
        fn gpriv() {}
    }
}

pub fn foo() {}

fn test1() {
    use bar::gpriv;
// { dg-error ".E0432." "" { target *-*-* } .-1 }
// { dg-error ".E0432." "" { target *-*-* } .-2 }

    // This should pass because the compiler will insert a fake name binding
    // for `gpriv`
    gpriv();
}

#[start] fn main(_: isize, _: *const *const u8) -> isize { 3 }

