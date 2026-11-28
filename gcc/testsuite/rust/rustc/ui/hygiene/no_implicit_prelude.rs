#![feature(decl_macro)]

mod foo {
    pub macro m() { Vec::new(); ().clone() }
    fn f() { ::bar::m!(); }
}

#[no_implicit_prelude]
mod bar {
    pub macro m() {
        Vec::new(); // { dg-error ".E0433." "" { target *-*-* } }
        ().clone() // { dg-error ".E0599." "" { target *-*-* } }
    }
    fn f() {
        ::foo::m!();
        assert_eq!(0, 0); // { dg-error "" "" { target *-*-* } }
    }
}

fn main() {}

