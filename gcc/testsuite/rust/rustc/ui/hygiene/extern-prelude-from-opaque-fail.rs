#![feature(decl_macro)]

macro a() {
    extern crate core as my_core;
    mod v {
        // Early resolution.
        use my_core; // { dg-error ".E0432." "" { target *-*-* } }
    }
    mod u {
        // Late resolution.
        fn f() { my_core::mem::drop(0); }
// { dg-error ".E0433." "" { target *-*-* } .-1 }
    }
}

a!();

mod v {
    // Early resolution.
    use my_core; // { dg-error ".E0432." "" { target *-*-* } }
}
mod u {
    // Late resolution.
    fn f() { my_core::mem::drop(0); }
// { dg-error ".E0433." "" { target *-*-* } .-1 }
}

fn main() {}

