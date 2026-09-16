#![feature(decl_macro)]

mod foo {
    pub trait T {
        fn f(&self) {}
    }
    impl T for () {}
}

mod bar {
    use foo::*;
    pub macro m() { ().f() }
    fn f() { ::baz::m!(); }
}

mod baz {
    pub macro m() { ().f() } // { dg-error ".E0599." "" { target *-*-* } }
    fn f() { ::bar::m!(); }
}

fn main() {}

