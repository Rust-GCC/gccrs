#![feature(crate_visibility_modifier)]

mod foo {
    struct Priv;
    mod bar {
        use foo::Priv;
        pub(super) fn f(_: Priv) {}
        pub(crate) fn g(_: Priv) {} // { dg-error ".E0446." "" { target *-*-* } }
        crate fn h(_: Priv) {} // { dg-error ".E0446." "" { target *-*-* } }
    }
}

fn main() { }

