// aux-build:pub_restricted.rs

#![allow(warnings)]
extern crate pub_restricted;

mod foo {
    pub mod bar {
        pub(super) fn f() {}
        #[derive(Default)]
        pub struct S {
            pub(super) x: i32,
        }
        impl S {
            pub(super) fn f(&self) {}
            pub(super) fn g() {}
        }
    }
    fn f() {
        use foo::bar::S;
        pub(self) use foo::bar::f; // ok
        pub(super) use foo::bar::f as g; // { dg-error ".E0364." "" { target *-*-* } }
        S::default().x; // ok
        S::default().f(); // ok
        S::g(); // ok
    }
}

fn f() {
    use foo::bar::S;
    use foo::bar::f; // { dg-error ".E0603." "" { target *-*-* } }
    S::default().x; // { dg-error ".E0616." "" { target *-*-* } }
    S::default().f(); // { dg-error ".E0624." "" { target *-*-* } }
    S::g(); // { dg-error ".E0624." "" { target *-*-* } }
}

fn main() {
    use pub_restricted::Universe;
    use pub_restricted::Crate; // { dg-error ".E0603." "" { target *-*-* } }

    let u = Universe::default();
    let _ = u.x;
    let _ = u.y; // { dg-error ".E0616." "" { target *-*-* } }
    let _ = u.z; // { dg-error ".E0616." "" { target *-*-* } }
    u.f();
    u.g(); // { dg-error ".E0624." "" { target *-*-* } }
    u.h(); // { dg-error ".E0624." "" { target *-*-* } }
}

mod pathological {
    pub(in bad::path) mod m1 {} // { dg-error ".E0433." "" { target *-*-* } }
    pub(in foo) mod m2 {} // { dg-error ".E0742." "" { target *-*-* } }
}

