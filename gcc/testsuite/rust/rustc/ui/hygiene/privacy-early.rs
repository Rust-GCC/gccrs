// edition:2018

#![feature(decl_macro)]

mod foo {
    fn f() {}
    macro f() {}

    pub macro m() {
        use f as g; // { dg-error ".E0364." "" { target *-*-* } }
        f!();
    }
}

fn main() {
    foo::m!();
}

