#![feature(start, no_core)]
#![no_core] // makes debugging this test *a lot* easier (during resolve)

// Test to make sure that globs don't leak in regular `use` statements.

mod bar {
    pub use self::glob::*;

    pub mod glob {
        use foo;
    }
}

pub fn foo() {}

fn test1() {
    use bar::foo;
// { dg-error ".E0432." "" { target *-*-* } .-1 }
// { dg-error ".E0432." "" { target *-*-* } .-2 }
}

fn test2() {
    use bar::glob::foo;
// { dg-error ".E0603." "" { target *-*-* } .-1 }
}

#[start] fn main(_: isize, _: *const *const u8) -> isize { 3 }

