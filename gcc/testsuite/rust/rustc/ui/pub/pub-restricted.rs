#![feature(pub_restricted)]

mod a {}

pub (a) fn afn() {} // { dg-error ".E0704." "" { target *-*-* } }
pub (b) fn bfn() {} // { dg-error ".E0704." "" { target *-*-* } }
pub (crate::a) fn cfn() {} // { dg-error ".E0704." "" { target *-*-* } }

pub fn privfn() {}
mod x {
    mod y {
        pub (in x) fn foo() {}
        pub (super) fn bar() {}
        pub (crate) fn qux() {}
    }
}

mod y {
    struct Foo {
        pub (crate) c: usize,
        pub (super) s: usize,
        valid_private: usize,
        pub (in y) valid_in_x: usize,
        pub (a) invalid: usize, // { dg-error ".E0704." "" { target *-*-* } }
        pub (in x) non_parent_invalid: usize, // { dg-error ".E0742." "" { target *-*-* } }
    }
}

fn main() {}

// test multichar names
mod xyz {}
pub (xyz) fn xyz() {} // { dg-error ".E0704." "" { target *-*-* } }

