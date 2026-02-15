#![feature(negative_impls)]
fn main() {
    struct Foo;

    impl !Sync for Foo {}

    unsafe impl Send for &'static Foo { } // { dg-error ".E0321." "" { target *-*-* } }
}

