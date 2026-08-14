// Test that things from the prelude aren't in scope. Use many of them
// so that renaming some things won't magically make this test fail
// for the wrong reason (e.g., if `Add` changes to `Addition`, and
// `no_implicit_prelude` stops working, then the `impl Add` will still
// fail with the same error message).

#[no_implicit_prelude]
mod foo {
    mod baz {
        struct Test;
        impl Add for Test {} // { dg-error ".E0405." "" { target *-*-* } }
        impl Clone for Test {} // { dg-error ".E0404." "" { target *-*-* } }
        impl Iterator for Test {} // { dg-error ".E0405." "" { target *-*-* } }
        impl ToString for Test {} // { dg-error ".E0405." "" { target *-*-* } }
        impl Writer for Test {} // { dg-error ".E0405." "" { target *-*-* } }

        fn foo() {
            drop(2) // { dg-error ".E0425." "" { target *-*-* } }
        }
    }

    struct Test;
    impl Add for Test {} // { dg-error ".E0405." "" { target *-*-* } }
    impl Clone for Test {} // { dg-error ".E0404." "" { target *-*-* } }
    impl Iterator for Test {} // { dg-error ".E0405." "" { target *-*-* } }
    impl ToString for Test {} // { dg-error ".E0405." "" { target *-*-* } }
    impl Writer for Test {} // { dg-error ".E0405." "" { target *-*-* } }

    fn foo() {
        drop(2) // { dg-error ".E0425." "" { target *-*-* } }
    }
}

fn qux() {
    #[no_implicit_prelude]
    mod qux_inner {
        struct Test;
        impl Add for Test {} // { dg-error ".E0405." "" { target *-*-* } }
        impl Clone for Test {} // { dg-error ".E0404." "" { target *-*-* } }
        impl Iterator for Test {} // { dg-error ".E0405." "" { target *-*-* } }
        impl ToString for Test {} // { dg-error ".E0405." "" { target *-*-* } }
        impl Writer for Test {} // { dg-error ".E0405." "" { target *-*-* } }

        fn foo() {
            drop(2) // { dg-error ".E0425." "" { target *-*-* } }
        }
    }
}


fn main() {
    // these should work fine
    drop(2)
}

