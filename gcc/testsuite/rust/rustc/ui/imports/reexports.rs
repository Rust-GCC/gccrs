#![warn(unused_imports)]

mod a {
    fn foo() {}
    mod foo {}

    mod a {
        pub use super::foo; // { dg-error ".E0364." "" { target *-*-* } }
        pub use super::*;
// { dg-warning "" "" { target *-*-* } .-1 }
    }
}

mod b {
    pub fn foo() {}
    mod foo {
        pub struct S;
    }

    pub mod a {
        pub use super::foo; // This is OK since the value `foo` is visible enough.
        fn f(_: foo::S) {} // `foo` is imported in the type namespace (but not `pub` re-exported).
    }

    pub mod b {
        pub use super::*; // This is also OK since the value `foo` is visible enough.
        fn f(_: foo::S) {} // Again, the module `foo` is imported (but not `pub` re-exported).
    }
}

mod c {
    // Test that `foo` is not re-exported.
    use b::a::foo::S; // { dg-error ".E0603." "" { target *-*-* } }
    use b::b::foo::S as T; // { dg-error ".E0603." "" { target *-*-* } }
}

fn main() {}

