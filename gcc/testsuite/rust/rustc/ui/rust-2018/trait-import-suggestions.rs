// edition:2018
// aux-build:trait-import-suggestions.rs
// compile-flags:--extern trait-import-suggestions

mod foo {
    mod foobar {
        pub(crate) trait Foobar {
            fn foobar(&self) { }
        }

        impl Foobar for u32 { }
    }

    pub(crate) trait Bar {
        fn bar(&self) { }
    }

    impl Bar for u32 { }

    fn in_foo() {
        let x: u32 = 22;
        x.foobar(); // { dg-error ".E0599." "" { target *-*-* } }
    }
}

fn main() {
    let x: u32 = 22;
    x.bar(); // { dg-error ".E0599." "" { target *-*-* } }
    x.baz(); // { dg-error ".E0599." "" { target *-*-* } }
    let y = u32::from_str("33"); // { dg-error ".E0599." "" { target *-*-* } }
}

