// aux-build:two_macros.rs

extern crate two_macros;

mod foo {
    pub mod bar {
        pub use two_macros::m;
    }
}

fn f() {
    use foo::*;
    bar::m! { // { dg-error ".E0659." "" { target *-*-* } }
        mod bar { pub use two_macros::m; }
    }
}

pub mod baz {
    pub use two_macros::m;
}

fn g() {
    baz::m! { // { dg-error ".E0659." "" { target *-*-* } }
        mod baz { pub use two_macros::m; }
    }
}

fn main() {}

