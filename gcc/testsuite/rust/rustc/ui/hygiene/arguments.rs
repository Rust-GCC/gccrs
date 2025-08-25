// ignore-pretty pretty-printing is unhygienic

#![feature(decl_macro)]

macro m($t:ty, $e:expr) {
    mod foo {
        #[allow(unused)]
        struct S;
        pub(super) fn f(_: $t) {}
    }
    foo::f($e);
}

fn main() {
    struct S;
    m!(S, S); // { dg-error ".E0412." "" { target *-*-* } }
}

